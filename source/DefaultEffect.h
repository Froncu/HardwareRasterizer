#pragma once

#include "Effect.h"
#include "Matrix.h"
#include "Texture.h"

class DefaultEffect : public Effect
{
public:
	DefaultEffect(ID3D11Device* const pDevice, const std::string& effectPath);

	DefaultEffect(const DefaultEffect&) = delete;
	DefaultEffect(DefaultEffect&&) noexcept = delete;
	DefaultEffect& operator=(const DefaultEffect&) = delete;
	DefaultEffect& operator=(DefaultEffect&&) noexcept = delete;

	virtual ~DefaultEffect() override = default;

	void SetWorldMatrix(const Matrix& matrix) const;
	void SetViewProjectionMatrix(const Matrix& matrix) const;
	void SetDiffuseTexture(const Texture& texture) const;
	void SetLightDirection(const Vector3& direction);

	ID3DX11EffectTechnique* const GetDefaultTechnique() const;
	ID3DX11EffectPass* const GetDefaultPass() const;
};