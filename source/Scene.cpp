#include "pch.h"
#include "Scene.h"

#include "Timer.h"

#pragma region Constructors/Destructor
Scene::Scene(ID3D11Device* const pDevice) :
	m_Camera{ {0.0f, 0.0f, -50.0f} },

	m_VehicleDiffuse{ pDevice, "Resources/Vehicle/VehicleDiffuse.png" },
	m_VehicleNormal{ pDevice, "Resources/Vehicle/VehicleNormal.png" },
	m_VehicleSpecular{ pDevice, "Resources/Vehicle/VehicleSpecular.png" },
	m_VehicleGloss{ pDevice, "Resources/Vehicle/VehicleGloss.png" },
	m_FireDiffuse{ pDevice, "Resources/Fire/FireDiffuse.png" },

	m_VehicleEffect{ pDevice },
	m_FireEffect{ pDevice },
	m_vMeshes
	{
		{ &m_VehicleEffect, pDevice, "Resources/Vehicle/Vehicle.obj" },
		{ &m_FireEffect, pDevice, "Resources/Fire/Fire.obj" }
	}
{
	m_VehicleEffect.SetDiffuseTexture(m_VehicleDiffuse);
	m_VehicleEffect.SetNormalTexture(m_VehicleNormal);
	m_VehicleEffect.SetSpecularTexture(m_VehicleSpecular);
	m_VehicleEffect.SetGlossTexture(m_VehicleGloss);

	m_FireEffect.SetDiffuseTexture(m_FireDiffuse);
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void Scene::Update(const Timer& timer)
{
	m_Camera.Update(timer);

	m_VehicleEffect.SetCameraOrigin(m_Camera.GetOrigin());

	for (Mesh& mesh : m_vMeshes)
		mesh.SetRotorY(TO_RADIANS * 45.0f * timer.GetTotal());
}

void Scene::Render(ID3D11DeviceContext* const pDeviceContext) const
{
	for (const Mesh& mesh : m_vMeshes)
		mesh.Render(pDeviceContext, m_Camera.GetViewProjectionMatrix());
}

void Scene::ToggleFilteringType()
{
	m_VehicleEffect.ToggleFilteringType();
}

Camera& Scene::GetCamera()
{
	return m_Camera;
}
#pragma endregion PublicMethods