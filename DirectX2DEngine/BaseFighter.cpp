#include "BaseFighter.h"

#include "AnimationTexture.h"
#include "Commands.h"
#include "Engine.h"
#include "JSONParser.h"
#include "Keyboard.h"
#include "Timer.h"

//**** COMBO TREE ****//
BaseFighter::ComboTree::ComboTree()
    :
    root(std::make_shared<ComboNode>(Action::None)),
    currentStage(root)
{}

void BaseFighter::ComboTree::UpdateTree()
{
    const IVec2D input = GetCommandInput();

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

    if (new_action != lastAction)
    {
        lastAction = new_action;
        for (const std::shared_ptr<ComboNode> leaf : currentStage->leaves)
        {
            if (*leaf == new_action)
            {
                currentStage = leaf;
                timer = 0.3f;
                break;
            }
        }
    }
    else timer -= DELTATIME;

    if (timer <= 0.0f)
    {
        lastAction = Action::None;
        currentStage = root;
        timer = 0.3f;
    }
}

std::string BaseFighter::ComboTree::GetCurrentCombo()
{
    std::string out = currentStage->result;
    lastAction = Action::None;
    timer = 0.3f;
    currentStage = root;
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

/// BASE FIGHTER ///

BaseFighter::BaseFighter(Actor& owner, const std::string jsonPath, std::shared_ptr<ComboTree> pComboTree, int priority)
    :
    owner(owner),
    pComboTree(pComboTree),
    animSys(jsonPath),
	model(owner.GetWorld().CreateModel<TextureModel>(priority))
    //collisionSys(owner, jsonPath, animSys.GetAnimationList())_
{
    JSONParser::Reader jsonParser;
    jsonParser.ReadFile(jsonPath);
    icon = jsonParser.GetValueOf("character").GetString() + std::string("icon.tga");
}

BaseFighter::~BaseFighter()
{
}

void BaseFighter::Update()
{
    if (const std::shared_ptr<BaseFighter> shared_enemy = pEnemy.lock()) 
        model->SetInverted(owner.GetPosition().x < shared_enemy->owner.GetPosition().x);

    pComboTree->UpdateTree();
    if (KBD.KeyIsPressed(Commands::PUNCH)) {
        const std::string test = pComboTree->GetCurrentCombo();
        if (!test.empty()) OutputDebugStringA((test + std::string("\n")).c_str());
    }
    animSys.Update();

    FVec2D texture_center = animSys.GetTexture().GetCenter();
    if (model->IsInverted()) texture_center.x *= -1;

    model->SetPosition(owner.GetPosition() - texture_center);
    model->SetTexture(animSys.GetTexture());
    
    //collisionSys.Update();
}

void BaseFighter::SetEnemy(std::weak_ptr<BaseFighter> enemy)
{
    pEnemy = enemy;
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
    //for (std::shared_ptr<Collider>& c : collisionSys.GetColliders()) out.emplace_back(c);
    return out;
}
