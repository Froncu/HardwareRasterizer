#include "pch.h"
#include "Renderer.h"

Renderer::Renderer(SDL_Window* pWindow) :
	m_pWindow{ pWindow },
	m_IsInitialized{}
{
	const HRESULT result = InitializeDirectX();
	if (result == S_OK)
	{
		m_IsInitialized = true;
		std::cout << "DirectX is initialized and ready!\n";
	}
	else
	{
		std::cout << "DirectX initialization failed!\n";
	}
}

Renderer::~Renderer()
{
}

void Renderer::Update(const Timer& timer)
{

}

void Renderer::Render() const
{
	if (!m_IsInitialized)
		return;

}

HRESULT Renderer::InitializeDirectX()
{
	return S_FALSE;
}