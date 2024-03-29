#include "pch.h"
#include "VehicleEffect.h"

#include "Vertex.hpp"
#include "Vector3.h"

#include <cassert>

#pragma region Constructors/Destructor
VehicleEffect::VehicleEffect(ID3D11Device* const pDevice) :
	DefaultEffect(pDevice, "Resources/Vehicle.fx"),

	m_FilteringType{ FilteringType::point }
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void VehicleEffect::SetCameraOrigin(const Vector3& position) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_CameraOrigin")->AsVector()->SetFloatVector(&position.x) };
	assert(SUCCEEDED(result));
}

void VehicleEffect::SetNormalTexture(const Texture& texture) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_NormalTexture")->AsShaderResource()->SetResource(texture.GetShaderResourceView()) };
	assert(SUCCEEDED(result));
}

void VehicleEffect::SetSpecularTexture(const Texture& texture) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_SpecularTexture")->AsShaderResource()->SetResource(texture.GetShaderResourceView()) };
	assert(SUCCEEDED(result));
}

void VehicleEffect::SetGlossTexture(const Texture& texture) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_GlossTexture")->AsShaderResource()->SetResource(texture.GetShaderResourceView()) };
	assert(SUCCEEDED(result));
}

void VehicleEffect::ToggleFilteringType()
{
	const uint32_t currentType{ static_cast<uint32_t>(m_FilteringType) };
	const uint32_t typesCount{ static_cast<uint32_t>(FilteringType::COUNT) };
	const uint32_t nextType{ (currentType + 1) % typesCount };

	SetFilteringType(static_cast<FilteringType>(nextType));
}

void VehicleEffect::SetFilteringType(FilteringType filteringType)
{
	m_FilteringType = filteringType;

	std::string variableName, filteringTypeName;
	switch (m_FilteringType)
	{
	case FilteringType::point:
		variableName = "g_PointSamplerState";
		filteringTypeName = "Point";
		break;

	case FilteringType::linear:
		variableName = "g_LinearSamplerState";
		filteringTypeName = "Linear";
		break;

	case FilteringType::anisotropic:
		variableName = "g_AnisotropicSamplerState";
		filteringTypeName = "Anisotropic";
		break;
	}

	ID3D11SamplerState* pSamplerState;

	HRESULT result;

	result = m_pEffect->GetVariableByName(variableName.c_str())->AsSampler()->GetSampler(NULL, &pSamplerState);
	assert(SUCCEEDED(result));

	result = m_pEffect->GetVariableByName("g_ActiveSamplerState")->AsSampler()->SetSampler(NULL, pSamplerState);
	assert(SUCCEEDED(result));

	pSamplerState->Release();

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "FILTERING TYPE: " << filteringTypeName << "\n"
		<< "--------\n";
}

void VehicleEffect::ToggleUseNormalTexture()
{
	HRESULT result;

	bool useNormalTexture;
	result = m_pEffect->GetVariableByName("g_UseNormalTexture")->AsScalar()->GetBool(&useNormalTexture);
	assert(SUCCEEDED(result));

	result = m_pEffect->GetVariableByName("g_UseNormalTexture")->AsScalar()->SetBool(!useNormalTexture);
	assert(SUCCEEDED(result));

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "USE NORMAL TEXTURE: " << std::boolalpha << !useNormalTexture << "\n"
		<< "--------\n";
}
#pragma endregion PublicMethods