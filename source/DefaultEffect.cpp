#include "pch.h"
#include "DefaultEffect.h"

#include "Vector3.h"

#include <cassert>

#pragma region Constructors/Destructor
DefaultEffect::DefaultEffect(ID3D11Device* const pDevice, const std::string& effectPath) :
	Effect(pDevice, effectPath)
{
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void DefaultEffect::SetWorldMatrix(const Matrix& matrix) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_WorldMatrix")->AsMatrix()->SetMatrix(matrix.GetDataPointer()) };
	assert(SUCCEEDED(result));
}

void DefaultEffect::SetViewProjectionMatrix(const Matrix& matrix) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_ViewProjectionMatrix")->AsMatrix()->SetMatrix(matrix.GetDataPointer()) };
	assert(SUCCEEDED(result));
}

void DefaultEffect::SetDiffuseTexture(const Texture& texture) const
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_DiffuseTexture")->AsShaderResource()->SetResource(texture.GetShaderResourceView()) };
	assert(SUCCEEDED(result));
}

void DefaultEffect::SetLightDirection(const Vector3& direction)
{
	const HRESULT result{ m_pEffect->GetVariableByName("g_LightDirection")->AsVector()->SetFloatVector(&direction.x) };
	assert(SUCCEEDED(result));
}

ID3DX11EffectTechnique* const DefaultEffect::GetDefaultTechnique() const
{
	return m_pEffect->GetTechniqueByName("DefaultTechnique");
}

ID3DX11EffectPass* const DefaultEffect::GetDefaultPass() const
{
	return GetDefaultTechnique()->GetPassByName("DefaultPass");
}
#pragma region PublicMethods