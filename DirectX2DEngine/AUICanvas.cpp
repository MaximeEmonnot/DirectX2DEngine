#include "AUICanvas.h"

void AUICanvas::Render() const
{
	for (const std::shared_ptr<AUIElement>& ui : UIElements) ui->Render();
}
