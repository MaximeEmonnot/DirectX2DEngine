#pragma once

#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIButton.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"

class UICanvas_Title : public AUICanvas
{
public:
	UICanvas_Title();

	virtual void	Update() override;

private:
	std::shared_ptr<UIImage>	pTitleLogo;
	std::shared_ptr<UIButton>	pSinglePlayerButton;
	std::shared_ptr<UIText>		pSinglePlayerButtonTitle;
	std::shared_ptr<UIButton>	pMultiPlayerButton;
	std::shared_ptr<UIText>		pMultiPlayerButtonTitle;
};

