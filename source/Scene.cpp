#include "pch.h"
#include "Scene.h"

#include "Timer.h"

#pragma region Constructors/Destructor
Scene::Scene(ID3D11Device* const pDevice) :
	m_RotateMeshes{ true },
	m_NotRotatingElapsedSeconds{},
	m_RenderFire{ true },

	m_Camera{ {0.0f, 0.0f, -50.0f} },

	m_VehicleEffect{ pDevice },
	m_FireEffect{ pDevice },

	m_VehicleMesh{ &m_VehicleEffect, pDevice, "Resources/vehicle.obj" },
	m_FireMesh{ &m_FireEffect, pDevice, "Resources/fireFX.obj" }
{
	m_VehicleEffect.SetDiffuseTexture({ pDevice, "Resources/vehicle_diffuse.png" });
	m_VehicleEffect.SetNormalTexture({ pDevice, "Resources/vehicle_normal.png" });
	m_VehicleEffect.SetSpecularTexture({ pDevice, "Resources/vehicle_specular.png" });
	m_VehicleEffect.SetGlossTexture({ pDevice, "Resources/vehicle_gloss.png" });

	m_FireEffect.SetDiffuseTexture({ pDevice, "Resources/fireFX_diffuse.png" });
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void Scene::Update(const Timer& timer)
{
	m_Camera.Update(timer);

	m_VehicleEffect.SetCameraOrigin(m_Camera.GetOrigin());

	if (m_RotateMeshes)
	{
		const float yaw{ TO_RADIANS * 45.0f * (timer.GetTotal() - m_NotRotatingElapsedSeconds) };
		m_VehicleMesh.SetRotorY(yaw);
		m_FireMesh.SetRotorY(yaw);
	}
	else
		m_NotRotatingElapsedSeconds += timer.GetElapsed();
}

void Scene::Render(ID3D11DeviceContext* const pDeviceContext) const
{
	m_VehicleMesh.Render(pDeviceContext, m_Camera.GetViewProjectionMatrix());

	if (m_RenderFire)
		m_FireMesh.Render(pDeviceContext, m_Camera.GetViewProjectionMatrix());;
}

void Scene::ToggleFilteringType()
{
	m_VehicleEffect.ToggleFilteringType();
}

void Scene::ToggleRotation()
{
	m_RotateMeshes = !m_RotateMeshes;

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "ROTATE: " << std::boolalpha << m_RotateMeshes << "\n"
		<< "--------\n";
}

void Scene::ToggleNormalMapping()
{
	m_VehicleEffect.ToggleUseNormalTexture();
}

void Scene::ToggleFireRendering()
{
	m_RenderFire = !m_RenderFire;

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "RENDER FIRE: " << std::boolalpha << m_RenderFire << "\n"
		<< "--------\n";
}

Camera& Scene::GetCamera()
{
	return m_Camera;
}
#pragma endregion PublicMethods