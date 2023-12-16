#include "pch.h"
#include "Mesh.h"

#include <cassert>
#include <fstream>

#pragma region Constructors/Destructor
Mesh::~Mesh()
{
	m_pIndexBuffer->Release();

	m_pVertexBuffer->Release();

	m_pInputLayout->Release();
}

Mesh::Mesh(ID3D11Device* const pDevice, const std::vector<VertexIn>& vVertices, const std::vector<uint32_t> vIndices) :
	m_Effect{ pDevice, L"Resources/PositionColor3D.fx" },
	m_pEffectTechnique{ m_Effect->GetTechniqueByName("DefaultTechnique") },

	m_Translator{ IDENTITY },
	m_Rotor{ IDENTITY },
	m_Scalar{ IDENTITY },
	m_WorldMatrix{ IDENTITY }
{
	assert(SUCCEEDED(CreateInputLayout(m_pEffectTechnique, pDevice, m_pInputLayout)));

	assert(SUCCEEDED(CreateVertexBuffer(vVertices, pDevice, m_pVertexBuffer)));

	assert(SUCCEEDED(CreateIndexBuffer(vIndices, pDevice, m_pIndexBuffer, m_NumberOfIndices)));

	m_Effect.SetDiffuseTexture(Texture(pDevice, "Resources/uv_grid_2.png"));
}

Mesh::Mesh(ID3D11Device* const pDevice, const std::string& OBJFilePath, bool flipAxisAndWinding) :
	m_Effect{ pDevice, L"Resources/PositionColor3D.fx" },
	m_pEffectTechnique{ m_Effect->GetTechniqueByName("DefaultTechnique") },

	m_Translator{ IDENTITY },
	m_Rotor{ IDENTITY },
	m_Scalar{ IDENTITY },
	m_WorldMatrix{ IDENTITY }
{
	std::vector<VertexIn> vVertices{};
	std::vector<uint32_t> vIndices{};
	ParseOBJ(OBJFilePath, flipAxisAndWinding, vVertices, vIndices);

	assert(SUCCEEDED(CreateInputLayout(m_pEffectTechnique, pDevice, m_pInputLayout)));

	assert(SUCCEEDED(CreateVertexBuffer(vVertices, pDevice, m_pVertexBuffer)));

	assert(SUCCEEDED(CreateIndexBuffer(vIndices, pDevice, m_pIndexBuffer, m_NumberOfIndices)));

	m_Effect.SetDiffuseTexture(Texture(pDevice, "Resources/vehicle_diffuse.png"));
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void Mesh::Render(ID3D11DeviceContext* const pDeviceContext, const Matrix& viewProjectionMatrix) const
{
	m_Effect->GetVariableByName("gWorldViewProjection")->AsMatrix()->SetMatrix(Matrix(m_WorldMatrix * viewProjectionMatrix).GetDataPointer());

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

	aVertexDescription[2].SemanticName = "TEXCOORD";
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

bool Mesh::ParseOBJ(const std::string& path, bool flipAxisAndWinding, std::vector<VertexIn>& vVertices, std::vector<uint32_t>& vIndices)
{
	std::ifstream file{ path };
	if (!file)
		return false;

	vVertices.clear();
	vIndices.clear();

	std::string command;
	std::vector<Vector3> vPositions{};
	std::vector<Vector2> vUVs{};
	std::vector<Vector3> vNormals{};

	while (!file.eof())
	{
		file >> command;

		if (command == "v") // Vertex (Position Local)
		{
			float x, y, z;
			file >> x >> y >> z;
			vPositions.emplace_back(x, y, z);
		}
		else if (command == "vt") // Vertex Texture Coordinate (UV)
		{
			float u, v;
			file >> u >> v;
			vUVs.emplace_back(u, 1.0f - v);
		}
		else if (command == "vn") // Vertex Normal
		{
			float x, y, z;
			file >> x >> y >> z;
			vNormals.emplace_back(x, y, z);
		}
		else if (command == "f") // Face (Triangle)
		{
			std::vector<uint32_t> vTemporaryIndices(3);

			for (size_t faceIndex{}; faceIndex < 3; ++faceIndex)
			{
				VertexIn vertex{};

				size_t positionIndex;
				file >> positionIndex;
				vertex.position = vPositions[positionIndex - 1]; // OBJ format uses 1-based arrays, hence -1

				if (file.peek() == '/')
				{
					file.ignore();

					if (file.peek() != '/')
					{
						size_t UVIndex;
						file >> UVIndex;
						vertex.UV = vUVs[UVIndex - 1]; // OBJ format uses 1-based arrays, hence -1
					}

					if (file.peek() == '/')
					{
						file.ignore();

						size_t normalIndex;
						file >> normalIndex;
						//vertexIn.normal = vNormals[normalIndex - 1]; // OBJ format uses 1-based arrays, hence -1

						//vertexIn.normal.Normalize();
					}
				}

				vVertices.push_back(vertex);
				vTemporaryIndices[faceIndex] = static_cast<uint32_t>(vVertices.size() - 1);
			}

			vIndices.push_back(vTemporaryIndices[0]);
			if (!flipAxisAndWinding)
			{
				vIndices.push_back(vTemporaryIndices[1]);
				vIndices.push_back(vTemporaryIndices[2]);
			}
			else
			{
				vIndices.push_back(vTemporaryIndices[2]);
				vIndices.push_back(vTemporaryIndices[1]);
			}
		}

		file.ignore(1000, '\n'); // Read till end of line and ignore all remaining chars
	}

	//// Cheap Tangent Calculation
	//for (size_t index{}; index < vIndices.size(); index += 3)
	//{
	//	const size_t
	//		index0{ vIndices[index] },
	//		index1{ vIndices[index + 1] },
	//		index2{ vIndices[index + 2] };

	//	const Vector2
	//		& v0UV{ vVerticesIn[index0].UV },
	//		& v1UV{ vVerticesIn[index1].UV },
	//		& v2UV{ vVerticesIn[index2].UV };

	//	const Vector2
	//		differenceX{ v1UV.x - v0UV.x, v2UV.x - v0UV.x },
	//		differenceY{ v1UV.y - v0UV.y, v2UV.y - v0UV.y };

	//	const float
	//		cross{ Vector2::Cross(differenceX, differenceY) },
	//		inversedCross{ cross ? (1.0f / cross) : 0.0f };

	//	const Vector3
	//		& v0Position{ vVerticesIn[index0].position },
	//		& v1Position{ vVerticesIn[index1].position },
	//		& v2Position{ vVerticesIn[index2].position },

	//		edge0{ v1Position - v0Position },
	//		edge1{ v2Position - v0Position },
	//		tangent{ (edge0 * differenceY.y - edge1 * differenceY.x) * inversedCross };

	//	vVerticesIn[index0].tangent += tangent;
	//	vVerticesIn[index1].tangent += tangent;
	//	vVerticesIn[index2].tangent += tangent;
	//}

	//// Fix the tangents per vertex now because we accumulated
	//for (VertexIn& vertexIn : vVerticesIn)
	//{
	//	vertexIn.tangent = Vector3::Reject(vertexIn.tangent, vertexIn.normal).GetNormalized();

	//	if (flipAxisAndWinding)
	//	{
	//		vertexIn.position.z *= -1.0f;
	//		vertexIn.normal.z *= -1.0f;
	//		vertexIn.tangent.z *= -1.0f;
	//	}
	//}

	return true;
}
#pragma endregion PrivateMethods