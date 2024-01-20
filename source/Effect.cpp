#include "pch.h"
#include "Effect.h"

#include "Vector3.h"

#pragma region Constructors/Destructor
Effect::Effect(ID3D11Device* const pDevice)
{
	const std::wstring effectPath{ L"Resources/Effect.fx" };

	DWORD shaderFlags{};

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* pErrorBlob{};
	if (SUCCEEDED(D3DX11CompileEffectFromFile
	(
		effectPath.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		NULL,
		pDevice,
		&m_pEffect,
		&pErrorBlob
	)))
		return;

	std::wstringstream stringStream;
	if (pErrorBlob)
	{
		const char* pErrorsString{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };
		for (size_t index{}; index < pErrorBlob->GetBufferSize(); ++index)
			stringStream << pErrorsString[index];
		pErrorBlob->Release();
		pErrorBlob = nullptr;
	}
	else
		stringStream << L"Effect.cpp -> LoadEffect() FAILED [EFFECT PATH: " << effectPath << L"]\n";

	OutputDebugStringW(stringStream.str().c_str());
}

Effect::~Effect()
{
	m_pEffect->Release();
}
#pragma endregion Constructors/Destructor



#pragma region Operators
ID3DX11Effect* Effect::operator->() const
{
	return m_pEffect;
}
#pragma endregion Operators



#pragma region PublicMethods
void Effect::SetWorldMatrix(const Matrix& matrix) const
{
	m_pEffect->GetVariableByName("g_WorldMatrix")->AsMatrix()->SetMatrix(matrix.GetDataPointer());
}

void Effect::SetViewProjectionMatrix(const Matrix& matrix) const
{
	m_pEffect->GetVariableByName("g_ViewProjectionMatrix")->AsMatrix()->SetMatrix(matrix.GetDataPointer());
}

void Effect::SetCameraOrigin(const Vector3& position) const
{
	m_pEffect->GetVariableByName("g_CameraPosition")->SetRawValue(&position, 0, sizeof(Vector3));
}

void Effect::SetDiffuseTexture(const Texture& texture) const
{
	ID3DX11EffectShaderResourceVariable* const pDiffuseTexture{ m_pEffect->GetVariableByName("g_DiffuseTexture")->AsShaderResource() };
	pDiffuseTexture->SetResource(texture.GetShaderResourceView());
}

void Effect::SetNormalTexture(const Texture& texture) const
{
	ID3DX11EffectShaderResourceVariable* const pNormalTexture{ m_pEffect->GetVariableByName("g_NormalTexture")->AsShaderResource() };
	pNormalTexture->SetResource(texture.GetShaderResourceView());
}

void Effect::SetSpecularTexture(const Texture& texture) const
{
	ID3DX11EffectShaderResourceVariable* const pSpecularTexture{ m_pEffect->GetVariableByName("g_SpecularTexture")->AsShaderResource() };
	pSpecularTexture->SetResource(texture.GetShaderResourceView());
}

void Effect::SetGlossinessTexture(const Texture& texture) const
{
	ID3DX11EffectShaderResourceVariable* const pGlossinessTexture{ m_pEffect->GetVariableByName("g_GlossinessTexture")->AsShaderResource() };
	pGlossinessTexture->SetResource(texture.GetShaderResourceView());
}
#pragma region PublicMethods