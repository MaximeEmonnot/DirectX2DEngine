#pragma once
#include "AUICanvas.h"
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"

class UICanvas_Title : public AUICanvas
{
public:
	UICanvas_Title();

	virtual void Update() override;

private:
	std::shared_ptr<UIImage> titleLogo;
	std::shared_ptr<UIButton> startButton;
	std::shared_ptr<UIText> startButtonTitle;
};

