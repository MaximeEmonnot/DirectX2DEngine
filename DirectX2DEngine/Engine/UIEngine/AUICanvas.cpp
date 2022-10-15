#include "UIEngine/AUICanvas.h"

void AUICanvas::Render() const
{
	for (const std::shared_ptr<AUIElement>& pUI : UIElements) pUI->Render();
}
