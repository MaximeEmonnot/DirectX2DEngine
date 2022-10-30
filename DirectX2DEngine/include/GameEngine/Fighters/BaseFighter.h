#pragma once

#include "GraphicsEngine/TextureModel.h"
#include "GraphicsEngine/AnimationSystem.h"
#include "MainEngine/Actor.h"
#include "MainEngine/CollisionSystem.h"

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
        ComboTree();

        void            UpdateTree();

        std::string     GetCurrentCombo();
    private:
        IVec2D          GetCommandInput() const;

    protected:
        std::shared_ptr<ComboNode>  pRoot;
    private:
        std::shared_ptr<ComboNode>  pCurrentStage;

        Action                      lastAction = Action::None;
        float                       timer = 0.125f;
    };

protected:
    BaseFighter(Actor& owner, const std::string jsonPath, std::shared_ptr<ComboTree> pComboTree, int priority);

public:
    virtual ~BaseFighter() = default;

    virtual void                            Update();

    void                                    SetEnemy(std::weak_ptr<BaseFighter> _pEnemy);

    std::string                             GetIcon() const;
    std::wstring                            GetName() const;

    std::vector<std::shared_ptr<Collider>>  GetColliders() const;

protected:
    void AddAnimationTransition(const std::string& from, const std::string& to, const std::function<bool()>& condition);

protected:
    Actor&                          owner;
    AnimationSystem                 animSys;
    CollisionSystem                 collisionSys;
    std::shared_ptr<TextureModel>   pModel;
    std::shared_ptr<ComboTree>      pComboTree;
    std::string                     icon;
    std::wstring                    name;

    std::weak_ptr<BaseFighter>      pEnemy;
};

