#pragma once

#include "ColorRGB.h"

struct SDL_Window;
class Scene;

class Renderer final
{
public:
	Renderer(SDL_Window* const pWindow);

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	~Renderer();

	void Render(const Scene& scene);

	ID3D11Device* const GetDevice() const;

private:
	void ClearBuffers(ID3D11DeviceContext* const pDeviceContext, const ColorRGB& color = CORNFLOWER_BLUE) const;

	HRESULT CreateDevice(ID3D11Device*& pDevice, ID3D11DeviceContext*& pDeviceContext) const;
	HRESULT CreateFactory(IDXGIFactory1*& pDXGIFactory) const;
	HRESULT CreateSwapChain(SDL_Window* const pWindow, IDXGIFactory1* const pDXGIFactory, ID3D11Device* const pDevice, IDXGISwapChain*& pSwapChain) const;
	HRESULT CreateDepthStencilBuffer(ID3D11Device* const pDevice, ID3D11Texture2D*& pDepthStencilBuffer) const;
	HRESULT CreateDepthStencilView(ID3D11Device* const pDevice, ID3D11Texture2D* const pDepthStencilBuffer, ID3D11DepthStencilView*& pDepthStencilView) const;
	HRESULT CreateRenderTargetBuffer(IDXGISwapChain* const pSwapChain, ID3D11Resource*& pRenderTargetBuffer) const;
	HRESULT CreateRenderTargetView(ID3D11Device* const pDevice, ID3D11Resource* const pRenderTargetBuffer, ID3D11RenderTargetView*& pRenderTargetView) const;
	void BindAsActiveBuffers(ID3D11DeviceContext* const pDeviceContext, ID3D11DepthStencilView* const pDepthStencilView, ID3D11RenderTargetView*& pRenderTargetView) const;
	void SetViewPort(ID3D11DeviceContext* const pDeviceContext) const;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Resource* m_pRenderTargetBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;
};