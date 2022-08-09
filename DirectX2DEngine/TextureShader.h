#pragma once
#include "BaseShader.h"
class TextureShader :
    public BaseShader
{
public:
    struct VertexInput
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 uv;
    };

    struct VariableBuffer
    {
        DirectX::XMMATRIX translation;
        DirectX::XMMATRIX rotation;
        DirectX::XMMATRIX scale;
    };

public:
    TextureShader() = default;

    virtual void Initialize() override;

    virtual void RenderModel(BaseModel& model) override;

protected:
    virtual HRESULT InputLayoutCreation() override;
    virtual HRESULT VertexShaderCreation() override;
    virtual HRESULT PixelShaderCreation() override;
    virtual HRESULT SampleStateCreation();
    virtual HRESULT ConstantBufferCreation() override;

private:
    Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
};

