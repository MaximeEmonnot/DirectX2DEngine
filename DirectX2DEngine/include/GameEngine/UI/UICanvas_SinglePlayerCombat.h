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
    UICanvas_SinglePlayerCombat(std::shared_ptr<BaseFighter> pPlayer, std::shared_ptr<BaseFighter> pEnemy);

    void    Update() override;

private:
    std::shared_ptr<UIProgressBar>  pPlayerHealthBar;
    std::shared_ptr<UIImage>        pPlayerIcon;
    std::shared_ptr<UIText>         pPlayerName;
    std::shared_ptr<UIProgressBar>  pEnemyHealthBar;
    std::shared_ptr<UIImage>        pEnemyIcon;
    std::shared_ptr<UIText>         pEnemyName;
};

