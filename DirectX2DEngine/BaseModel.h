#pragma once
#include "DirectX.h"
#include <memory>
#include "BaseShader.h"

class BaseModel
{
public:
	BaseModel() = default;
	virtual ~BaseModel();
	void Initialize();

	void Render();
	int GetVertices() const;

	void SetDepth(float new_depth);
	float GetDepth() const;

	void SetVisibility(bool bValue);
	bool IsVisible() const;

	void SetInverted(bool bValue);
	bool IsInverted() const;

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

	unsigned int vertexBufferByteWidth = 0u;
	int nVertices = 0;
	unsigned int stride = 0u;
	unsigned int offset = 0u;

	void* vertices = nullptr;
	void* inverted_vertices = nullptr;
	unsigned long* indices = nullptr;

	float depth = 0.f;
};

