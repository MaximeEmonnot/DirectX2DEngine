#pragma once
#include "BaseModel.h"
#include "ColorShader.h"
#include "Rect.h"

class ColorModel :
    public BaseModel
{
public:
    ColorModel() = default;
    ColorModel(int x, int y, int width, int height, DirectX::XMFLOAT4 color);
    ColorModel(FRect rect, DirectX::XMFLOAT4 color);
    ~ColorModel() override = default;

    DirectX::XMMATRIX GetTranslationMatrix() const;
    DirectX::XMMATRIX GetRotationMatrix() const;
    DirectX::XMMATRIX GetScaleMatrix() const;
    DirectX::XMFLOAT4 GetColor() const;

    void SetColor(const DirectX::XMFLOAT4& new_color);
    void SetPosition(const FVec2D& new_pos);
    void SetRectangle(const FRect& new_rect);

protected:
    virtual void SettingShader() override;
    virtual void SettingRenderingConstants() override;
    virtual void SettingBufferConstants() override;
    virtual void SettingVertices() override;

private:
    int x, y;
    int width, height;
    DirectX::XMFLOAT4 color;
    float angle;

    ColorShader::VertexInput color_vertices[6];
};

