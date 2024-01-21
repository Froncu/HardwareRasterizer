#pragma once

#include "ColorRGB.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#include <array>

struct Vertex
{
private:
	static constexpr uint32_t m_DATA_MEMBERS_COUNT{ 4 };

public:
	Vector3 position;

	Vector2 UV;
	Vector3 normal;
	Vector3 tangent;

	static consteval std::array<D3D11_INPUT_ELEMENT_DESC, m_DATA_MEMBERS_COUNT> GetLayout()
	{
		std::array<D3D11_INPUT_ELEMENT_DESC, m_DATA_MEMBERS_COUNT> aVertexDescription{};

		aVertexDescription[0].SemanticName = "SV_POSITION";
		aVertexDescription[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		aVertexDescription[0].AlignedByteOffset = 0;
		aVertexDescription[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		aVertexDescription[1].SemanticName = "TEXCOORD";
		aVertexDescription[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		aVertexDescription[1].AlignedByteOffset = 12;
		aVertexDescription[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		aVertexDescription[2].SemanticName = "NORMAL";
		aVertexDescription[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		aVertexDescription[2].AlignedByteOffset = 20;
		aVertexDescription[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		aVertexDescription[3].SemanticName = "TANGENT";
		aVertexDescription[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		aVertexDescription[3].AlignedByteOffset = 32;
		aVertexDescription[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

		return aVertexDescription;
	}
};