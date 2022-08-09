#pragma once
#include "BaseModel.h"
#include "TextureShader.h"
#include "Texture.h"

class TextureModel :
    public BaseModel
{
public:
    TextureModel() = default;
    ~TextureModel() override = default;
    TextureModel(const std::string& texture_path);
    TextureModel(Texture& texture);

    virtual void Update() override;

    DirectX::XMMATRIX GetTranslationMatrix() const;
    DirectX::XMMATRIX GetRotationMatrix() const;
    DirectX::XMMATRIX GetScaleMatrix() const;
    void SetTexture(const Texture& tex);
    Texture GetTexture() const;

protected:
    virtual void SettingShader() override;
    virtual void SettingRenderingConstants() override;
    virtual void SettingBufferConstants() override;
    virtual void SettingVertices() override;

private:
    int x = 0, y = 0;
    Texture texture;
    float angle = 0.f;

    TextureShader::VertexInput texture_vertices[6];
};

