#pragma once
#include "DirectX.h"
#include <memory>
#include "BaseShader.h"

class BaseModel
{
public:
	BaseModel() = default;
	virtual ~BaseModel() = default;
	void Initialize();

	void Render();
	int GetVertices() const;

	void SetDepth(float new_depth);
	float GetDepth() const;

	void SetVisibility(bool bValue);
	bool IsVisible() const;

	void SetInverted(bool bValue);

protected:
	virtual void SettingShader() = 0;
	virtual void SettingBufferConstants() = 0;
	virtual void SettingRenderingConstants() = 0;
	virtual void SettingVertices() = 0;

	void SettingIndices();
private:
	HRESULT SettingVertexBuffers();
	HRESULT SettingIndexBuffer();

protected:
	std::shared_ptr<BaseShader> pShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pInvertedVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	bool bIsVisible = true;
	bool bIsInverted = false;

	unsigned int vertexBufferByteWidth;
	int nVertices;
	unsigned int stride;
	unsigned int offset;

	void* vertices;
	void* inverted_vertices;
	unsigned long* indices;

	float depth;
};

