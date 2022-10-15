#pragma once

#include "BaseModel.h"
#include "ColorShader.h"
#include "MathEngine/Rect.h"

class ColorModel :
    public BaseModel
{
public:
    ColorModel() = default;
    ColorModel(int x, int y, int width, int height, DirectX::XMFLOAT4 color);
    ColorModel(FRect rect, DirectX::XMFLOAT4 color);
    ~ColorModel() override = default;

    DirectX::XMFLOAT2   GetOffset() const;
    DirectX::XMMATRIX   GetRotationMatrix() const;
    DirectX::XMMATRIX   GetScaleMatrix() const;
    DirectX::XMFLOAT4   GetColor() const;

    void                SetColor(const DirectX::XMFLOAT4& newColor);
    void                SetPosition(const FVec2D& newPos);
    void                SetRectangle(const FRect& newRect);

protected:
    virtual void        SettingShader() override;
    virtual void        SettingRenderingConstants() override;
    virtual void        SettingBufferConstants() override;
    virtual void        SettingVertices() override;

private:
    int                         x;
	int                         y;
    int                         width;
	int                         height;
    DirectX::XMFLOAT4           color;
    float                       angle;

    ColorShader::VertexInput    colorVertices[6] = {};
};

