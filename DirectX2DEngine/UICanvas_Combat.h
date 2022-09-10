#pragma once
#include "AUICanvas.h"
#include "BaseFighter.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
#include "UIProgressBar.h"

class UICanvas_Combat :
    public AUICanvas
{
public:
    UICanvas_Combat(std::shared_ptr<BaseFighter> player, std::shared_ptr<BaseFighter> enemy);

    void Update() override;

private:
    std::shared_ptr<UIProgressBar> playerHealthBar;
    std::shared_ptr<UIImage> playerIcon;
    std::shared_ptr<UIText> playerName;
    std::shared_ptr<UIProgressBar> enemyHealthBar;
    std::shared_ptr<UIImage> enemyIcon;
    std::shared_ptr<UIText> enemyName;
};

