#pragma once

struct SDL_Window;
struct SDL_Surface;

class Renderer final
{
public:
	Renderer(SDL_Window* pWindow);
	~Renderer();

	Renderer(const Renderer&) = delete;
	Renderer(Renderer&&) noexcept = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&&) noexcept = delete;

	void Update(const Timer& timer);
	void Render() const;

private:
	HRESULT InitializeDirectX();

	SDL_Window* m_pWindow;
	bool m_IsInitialized;
};