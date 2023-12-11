#include "pch.h"
#include "Effect.h"

#pragma region PublicMethods
ID3DX11Effect* Effect::Load(ID3D11Device* const pDevice, const std::wstring& effectPath)
{
	DWORD shaderFlags{};

#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DX11Effect* pEffect;
	ID3D10Blob* pErrorBlob{};

	if (FAILED(D3DX11CompileEffectFromFile
	(
		effectPath.c_str(),
		nullptr,
		nullptr,
		shaderFlags,
		0,
		pDevice,
		&pEffect,
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

	return pEffect;
}
#pragma region PublicMethods