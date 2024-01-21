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
	void ToggleRotation();
	void ToggleNormalMapping();
	void ToggleFireRendering();

	Camera& GetCamera();

private:
	bool m_RotateMeshes;
	float m_NotRotatingElapsedSeconds;
	bool m_RenderFire;

	Camera m_Camera;

	VehicleEffect m_VehicleEffect;
	FireEffect m_FireEffect;

	Mesh m_VehicleMesh;
	Mesh m_FireMesh;
};