#pragma once
#include "UIEngine/AUICanvas.h"
#include "UIEngine/UIButton.h"
#include "UIEngine/UIImage.h"
#include "UIEngine/UIText.h"

class UICanvas_Title : public AUICanvas
{
public:
	UICanvas_Title();

	virtual void Update() override;

private:
	std::shared_ptr<UIImage> titleLogo;
	std::shared_ptr<UIButton> singlePlayerButton;
	std::shared_ptr<UIText> singlePlayerButtonTitle;
	std::shared_ptr<UIButton> multiPlayerButton;
	std::shared_ptr<UIText> multiPlayerButtonTitle;
};

