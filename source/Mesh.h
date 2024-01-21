#pragma once

#include "Vertex.hpp"
#include "Matrix.h"

class DefaultEffect;

class Mesh final
{
public:
	Mesh(const DefaultEffect* const pEffect, ID3D11Device* const pDevice, const std::vector<Vertex>& vVertices, const std::vector<uint32_t> vIndices);
	Mesh(const DefaultEffect* const pEffect, ID3D11Device* const pDevice, const std::string& OBJFilePath, bool flipAxisAndWinding = true);

	Mesh(const Mesh& other);
	Mesh(Mesh&& other) noexcept = delete;
	Mesh& operator=(const Mesh& other);
	Mesh& operator=(Mesh&& other) noexcept = delete;

	~Mesh();

	void Render(ID3D11DeviceContext* const pDeviceContext, const Matrix& viewProjectionMatrix) const;

	void SetTranslator(const Vector3& translator);
	void SetRotorY(float yaw);
	void SetScalar(float scalar);

private:
	HRESULT CreateInputLayout(ID3DX11EffectTechnique* const pEffectTechnique, ID3D11Device* const pDevice, ID3D11InputLayout*& pInputLayout) const;
	HRESULT CreateVertexBuffer(const std::vector<Vertex>& vVertices, ID3D11Device* const pDevice, ID3D11Buffer*& pVertexBuffer);
	HRESULT CreateIndexBuffer(const std::vector<uint32_t>& vIndices, ID3D11Device* const pDevice, ID3D11Buffer*& pIndexBuffer, uint32_t& numberOfIndices);

	bool ParseOBJ(const std::string& path, bool flipAxisAndWinding, std::vector<Vertex>& vVertices, std::vector<uint32_t>& vIndices);

	const DefaultEffect* m_pEffect;
	
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	uint32_t m_NumberOfIndices;

	Matrix m_Translator;
	Matrix m_Rotor;
	Matrix m_Scalar;
	Matrix m_WorldMatrix;
};