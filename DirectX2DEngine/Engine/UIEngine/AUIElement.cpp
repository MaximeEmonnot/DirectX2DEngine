#include "UIEngine/AUIElement.h"

AUIElement::AUIElement(const FRect& position)
	:
	position(position)
{
}

void AUIElement::SetPosition(const FRect& newPosition)
{
	position = newPosition;
}

void AUIElement::Update()
{
	
}
