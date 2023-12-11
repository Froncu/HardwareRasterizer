#pragma once

class Effect final
{
public:
	~Effect() = delete;

	Effect(const Effect&) = delete;
	Effect(Effect&&) noexcept = delete;
	Effect& operator=(const Effect&) = delete;
	Effect& operator=(Effect&&) noexcept = delete;

	Effect() = delete;

	static ID3DX11Effect* Load(ID3D11Device* const pDevice, const std::wstring& effectPath);
};