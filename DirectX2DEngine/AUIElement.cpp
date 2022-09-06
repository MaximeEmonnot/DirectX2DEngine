#include "AUIElement.h"

AUIElement::AUIElement(const FRect& position)
	:
	position(position)
{
}

void AUIElement::SetPosition(const FRect& new_position)
{
	position = new_position;
}

void AUIElement::Update()
{
	
}
