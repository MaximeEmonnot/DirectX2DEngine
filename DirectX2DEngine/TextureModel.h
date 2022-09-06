#pragma once
#include "BaseModel.h"
#include "Rect.h"
#include "TextureShader.h"
#include "Texture.h"

class TextureModel :
    public BaseModel
{
public:
    TextureModel() = default;
    ~TextureModel() override = default;
    TextureModel(const std::string& texture_path);
    TextureModel(const Texture& texture); 
    DirectX::XMFLOAT2 GetOffset() const;
    DirectX::XMMATRIX GetRotationMatrix() const;
    DirectX::XMMATRIX GetScaleMatrix() const;

    void SetPosition(const FVec2D& new_pos);
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

