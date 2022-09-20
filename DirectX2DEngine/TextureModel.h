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
    void SetRectangle(const FRect& rectangle);
    void SetTexture(const Texture& tex);
    Texture GetTexture() const;

protected:
    virtual void SettingShader() override;
    virtual void SettingRenderingConstants() override;
    virtual void SettingBufferConstants() override;
    virtual void SettingVertices() override;

private:
    Texture texture;
    int x = 0, y = 0;
    int height = 0, width = 0;
    float angle = 0.f;

    TextureShader::VertexInput texture_vertices[6] = {};
    TextureShader::VertexInput inverted_texture_vertices[6] = {};
};

