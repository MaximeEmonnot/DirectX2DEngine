export module GameEngine:Fighters;

import <string>;
import <vector>;
import <memory>;
import <random>;

import MathEngine;
import CoreEngine;
import ParserEngine;

import GameEngine;
import Controllers;

export
class BaseFighter
{
public:
    enum class Action
    {
        None,
        Right,
        DownRight,
        Down,
        DownLeft,
        Left,
        UpLeft,
        Up,
        UpRight
    };
protected:
    class ComboTree
    {
    protected:
        struct ComboNode
        {
            ComboNode(const Action& value, const std::string& result = "")
                :
                value(value),
                result(result)
            {}

            bool operator== (const Action& action)
            {
                return value == action;
            }

            const Action                                value;
            std::string                                 result;
            std::vector<std::shared_ptr<ComboNode>>     leaves;
        };
    public:
        ComboTree()
            :
            pRoot(std::make_shared<ComboNode>(Action::None)),
            pCurrentStage(pRoot)
        {}

        void            UpdateTree()
        {
            const IVec2D input = GetCommandInput();

            // We transform the CommandInput into an Action (X = 1, Y = 0 gives Right; X = 0, Y = 1 gives Up and so on)
            Action new_action = Action::None;
            switch (input.x)
            {
            case 1:
                switch (input.y)
                {
                case 1:
                    new_action = Action::DownRight;
                    break;
                case 0:
                    new_action = Action::Right;
                    break;
                case -1:
                    new_action = Action::UpRight;
                    break;
                default:
                    break;
                }
                break;
            case 0:
                switch (input.y)
                {
                case 1:
                    new_action = Action::Down;
                    break;
                case -1:
                    new_action = Action::Up;
                    break;
                default:
                    break;
                }
                break;
            case -1:
                switch (input.y)
                {
                case 1:
                    new_action = Action::DownLeft;
                    break;
                case 0:
                    new_action = Action::Left;
                    break;
                case -1:
                    new_action = Action::UpLeft;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }

            // If the current Action is different from the last one...
            if (new_action != lastAction)
            {
                // ... we update the last Action and we move the CurrentStage pointer in the ComboTree, as each node is associated to an Action.
                lastAction = new_action;
                for (const std::shared_ptr<ComboNode> leaf : pCurrentStage->leaves)
                {
                    if (*leaf == new_action)
                    {
                        pCurrentStage = leaf;
                        timer = 0.3f;
                        break;
                    }
                }
            }
            else timer -= DELTATIME; // If it was the same action, we update the Timer

            // When the timer reaches 0.0, the CurrentStage pointer returns to the Root of the ComboTree
            if (timer <= 0.0f)
            {
                lastAction = Action::None;
                pCurrentStage = pRoot;
                timer = 0.3f;
            }
        }

        std::string     GetCurrentCombo()
        {
            std::string out = pCurrentStage->result;
            lastAction = Action::None;
            timer = 0.3f;
            pCurrentStage = pRoot;
            return out;
        }
    private:
        IVec2D          GetCommandInput() const
        {
            IVec2D out;
            if (KBD.KeyIsPressed(Commands::JUMP)) out.y--;
            if (KBD.KeyIsPressed(Commands::RIGHT)) out.x++;
            if (KBD.KeyIsPressed(Commands::DOWN)) out.y++;
            if (KBD.KeyIsPressed(Commands::LEFT)) out.x--;
            return out;
        }

    protected:
        std::shared_ptr<ComboNode>  pRoot;
    private:
        std::shared_ptr<ComboNode>  pCurrentStage;

        Action                      lastAction = Action::None;
        float                       timer = 0.125f;
    };

protected:
    BaseFighter(Actor& owner, const std::string jsonPath, std::shared_ptr<ComboTree> pComboTree, int maxHealth, int priority)
        :
        owner(owner),
        pComboTree(pComboTree),
        animSys(jsonPath),
        pModel(owner.GetWorld().CreateModel<TextureModel>(priority)),
        collisionSys(owner, jsonPath, animSys.GetAnimationList()),
        maxHealth(maxHealth),
        currentHealth(maxHealth),
        pController(std::dynamic_pointer_cast<FightingController>(dynamic_cast<Pawn*>(&owner)->GetController()))
    {
        icon = JSON(jsonPath).GetValueOf("character").GetString() + std::string("icon.tga");
    }

public:
    virtual ~BaseFighter() = default;

    virtual void                            Update()
    {
        // The fighter faces his enemy based on his position 
        if (const std::shared_ptr<BaseFighter> shared_enemy = pEnemy.lock()) {
            const bool bIsInverted = owner.GetPosition().x < shared_enemy->owner.GetPosition().x;
            pModel->SetInverted(bIsInverted);
            collisionSys.SetDirection(bIsInverted ? -1 : 1);
            std::dynamic_pointer_cast<FightingController>(dynamic_cast<Pawn*>(&owner)->GetController())->SetLookAtDirection(!bIsInverted);
        }

        pComboTree->UpdateTree();

        // We read the result from the ComboTree
        if (KBD.KeyIsPressed(Commands::PUNCH)) {
            const std::string test = pComboTree->GetCurrentCombo();
            if (!test.empty()) OutputDebugStringA((test + std::string("\n")).c_str());
        }

        // Animation updates
        animSys.Update();
        FVec2D texture_center = animSys.GetTexture().GetCenter();
        if (pModel->IsInverted()) texture_center.x *= -1;
        pModel->SetPosition(owner.GetPosition() - texture_center);
        pModel->SetTexture(animSys.GetTexture());

        collisionSys.Update();
    }

    void                                    SetEnemy(std::weak_ptr<BaseFighter> _pEnemy)
    {
        pEnemy = _pEnemy;
    }
    void                                    ApplyDamage(int damage)
    {
        currentHealth = (currentHealth - damage >= 0) ? currentHealth - damage : 0;
    }

    std::string                             GetIcon() const
    {
        return icon;
    }
    std::wstring                            GetName() const
    {
        return name;
    }

    std::vector<std::shared_ptr<Collider>>  GetColliders() const
    {
        std::vector<std::shared_ptr<Collider>> out;
        for (std::shared_ptr<Collider>& pCollider : collisionSys.GetColliders()) out.emplace_back(pCollider);
        return out;
    }

    float                                   GetHealth() const
    {
        return static_cast<float>(currentHealth) / static_cast<float>(maxHealth);
    }
    bool                                    IsAlive() const
    {
        return currentHealth != 0;
    }

protected:
    void AddAnimationTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
    {
        animSys.AddTransition(from, to, condition);
        collisionSys.AddTransition(from, to, condition);
    }

protected:
    Actor& owner;
    AnimationSystem                         animSys;
    CollisionSystem                         collisionSys;
    std::shared_ptr<TextureModel>           pModel;
    std::shared_ptr<ComboTree>              pComboTree;
    std::string                             icon;
    std::wstring                            name;

    std::weak_ptr<BaseFighter>              pEnemy;
    std::shared_ptr<FightingController>     pController;

    int                                     maxHealth;
    int                                     currentHealth;
};


export
class SolBadguy :
    public BaseFighter
{
private:
    class SolComboTree : public ComboTree
    {
    public:
        SolComboTree()
        {
            /// COMBOTREE INITIALIZATION
            // Down
            pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down, "Down"));
            // Down DownRight Right
            pRoot->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
            pRoot->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Quarter Circle Front"));
            // Down DownLeft Left
            pRoot->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
            pRoot->leaves.at(0)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Quarter Circle Back"));
            // Down DownLeft Left Down DownLeft Left
            pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
            pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
            pRoot->leaves.at(0)->leaves.at(1)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Double Quarter Circle Back"));

            // Left Left
            pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
            pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left, "Escape"));
            // Left Right
            pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right));
            // Left DownLeft Down DownRight Right
            pRoot->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
            pRoot->leaves.at(1)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
            pRoot->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
            pRoot->leaves.at(1)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Half Circle Front"));

            // Right
            pRoot->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Right"));
            // Right Right
            pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Dash"));
            // Right Down DownRight
            pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
            pRoot->leaves.at(2)->leaves.at(1)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight, "Dragon Punch"));
            // Right DownRight Down DownLeft Left Right
            pRoot->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownRight));
            pRoot->leaves.at(2)->leaves.at(2)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Down));
            pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::DownLeft));
            pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Left));
            pRoot->leaves.at(2)->leaves.at(2)->leaves.at(0)->leaves.at(0)->leaves.at(0)->leaves.emplace_back(std::make_shared<ComboNode>(Action::Right, "Overdrive"));
        }
    };

public:
    SolBadguy(Actor& owner, int priority)
        :
        BaseFighter(owner, "json\\sol.json", std::make_shared<SolComboTree>(), 140, priority)
    {
        // Name initialization
        name = L"Sol Badguy";

        /// TRANSITIONS
        // Idle - RandomIdle
        AddAnimationTransition("Idle", "RandomIdle", [&] { return bIdleToRandomIdle; });
        AddAnimationTransition("RandomIdle", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // WalkingForward
        AddAnimationTransition("Idle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
        AddAnimationTransition("RandomIdle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
        AddAnimationTransition("WalkingForward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Front); });

        // WalkingBack
        AddAnimationTransition("Idle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
        AddAnimationTransition("RandomIdle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
        AddAnimationTransition("WalkingBackward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Back); });

        // Crouch
        AddAnimationTransition("Idle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("RandomIdle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("CrouchTransition", "CrouchIdle", [&] { return pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });
        AddAnimationTransition("CrouchIdle", "CrouchTransition", [&] { return !pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("CrouchTransition", "Idle", [&] {return !pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });

        // Punch
        AddAnimationTransition("Idle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
        AddAnimationTransition("RandomIdle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
        AddAnimationTransition("Punch", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Kick
        AddAnimationTransition("Idle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
        AddAnimationTransition("RandomIdle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
        AddAnimationTransition("Kick", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Slash
        AddAnimationTransition("Idle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
        AddAnimationTransition("RandomIdle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
        AddAnimationTransition("Slash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // HardSlash
        AddAnimationTransition("Idle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
        AddAnimationTransition("RandomIdle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
        AddAnimationTransition("HardSlash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Jump
        AddAnimationTransition("Idle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("RandomIdle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("WalkingForward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("WalkingBackward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("Jump", "MaxJump", [&] {return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });
        AddAnimationTransition("MaxJump", "Fall", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });


        // Fall
        AddAnimationTransition("Idle", "Fall", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
                return false;
            });
        AddAnimationTransition("RandomIdle", "Fall", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
                return false;
            });
        AddAnimationTransition("Fall", "Idle", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return player->IsGrounded();
                return false;
            });
    }

    virtual void Update() override
    {
        // We update the condition for the transition from Idle to RandomIdle BEFORE the parent class Update()
        // so that the transition will occur during the current frame and not during the next frame
        IdleToRandomIdleConditionUpdate();

        BaseFighter::Update();
    }

private:
    void IdleToRandomIdleConditionUpdate()
    {
        if (animSys.AnimationIsFinished()) {
            std::mt19937 rng(std::random_device{}());
            std::uniform_int_distribution<int> dist(0, 500);
            bIdleToRandomIdle = dist(rng) == 400;
        }
        else bIdleToRandomIdle = false;
    }

private:
    bool bIdleToRandomIdle = false;
};


export
class RoboKy : public BaseFighter
{
private:
    class RoboKyComboTree : public ComboTree
    {
    public:
        RoboKyComboTree() {}
    };
public:
    RoboKy(Actor& owner, int priority)
        :
        BaseFighter(owner, "json\\roboky.json", std::make_shared<RoboKyComboTree>(), 120, priority)
    {
        // Name Initialization
        name = L"Robo Ky";

        // ANIMATIONS TRANSITIONS
        // WalkingForward
        AddAnimationTransition("Idle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
        AddAnimationTransition("RandomIdle", "WalkingForward", [&] { return pController->IsDoingAction(FightingController::EAction::Front); });
        AddAnimationTransition("WalkingForward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Front); });

        // WalkingBack
        AddAnimationTransition("Idle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
        AddAnimationTransition("RandomIdle", "WalkingBackward", [&] { return pController->IsDoingAction(FightingController::EAction::Back); });
        AddAnimationTransition("WalkingBackward", "Idle", [&] { return !pController->IsDoingAction(FightingController::EAction::Back); });

        // Crouch
        AddAnimationTransition("Idle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("RandomIdle", "CrouchTransition", [&] { return pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("CrouchTransition", "CrouchIdle", [&] { return pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });
        AddAnimationTransition("CrouchIdle", "CrouchTransition", [&] { return !pController->IsDoingAction(FightingController::EAction::Down); });
        AddAnimationTransition("CrouchTransition", "Idle", [&] {return !pController->IsDoingAction(FightingController::EAction::Down) && (animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished()); });

        // Punch
        AddAnimationTransition("Idle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
        AddAnimationTransition("RandomIdle", "Punch", [&] { return pController->IsDoingAction(FightingController::EAction::Punch); });
        AddAnimationTransition("Punch", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Kick
        AddAnimationTransition("Idle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
        AddAnimationTransition("RandomIdle", "Kick", [&] { return pController->IsDoingAction(FightingController::EAction::Kick); });
        AddAnimationTransition("Kick", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Slash
        AddAnimationTransition("Idle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
        AddAnimationTransition("RandomIdle", "Slash", [&] { return pController->IsDoingAction(FightingController::EAction::Slash); });
        AddAnimationTransition("Slash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // HardSlash
        AddAnimationTransition("Idle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
        AddAnimationTransition("RandomIdle", "HardSlash", [&] { return pController->IsDoingAction(FightingController::EAction::HardSlash); });
        AddAnimationTransition("HardSlash", "Idle", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Jump
        AddAnimationTransition("Idle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("RandomIdle", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("WalkingForward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("WalkingBackward", "Jump", [&] { return pController->IsDoingAction(FightingController::EAction::Jump); });
        AddAnimationTransition("Jump", "MaxJump", [&] {return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });
        AddAnimationTransition("MaxJump", "Fall", [&] { return animSys.AnimationIsFinished() || collisionSys.AnimationIsFinished(); });

        // Fall
        AddAnimationTransition("Idle", "Fall", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
                return false;
            });
        AddAnimationTransition("RandomIdle", "Fall", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return !player->IsGrounded();
                return false;
            });
        AddAnimationTransition("Fall", "Idle", [&]
            {
                if (const Character* player = dynamic_cast<Character*>(&owner)) return player->IsGrounded();
                return false;
            });
    }
};

