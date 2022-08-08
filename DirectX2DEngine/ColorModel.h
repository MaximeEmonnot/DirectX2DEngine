#pragma once
#include "BaseModel.h"
#include "ColorShader.h"

class ColorModel :
    public BaseModel
{
public:
    ColorModel() = default;
    ColorModel(int x, int y, int width, int height, DirectX::XMFLOAT4 color);
    ~ColorModel() override = default;

    virtual void Update() override;

    DirectX::XMMATRIX GetTranslationMatrix() const;
    DirectX::XMMATRIX GetRotationMatrix() const;
    DirectX::XMMATRIX GetScaleMatrix() const;
    DirectX::XMFLOAT4 GetColor() const;

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

