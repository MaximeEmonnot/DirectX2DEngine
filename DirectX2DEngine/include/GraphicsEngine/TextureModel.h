#pragma once

#include "BaseModel.h"
#include "TextureShader.h"
#include "Texture.h"
#include "MathEngine/Rect.h"

class TextureModel :
    public BaseModel
{
public:
    TextureModel() = default;
    ~TextureModel() override = default;
    TextureModel(const std::string& texturePath);
    TextureModel(const Texture& texture);

    DirectX::XMFLOAT2   GetOffset() const;
    DirectX::XMMATRIX   GetRotationMatrix() const;
    DirectX::XMMATRIX   GetScaleMatrix() const;

    void                SetPosition(const FVec2D& newPos);
    void                SetRectangle(const FRect& newRectangle);
    void                SetTexture(const Texture& newTexture);
    Texture             GetTexture() const;

protected:
    virtual void        SettingShader() override;
    virtual void        SettingRenderingConstants() override;
    virtual void        SettingBufferConstants() override;
    virtual void        SettingVertices() override;

private:
    Texture                     texture;
    int                         x = 0;
	int                         y = 0;
    int                         height = 0;
	int                         width = 0;
    float                       angle = 0.f;

    TextureShader::VertexInput  textureVertices[6] = {};
    TextureShader::VertexInput  invertedTextureVertices[6] = {};
};

