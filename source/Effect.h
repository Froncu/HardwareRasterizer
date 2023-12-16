#pragma once

#include "Texture.h"

class Effect final
{
public:
	~Effect();

	Effect(const Effect&) = delete;
	Effect(Effect&&) noexcept = delete;
	Effect& operator=(const Effect&) = delete;
	Effect& operator=(Effect&&) noexcept = delete;

	Effect(ID3D11Device* const pDevice, const std::wstring& effectPath);

	ID3DX11Effect* operator->() const;

	void SetDiffuseTexture(const Texture& texture) const;

private:
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseTexture;
};