#include "GameEngine/Fighters/BaseFighter.h"

#include "CoreEngine/Keyboard.h"
#include "CoreEngine/Timer.h"
#include "GraphicsEngine/AnimationTexture.h"
#include "ParserEngine/JSONParser.h"
#include "MainEngine/Engine.h"
#include "GameEngine/Commands.h"

//**** COMBO TREE ****//

BaseFighter::ComboTree::ComboTree()
    :
    pRoot(std::make_shared<ComboNode>(Action::None)),
    pCurrentStage(pRoot)
{}

void BaseFighter::ComboTree::UpdateTree()
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

std::string BaseFighter::ComboTree::GetCurrentCombo()
{
    std::string out = pCurrentStage->result;
    lastAction = Action::None;
    timer = 0.3f;
    pCurrentStage = pRoot;
    return out;
}

IVec2D BaseFighter::ComboTree::GetCommandInput() const
{
    IVec2D out;
    if (KBD.KeyIsPressed(Commands::JUMP)) out.y--;
    if (KBD.KeyIsPressed(Commands::RIGHT)) out.x++;
    if (KBD.KeyIsPressed(Commands::DOWN)) out.y++;
    if (KBD.KeyIsPressed(Commands::LEFT)) out.x--;
    return out;
}

//**** BASE FIGHTER ****//

BaseFighter::BaseFighter(Actor& owner, const std::string jsonPath, std::shared_ptr<ComboTree> pComboTree, int maxHealth, int priority)
    :
    owner(owner),
    pComboTree(pComboTree),
    animSys(jsonPath),
	pModel(owner.GetWorld().CreateModel<TextureModel>(priority)),
    collisionSys(owner, jsonPath, animSys.GetAnimationList()),
	maxHealth(maxHealth),
	currentHealth(maxHealth)
{
    JSONParser::Reader jsonParser;
    jsonParser.ReadFile(jsonPath);
    icon = jsonParser.GetValueOf("character").GetString() + std::string("icon.tga");
}

void BaseFighter::Update()
{
    // The fighter faces his enemy based on his position 
    if (const std::shared_ptr<BaseFighter> shared_enemy = pEnemy.lock()) {
        const bool bIsInverted = owner.GetPosition().x < shared_enemy->owner.GetPosition().x;
        pModel->SetInverted(bIsInverted);
        collisionSys.SetDirection(bIsInverted ? -1 : 1);
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

void BaseFighter::SetEnemy(std::weak_ptr<BaseFighter> _pEnemy)
{
    pEnemy = _pEnemy;
}

void BaseFighter::ApplyDamage(int damage)
{
    currentHealth = (currentHealth - damage >= 0) ? currentHealth - damage : 0;
}

std::string BaseFighter::GetIcon() const
{
    return icon;
}

std::wstring BaseFighter::GetName() const
{
    return name;
}

std::vector<std::shared_ptr<Collider>> BaseFighter::GetColliders() const
{
    std::vector<std::shared_ptr<Collider>> out;
    for (std::shared_ptr<Collider>& pCollider : collisionSys.GetColliders()) out.emplace_back(pCollider);
    return out;
}

float BaseFighter::GetHealth() const
{
    return static_cast<float>(currentHealth) / static_cast<float>(maxHealth);
}

bool BaseFighter::IsAlive() const
{
    return currentHealth != 0;
}

void BaseFighter::AddAnimationTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition)
{
    animSys.AddTransition(from, to, condition);
    collisionSys.AddTransition(from, to, condition);
}
