#pragma once
#include "BaseShader.h"
class ColorShader :
    public BaseShader
{
public:
    struct VertexInput
    {
        DirectX::XMFLOAT3 position;
    };

    struct VariableBuffer
    {
        DirectX::XMMATRIX translation;
        DirectX::XMMATRIX rotation;
        DirectX::XMMATRIX scale;
        DirectX::XMFLOAT4 color;
    };

public:
    ColorShader() = default;

    virtual void RenderModel(BaseModel& model) override;

protected:
    virtual HRESULT InputLayoutCreation() override;
    virtual HRESULT VertexShaderCreation() override;
    virtual HRESULT PixelShaderCreation() override;
    virtual HRESULT ConstantBufferCreation() override;
};

