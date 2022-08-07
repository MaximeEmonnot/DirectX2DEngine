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

	virtual void Update() = 0;
	void Render();
	int GetVertices() const;

protected:
	virtual void SettingShader() = 0;
	virtual void SettingBufferConstants() = 0;
	virtual void SettingRenderingConstants() = 0;
	virtual void SettingVertices() = 0;
	void SettingIndices();
private:
	HRESULT SettingVertexBuffer();
	HRESULT SettingIndexBuffer();

protected:
	std::shared_ptr<BaseShader> pShader;

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	unsigned int vertexBufferByteWidth;
	int nVertices;
	unsigned int stride;
	unsigned int offset;

	void* vertices;
	unsigned long* indices;
};

