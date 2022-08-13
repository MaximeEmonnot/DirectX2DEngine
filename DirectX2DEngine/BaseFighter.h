#pragma once
#include "Actor.h"
#include "AnimationSystem.h"
#include "TextureModel.h"

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

            const Action value;
            std::string result;
            std::vector<std::shared_ptr<ComboNode>> leaves;
        };
    public:
        ComboTree();

        void UpdateTree();

        std::string GetCurrentCombo();
    private:
        IVec2D GetCommandInput() const;

    protected:
        std::shared_ptr<ComboNode> root;
    private:
        std::shared_ptr<ComboNode> currentStage;

        Action lastAction = Action::None;
        float timer = 0.125f;
    };

protected:
    BaseFighter(Actor& owner, const std::string jsonPath, std::shared_ptr<ComboTree> pComboTree);

public:
    virtual void Update();

    std::vector<std::shared_ptr<Collider>> GetColliders() const;

protected:
    Actor& owner;
    AnimationSystem animSys;
    std::shared_ptr<TextureModel> model;
    std::shared_ptr<ComboTree> pComboTree;
};

