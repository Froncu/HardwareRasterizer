#pragma once

#include "DefaultEffect.h"

class VehicleEffect final : public DefaultEffect
{
public:
	enum class FilteringType
	{
		point,
		linear,
		anisotropic,

		COUNT
	};

	VehicleEffect(ID3D11Device* const pDevice);

	VehicleEffect(const VehicleEffect&) = delete;
	VehicleEffect(VehicleEffect&&) noexcept = delete;
	VehicleEffect& operator=(const VehicleEffect&) = delete;
	VehicleEffect& operator=(VehicleEffect&&) noexcept = delete;

	virtual ~VehicleEffect() = default;

	void SetCameraOrigin(const Vector3& position) const;

	void SetNormalTexture(const Texture& texture) const;
	void SetSpecularTexture(const Texture& texture) const;
	void SetGlossTexture(const Texture& texture) const;

	void ToggleFilteringType();
	void SetFilteringType(FilteringType filteringType);

private:
	FilteringType m_FilteringType;
};