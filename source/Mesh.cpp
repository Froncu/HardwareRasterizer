#include "pch.h"
#include "Mesh.h"

#include "Effect.h"

#include <cassert>

#pragma region Constructors/Destructor
Mesh::~Mesh()
{
	m_pIndexBuffer->Release();

	m_pVertexBuffer->Release();

	m_pInputLayout->Release();

	m_pEffect->Release();
}

Mesh::Mesh(ID3D11Device* const pDevice, const std::vector<VertexIn>& vVertices, const std::vector<uint32_t> vIndices) :
	m_pEffect{ Effect::Load(pDevice, L"Resources/PositionColor3D.fx") },
	m_pEffectTechnique{ m_pEffect->GetTechniqueByName("DefaultTechnique") },

	m_Translator{ IDENTITY },
	m_Rotor{ IDENTITY },
	m_Scalar{ IDENTITY },
	m_WorldMatrix{ IDENTITY }
{
	assert(SUCCEEDED(CreateInputLayout(m_pEffectTechnique, pDevice, m_pInputLayout)));

	assert(SUCCEEDED(CreateVertexBuffer(vVertices, pDevice, m_pVertexBuffer)));

	assert(SUCCEEDED(CreateIndexBuffer(vIndices, pDevice, m_pIndexBuffer, m_NumberOfIndices)));
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void Mesh::Render(ID3D11DeviceContext* const pDeviceContext, const Matrix& viewProjectionMatrix) const
{
	m_pEffect->GetVariableByName("gWorldViewProjection")->AsMatrix()->SetMatrix(Matrix(m_WorldMatrix * viewProjectionMatrix).GetDataPointer());

	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT
		stride{ sizeof(VertexIn) },
		offset{};
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DX11_TECHNIQUE_DESC techniqueDescription{};
	m_pEffectTechnique->GetDesc(&techniqueDescription);
	for (UINT pass{}; pass < techniqueDescription.Passes; ++pass)
	{
		m_pEffectTechnique->GetPassByIndex(pass)->Apply(0, pDeviceContext);
		pDeviceContext->DrawIndexed(m_NumberOfIndices, 0, 0);
	}
}

void Mesh::SetTranslator(const Vector3& translator)
{
	m_Translator = Matrix::CreateTranslator(translator);
	m_WorldMatrix = m_Scalar * m_Rotor * m_Translator;
}

void Mesh::SetRotorY(float yaw)
{
	m_Rotor = Matrix::CreateRotorY(yaw);
	m_WorldMatrix = m_Scalar * m_Rotor * m_Translator;
}

void Mesh::SetScalar(float scalar)
{
	m_Scalar = Matrix::CreateScalar(scalar);
	m_WorldMatrix = m_Scalar * m_Rotor * m_Translator;
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
HRESULT Mesh::CreateInputLayout(ID3DX11EffectTechnique* const pEffectTechnique, ID3D11Device* const pDevice, ID3D11InputLayout*& pInputLayout)
{
	static constexpr uint32_t NUMBER_OF_ELEMENTS{ 3 };
	D3D11_INPUT_ELEMENT_DESC aVertexDescription[NUMBER_OF_ELEMENTS]{};

	aVertexDescription[0].SemanticName = "POSITION";
	aVertexDescription[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	aVertexDescription[0].AlignedByteOffset = 0;
	aVertexDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	aVertexDescription[1].SemanticName = "COLOR";
	aVertexDescription[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	aVertexDescription[1].AlignedByteOffset = 12;
	aVertexDescription[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	aVertexDescription[2].SemanticName = "UV";
	aVertexDescription[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	aVertexDescription[2].AlignedByteOffset = 24;
	aVertexDescription[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	D3DX11_PASS_DESC passDescription{};
	pEffectTechnique->GetPassByIndex(0)->GetDesc(&passDescription);

	return pDevice->CreateInputLayout
	(
		aVertexDescription,
		NUMBER_OF_ELEMENTS,
		passDescription.pIAInputSignature,
		passDescription.IAInputSignatureSize,
		&pInputLayout
	);
}

HRESULT Mesh::CreateVertexBuffer(const std::vector<VertexIn>& vVerticesIn, ID3D11Device* const pDevice, ID3D11Buffer*& pVertexBuffer)
{
	D3D11_BUFFER_DESC bufferDescription{};
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(VertexIn) * static_cast<uint32_t>(vVerticesIn.size());
	bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = vVerticesIn.data();

	return pDevice->CreateBuffer(&bufferDescription, &initialData, &pVertexBuffer);
}

HRESULT Mesh::CreateIndexBuffer(const std::vector<uint32_t>& vIndices, ID3D11Device* const pDevice, ID3D11Buffer*& pIndexBuffer, uint32_t& numberOfIndices)
{
	numberOfIndices = static_cast<uint32_t>(vIndices.size());

	D3D11_BUFFER_DESC bufferDescription{};
	bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDescription.ByteWidth = sizeof(uint32_t) * numberOfIndices;
	bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDescription.CPUAccessFlags = 0;
	bufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = vIndices.data();

	return pDevice->CreateBuffer(&bufferDescription, &initialData, &pIndexBuffer);
}
#pragma endregion PrivateMethods