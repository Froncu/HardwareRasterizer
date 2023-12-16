#pragma once

#include "Vertex.hpp"
#include "Matrix.h"

class Mesh final
{
public:
	~Mesh();

	Mesh(const Mesh& other) = default;
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	Mesh(ID3D11Device* const pDevice, const std::vector<VertexIn>& vVertices, const std::vector<uint32_t> vIndices);

	void Render(ID3D11DeviceContext* const pDeviceContext, const Matrix& viewProjectionMatrix) const;

	void SetTranslator(const Vector3& translator);
	void SetRotorY(float yaw);
	void SetScalar(float scalar);

private:
	HRESULT CreateInputLayout(ID3DX11EffectTechnique* const pEffectTechnique, ID3D11Device* const pDevice, ID3D11InputLayout*& pInputLayout);
	HRESULT CreateVertexBuffer(const std::vector<VertexIn>& vVerticesIn, ID3D11Device* const pDevice, ID3D11Buffer*& pVertexBuffer);
	HRESULT CreateIndexBuffer(const std::vector<uint32_t>& vIndices, ID3D11Device* const pDevice, ID3D11Buffer*& pIndexBuffer, uint32_t& numberOfIndices);

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pEffectTechnique;

	ID3DX11EffectVariable* m_pWorldViewProjectionMatrix;

	ID3D11InputLayout* m_pInputLayout;
	
	ID3D11Buffer
		* m_pVertexBuffer,
		* m_pIndexBuffer;

	uint32_t m_NumberOfIndices;

	Matrix
		m_Translator,
		m_Rotor,
		m_Scalar,
		m_WorldMatrix,
		m_ViewProjection;
};