#pragma once

class Effect
{
protected:
	Effect(ID3D11Device* const pDevice, const std::string& effectPath);

	Effect(const Effect&) = delete;
	Effect(Effect&&) noexcept = delete;
	Effect& operator=(const Effect&) = delete;
	Effect& operator=(Effect&&) noexcept = delete;
	
	virtual ~Effect();

	ID3DX11Effect* m_pEffect;

private:
	HRESULT CompileEffect(ID3D11Device* const pDevice, const std::string& effectPath, ID3DX11Effect*& pEffect) const;
};