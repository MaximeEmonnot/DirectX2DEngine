#pragma once

#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"

class UICanvas_Victory :
	public AUICanvas
{
public:
	UICanvas_Victory(const std::string& winnerName, int playerNumber);

	virtual void	Update() override;

private:
	std::shared_ptr<UIText>		pVictoryText;
	std::shared_ptr<UIImage>	pVictoryBackground;
};