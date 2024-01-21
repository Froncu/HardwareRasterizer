#pragma once

#include "DefaultEffect.h"

class FireEffect final : public DefaultEffect
{
public:
	FireEffect(ID3D11Device* const pDevice);

	FireEffect(const FireEffect&) = delete;
	FireEffect(FireEffect&&) noexcept = delete;
	FireEffect& operator=(const FireEffect&) = delete;
	FireEffect& operator=(FireEffect&&) noexcept = delete;

	virtual ~FireEffect() override = default;
};