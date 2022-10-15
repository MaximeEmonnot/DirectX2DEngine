#pragma once

#include "BaseShader.h"

class ColorShader :
    public BaseShader
{
public:
    struct VertexInput
    {
        DirectX::XMFLOAT3   position;
    };

    struct VariableBuffer
    {
        DirectX::XMFLOAT2   offset;
        DirectX::XMMATRIX   rotation;
        DirectX::XMMATRIX   scale;
        DirectX::XMFLOAT4   color;
        float               depth;
    };

public:
    ColorShader() = default;

    virtual void        RenderModel(BaseModel& model) override;

protected:
    virtual HRESULT     InputLayoutCreation() override;
    virtual HRESULT     VertexShaderCreation() override;
    virtual HRESULT     PixelShaderCreation() override;
    virtual HRESULT     ConstantBufferCreation() override;
};

