#pragma once

#include <string>

#include "SDL_stdinc.h"

struct ColorRGB;
struct Vector2;
struct SDL_Surface;
class Texture final
{
public:
	~Texture();

	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept = delete;
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept = delete;

	Texture(ID3D11Device* const pDevice, const std::string& path);

	ID3D11ShaderResourceView* GetShaderResourceView() const;

private:
	HRESULT CreateTexture2D(ID3D11Device* const pDevice, const std::string& path, ID3D11Texture2D*& pResource);
	HRESULT CreateShaderResourceView(ID3D11Device* const pDevice, ID3D11Texture2D* const pResource, ID3D11ShaderResourceView*& pShaderResourceView);

	ID3D11Texture2D* m_pResource;
	ID3D11ShaderResourceView* m_pShaderResourceView;
};