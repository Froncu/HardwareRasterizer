#pragma once

#include "ColorRGB.h"
#include "Mesh.h"

class Timer;
struct SDL_Window;
struct SDL_Surface;

class Renderer final
{
public:
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	Renderer(SDL_Window* pWindow);

	void Update(const Timer& timer);
	void Render() const;

private:
	void ClearBuffers(const ColorRGB& color = DARK_GRAY) const;

	HRESULT InitializeDirectX();
	HRESULT CreateDevice(ID3D11Device*& pDevice, ID3D11DeviceContext*& pDeviceContext);
	HRESULT CreateFactory(IDXGIFactory1*& pDXGIFactory);
	HRESULT CreateSwapChain(SDL_Window* const pWindow, IDXGIFactory1* const pDXGIFactory, ID3D11Device* const pDevice, IDXGISwapChain*& pSwapChain);
	HRESULT CreateDepthStencilBuffer(ID3D11Device* const pDevice, ID3D11Texture2D*& pDepthStencilBuffer);
	HRESULT CreateDepthStencilView(ID3D11Device* const pDevice, ID3D11Texture2D* const pDepthStencilBuffer, ID3D11DepthStencilView*& pDepthStencilView);
	HRESULT CreateRenderTargetBuffer(IDXGISwapChain* const pSwapChain, ID3D11Resource*& pRenderTargetBuffer);
	HRESULT CreateRenderTargetView(ID3D11Device* const pDevice, ID3D11Resource* const pRenderTargetBuffer, ID3D11RenderTargetView*& pRenderTargetView);
	void BindAsActiveBuffers(ID3D11DeviceContext* const pDeviceContext, ID3D11DepthStencilView* const pDepthStencilView, ID3D11RenderTargetView*& pRenderTargetView);
	void SetViewPort(ID3D11DeviceContext* const pDeviceContext);

	SDL_Window* m_pWindow;
	bool m_IsInitialized;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11Resource* m_pRenderTargetBuffer;
	ID3D11RenderTargetView* m_pRenderTargetView;

	Mesh m_Mesh;
};