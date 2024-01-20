#pragma once

#include "Texture.h"
#include "Matrix.h"

class Effect final
{
public:
	~Effect();

	Effect(const Effect&) = delete;
	Effect(Effect&&) noexcept = delete;
	Effect& operator=(const Effect&) = delete;
	Effect& operator=(Effect&&) noexcept = delete;

	Effect(ID3D11Device* const pDevice);

	ID3DX11Effect* operator->() const;

	void SetWorldMatrix(const Matrix& matrix) const;
	void SetViewProjectionMatrix(const Matrix& matrix) const;

	void SetCameraOrigin(const Vector3& position) const;

	void SetDiffuseTexture(const Texture& texture) const;
	void SetNormalTexture(const Texture& texture) const;
	void SetSpecularTexture(const Texture& texture) const;
	void SetGlossinessTexture(const Texture& texture) const;

private:
	ID3DX11Effect* m_pEffect;
};