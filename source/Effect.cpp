#include "pch.h"
#include "Effect.h"

#pragma region Constructors/Destructor
Effect::Effect(ID3D11Device* const pDevice, const std::wstring& effectPath) :
	m_pDiffuseTexture{}
{
	DWORD shaderFlags{};

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* pErrorBlob{};

	if (FAILED(D3DX11CompileEffectFromFile
	(
		effectPath.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&m_pEffect,
		&pErrorBlob
	)))
	{
		std::wstringstream stringStream;

		if (pErrorBlob)
		{
			const char* pErrorsString{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };
			for (size_t index{}; index < pErrorBlob->GetBufferSize(); ++index)
				stringStream << pErrorsString[index];
			pErrorBlob->Release();
			pErrorBlob = nullptr;

			OutputDebugStringW(stringStream.str().c_str());
		}
		else
			stringStream << L"Effect.cpp -> LoadEffect() FAILED [EFFECT PATH: " << effectPath << L"]\n";

		std::wcout << stringStream.str() << std::endl;
	}
	else
		m_pDiffuseTexture = m_pEffect->GetVariableByName("g_DiffuseTexture")->AsShaderResource();
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
void Effect::SetDiffuseTexture(const Texture& texture) const
{
	if (m_pDiffuseTexture)
		m_pDiffuseTexture->SetResource(texture.GetShaderResourceView());
}
#pragma region PublicMethods