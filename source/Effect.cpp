#include "pch.h"
#include "Effect.h"

#include <cassert>

#pragma region Constructors/Destructor
Effect::Effect(ID3D11Device* const pDevice, const std::string& effectPath)
{
	assert(SUCCEEDED(CompileEffect(pDevice, effectPath, m_pEffect)));
}

Effect::~Effect()
{
	m_pEffect->Release();
}
#pragma endregion Constructors/Destructor



#pragma region PrivateMethods
HRESULT Effect::CompileEffect(ID3D11Device* const pDevice, const std::string& effectPath, ID3DX11Effect*& pEffect) const
{
	DWORD shaderFlags{};

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* pErrorBlob{};
	const HRESULT result
	{
		D3DX11CompileEffectFromFile(
		std::wstring(effectPath.begin(), effectPath.end()).c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		NULL,
		pDevice,
		&pEffect,
		&pErrorBlob)
	};

	if (FAILED(result))
	{
		std::stringstream stringStream{};
		if (pErrorBlob)
		{
			const char* pErrorsString{ static_cast<char*>(pErrorBlob->GetBufferPointer()) };
			for (size_t index{}; index < pErrorBlob->GetBufferSize(); ++index)
				stringStream << pErrorsString[index];

			pErrorBlob->Release();
		}
		else
			stringStream << "Effect.cpp -> CompileEffect() FAILED [EFFECT PATH: " << effectPath << "]\n";

		OutputDebugStringA(stringStream.str().c_str());
	}

	return result;
}
#pragma endregion PrivateMethods