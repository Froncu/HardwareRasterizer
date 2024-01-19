#pragma once

#include "Vertex.hpp"
#include "Matrix.h"
#include "Effect.h"

class Mesh final
{
public:
	enum class FilteringType
	{
		point,
		linear,
		anisotropic,

		COUNT
	};

	~Mesh();

	Mesh(const Mesh& other) = default;
	Mesh(Mesh&&) noexcept = default;
	Mesh& operator=(const Mesh&) = default;
	Mesh& operator=(Mesh&&) noexcept = default;

	Mesh(ID3D11Device* const pDevice, const std::vector<Vertex>& vVertices, const std::vector<uint32_t> vIndices);
	Mesh(ID3D11Device* const pDevice, const std::string& OBJFilePath, bool flipAxisAndWinding = false);

	void Render(ID3D11DeviceContext* const pDeviceContext, const Matrix& viewProjectionMatrix) const;

	void SetFilteringType(FilteringType filteringType);
	void SetTranslator(const Vector3& translator);
	void SetRotorY(float yaw);
	void SetScalar(float scalar);

private:
	HRESULT CreateInputLayout(ID3DX11EffectTechnique* const pEffectTechnique, ID3D11Device* const pDevice, ID3D11InputLayout*& pInputLayout);
	HRESULT CreateVertexBuffer(const std::vector<Vertex>& vVertices, ID3D11Device* const pDevice, ID3D11Buffer*& pVertexBuffer);
	HRESULT CreateIndexBuffer(const std::vector<uint32_t>& vIndices, ID3D11Device* const pDevice, ID3D11Buffer*& pIndexBuffer, uint32_t& numberOfIndices);

	bool ParseOBJ(const std::string& path, bool flipAxisAndWinding, std::vector<Vertex>& vVertices, std::vector<uint32_t>& vIndices);

	Effect m_Effect;
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