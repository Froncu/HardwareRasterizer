#include "pch.h"
#include "Texture.h"

#include "SDL_image.h"
#include "Vector2.h"
#include "ColorRGB.h"
#include "Mathematics.hpp"

#include <cassert>

#pragma region Constructors/Destructor
Texture::Texture(ID3D11Device* const pDevice, const std::string& path)
{
	HRESULT result;

	result = CreateTexture2D(pDevice, path, m_pResource);
	assert(SUCCEEDED(result));

	result = CreateShaderResourceView(pDevice, m_pResource, m_pShaderResourceView);
	assert(SUCCEEDED(result));
}

Texture::~Texture()
{
	m_pShaderResourceView->Release();

	m_pResource->Release();
}
#pragma endregion Constructors/Destructor



#pragma region Getters
ID3D11ShaderResourceView* Texture::GetShaderResourceView() const
{
	return m_pShaderResourceView;
}
#pragma endregion Getters



#pragma region PrivateMethods
HRESULT Texture::CreateTexture2D(ID3D11Device* const pDevice, const std::string& path, ID3D11Texture2D*& pResource)
{
	SDL_Surface* const pSurface{ IMG_Load(path.c_str()) };

	D3D11_TEXTURE2D_DESC description{};
	description.Width = pSurface->w;
	description.Height = pSurface->h;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.CPUAccessFlags = 0;
	description.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = pSurface->pixels;
	initialData.SysMemPitch = static_cast<UINT>(pSurface->pitch);
	initialData.SysMemSlicePitch = static_cast<UINT>(pSurface->h * pSurface->pitch);

	const HRESULT result{ pDevice->CreateTexture2D(&description, &initialData, &pResource) };

	SDL_FreeSurface(pSurface);

	return result;
}

HRESULT Texture::CreateShaderResourceView(ID3D11Device* const pDevice, ID3D11Texture2D* const pResource, ID3D11ShaderResourceView*& pShaderResourceView)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC description{};
	description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	description.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	description.Texture2D.MipLevels = 1;

	return pDevice->CreateShaderResourceView(pResource, &description, &pShaderResourceView);
}
#pragma endregion PrivateMethods