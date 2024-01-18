#include "pch.h"
#include "Renderer.h"

#pragma region Constructors/Destructors
Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow{ pWindow },
	m_IsInitialized{ SUCCEEDED(InitializeDirectX()) },

	m_Mesh{ m_pDevice, "Resources/vehicle.obj" }
{
	if (!m_IsInitialized)
		std::cout << "DirectX initialization failed!\n";
}

Renderer::~Renderer()
{
	m_pRenderTargetView->Release();

	m_pRenderTargetBuffer->Release();

	m_pDepthStencilView->Release();

	m_pDepthStencilBuffer->Release();

	m_pSwapChain->Release();

	m_pDeviceContext->ClearState();
	m_pDeviceContext->Flush();
	m_pDeviceContext->Release();

	m_pDevice->Release();
}
#pragma endregion Constructors/Destructors



#pragma region PublicMethods
void Renderer::Update(const Timer& timer)
{
	m_Mesh.SetRotorY(90.0f * TO_RADIANS * timer.GetTotal());
}

void Renderer::Render(const Matrix& cameraMatrix)
{
	if (!m_IsInitialized)
		return;

	ClearBuffers();

	m_Mesh.Render(m_pDeviceContext, cameraMatrix);

	m_pSwapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);

	BindAsActiveBuffers(m_pDeviceContext, m_pDepthStencilView, m_pRenderTargetView);
}

void Renderer::ToggleFliteringType()
{
	const uint32_t
		currentType{ static_cast<uint32_t>(m_FilteringType) },
		typesCount{ static_cast<uint32_t>(Mesh::FilteringType::COUNT) },
		nextType{ (currentType + 1) % typesCount };

	m_FilteringType = static_cast<Mesh::FilteringType>(nextType);

	m_Mesh.SetFilteringType(m_FilteringType);

	std::string filteringType;
	switch (m_FilteringType)
	{
	case Mesh::FilteringType::point:
		filteringType = "Point";
		break;

	case Mesh::FilteringType::linear:
		filteringType = "Linear";
		break;

	case Mesh::FilteringType::anisotropic:
		filteringType = "Anisotropic";
		break;
	}

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "FILTERING TYPE: " << filteringType << "\n"
		<< "--------\n";
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void Renderer::ClearBuffers(const ColorRGB& color) const
{
	const FLOAT aColor[]{ static_cast<FLOAT>(color.red), static_cast<FLOAT>(color.green), static_cast<FLOAT>(color.blue), static_cast<FLOAT>(1.0f) };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, aColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

HRESULT Renderer::InitializeDirectX()
{
	HRESULT result;

	result = CreateDevice(m_pDevice, m_pDeviceContext);
	if (FAILED(result))
		return result;

	IDXGIFactory1* pDXGIFactory;
	result = CreateFactory(pDXGIFactory);
	if (FAILED(result))
		return result;

	result = CreateSwapChain(m_pWindow, pDXGIFactory, m_pDevice, m_pSwapChain);
	if (FAILED(result))
		return result;

	pDXGIFactory->Release();

	result = CreateDepthStencilBuffer(m_pDevice, m_pDepthStencilBuffer);
	if (FAILED(result))
		return result;

	result = CreateDepthStencilView(m_pDevice, m_pDepthStencilBuffer, m_pDepthStencilView);
	if (FAILED(result))
		return result;

	result = CreateRenderTargetBuffer(m_pSwapChain, m_pRenderTargetBuffer);
	if (FAILED(result))
		return result;

	result = CreateRenderTargetView(m_pDevice, m_pRenderTargetBuffer, m_pRenderTargetView);
	if (FAILED(result))
		return result;

	BindAsActiveBuffers(m_pDeviceContext, m_pDepthStencilView, m_pRenderTargetView);

	SetViewPort(m_pDeviceContext);

	return result;
}

HRESULT Renderer::CreateDevice(ID3D11Device*& pDevice, ID3D11DeviceContext*& pDeviceContext) const
{
	const D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_1 };
	uint32_t createDeviceFlags{};

#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	return D3D11CreateDevice
	(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION,
		&pDevice, nullptr, &pDeviceContext
	);
}

HRESULT Renderer::CreateFactory(IDXGIFactory1*& pDXGIFactory) const
{
	return CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&pDXGIFactory));
}

HRESULT Renderer::CreateSwapChain(SDL_Window* const pWindow, IDXGIFactory1* const pDXGIFactory, ID3D11Device* const pDevice, IDXGISwapChain*& pSwapChain) const
{
	DXGI_SWAP_CHAIN_DESC swapChainDescription{};
	swapChainDescription.BufferDesc.Width = WINDOW_WIDTH;
	swapChainDescription.BufferDesc.Height = WINDOW_HEIGHT;
	swapChainDescription.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 2;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	SDL_SysWMinfo sysWMinfo{};
	SDL_GetVersion(&sysWMinfo.version);
	SDL_GetWindowWMInfo(pWindow, &sysWMinfo);
	swapChainDescription.OutputWindow = sysWMinfo.info.win.window;
	return pDXGIFactory->CreateSwapChain(pDevice, &swapChainDescription, &pSwapChain);
}

HRESULT Renderer::CreateDepthStencilBuffer(ID3D11Device* const pDevice, ID3D11Texture2D*& pDepthStencilBuffer) const
{
	D3D11_TEXTURE2D_DESC depthStencilDescription{};
	depthStencilDescription.Width = WINDOW_WIDTH;
	depthStencilDescription.Height = WINDOW_HEIGHT;
	depthStencilDescription.MipLevels = 1;
	depthStencilDescription.ArraySize = 1;
	depthStencilDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDescription.SampleDesc.Count = 1;
	depthStencilDescription.SampleDesc.Quality = 0;
	depthStencilDescription.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDescription.CPUAccessFlags = 0;
	depthStencilDescription.MiscFlags = 0;

	return pDevice->CreateTexture2D(&depthStencilDescription, nullptr, &pDepthStencilBuffer);
}

HRESULT Renderer::CreateDepthStencilView(ID3D11Device* const pDevice, ID3D11Texture2D* const pDepthStencilBuffer, ID3D11DepthStencilView*& pDepthStencilView) const
{
	D3D11_TEXTURE2D_DESC depthStencilDescription;
	pDepthStencilBuffer->GetDesc(&depthStencilDescription);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescription{};
	depthStencilViewDescription.Format = depthStencilDescription.Format;
	depthStencilViewDescription.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDescription.Texture2D.MipSlice = 0;

	return pDevice->CreateDepthStencilView(pDepthStencilBuffer, &depthStencilViewDescription, &pDepthStencilView);
}

HRESULT Renderer::CreateRenderTargetBuffer(IDXGISwapChain* const pSwapChain, ID3D11Resource*& pRenderTargetBuffer) const
{
	return pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTargetBuffer));
}

HRESULT Renderer::CreateRenderTargetView(ID3D11Device* const pDevice, ID3D11Resource* const pRenderTargetBuffer, ID3D11RenderTargetView*& pRenderTargetView) const
{
	return pDevice->CreateRenderTargetView(pRenderTargetBuffer, nullptr, &pRenderTargetView);
}

void Renderer::BindAsActiveBuffers(ID3D11DeviceContext* const pDeviceContext, ID3D11DepthStencilView* const pDepthStencilView, ID3D11RenderTargetView*& pRenderTargetView) const
{
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
}

void Renderer::SetViewPort(ID3D11DeviceContext* const pDeviceContext) const
{
	D3D11_VIEWPORT viewport{};
	viewport.Width = static_cast<FLOAT>(WINDOW_WIDTH);
	viewport.Height = static_cast<FLOAT>(WINDOW_HEIGHT);
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1, &viewport);
}
#pragma endregion PrivateMethods