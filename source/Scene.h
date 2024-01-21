#pragma once

#include "Camera.h"
#include "Texture.h"
#include "VehicleEffect.h"
#include "FireEffect.h"
#include "Mesh.h"

class Scene final
{
public:
	Scene(ID3D11Device* const pDevice);

	Scene(const Scene&) = delete;
	Scene(Scene&&) noexcept = delete;
	Scene& operator=(const Scene&) = delete;
	Scene& operator=(Scene&&) noexcept = delete;

	virtual ~Scene() = default;

	void Update(const Timer& timer);
	void Render(ID3D11DeviceContext* const pDeviceContext) const;
	void ToggleFilteringType();

	Camera& GetCamera();

private:
	Camera m_Camera;

	Texture m_VehicleDiffuse;
	Texture m_VehicleNormal;
	Texture m_VehicleSpecular;
	Texture m_VehicleGloss;
	Texture m_FireDiffuse;

	VehicleEffect m_VehicleEffect;
	FireEffect m_FireEffect;
	std::vector<Mesh> m_vMeshes;
};