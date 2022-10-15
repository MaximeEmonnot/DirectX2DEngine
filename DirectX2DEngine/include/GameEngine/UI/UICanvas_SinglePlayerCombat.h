#pragma once

#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"
#include "UIEngine/UIProgressBar.h"
#include "GameEngine/Fighters/BaseFighter.h"

class UICanvas_SinglePlayerCombat :
    public AUICanvas
{
public:
    UICanvas_SinglePlayerCombat(std::shared_ptr<BaseFighter> player, std::shared_ptr<BaseFighter> enemy);

    void Update() override;

private:
    std::shared_ptr<UIProgressBar> playerHealthBar;
    std::shared_ptr<UIImage> playerIcon;
    std::shared_ptr<UIText> playerName;
    std::shared_ptr<UIProgressBar> enemyHealthBar;
    std::shared_ptr<UIImage> enemyIcon;
    std::shared_ptr<UIText> enemyName;
};

