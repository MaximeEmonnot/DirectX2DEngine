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
        DirectX::XMFLOAT2 offset;
        DirectX::XMMATRIX rotation;
        DirectX::XMMATRIX scale;
        float depth;
    };

public:
    TextureShader() = default;
    virtual ~TextureShader() override;

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

