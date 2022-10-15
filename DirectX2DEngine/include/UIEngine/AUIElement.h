#pragma once

#include "MathEngine/Rect.h"

class AUIElement
{
public:
	AUIElement() = default;
	AUIElement(const FRect& position);
	virtual ~AUIElement() = default;

	virtual void SetPosition(const FRect& newPosition);

	virtual void Update();
	virtual void Render() const = 0;

protected:
	FRect position;
	static constexpr float depth = 256.f;
};

