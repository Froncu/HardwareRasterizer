#include "pch.h"
#include "Camera.h"

#include "Timer.h"
#include "SDL_keyboard.h"
#include "SDL_mouse.h"

#include <iostream>

#pragma region Constructors/Destructor
Camera::Camera(const Vector3& origin, float fieldOfViewAngle) :
	m_Origin{ origin },
	m_ForwardDirection{ VECTOR3_UNIT_Z },
	m_RightDirection{ VECTOR3_UNIT_X },
	m_UpDirection{ VECTOR3_UNIT_Y },

	m_FieldOfViewAngle{ fieldOfViewAngle },
	m_FieldOfViewValue{ std::tanf(m_FieldOfViewAngle / 2.0f) },

	m_TotalPitch{},
	m_TotalYaw{},

	m_ViewMatrix{},
	m_ProjectionMatrix{},
	m_ViewProjectionMatrix{}
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
void Camera::Update(const Timer& timer)
{
	static constexpr float BASE_MOVEMENT_SPEED{ 15.0f };
	static constexpr float MOUSE_SENSITIVITY{ 0.25f };
	static constexpr float MAX_TOTAL_PITCH{ TO_RADIANS * 90.0f - FLT_EPSILON };
	static constexpr float DEFAULT_FIELD_OF_VIEW_ANGLE{ TO_RADIANS * 45.0f };
	static constexpr float MOUSE_MOVEMENT_ORIGIN_CORRECTOR{ 0.0625f };

	const float deltaTime{ timer.GetElapsed() };
	const float fieldOfViewScalar{ std::min(m_FieldOfViewAngle / DEFAULT_FIELD_OF_VIEW_ANGLE, 1.0f) };

	float movementSpeed{ BASE_MOVEMENT_SPEED };

	//	Keyboard Input
	const uint8_t* pKeyboardState{ SDL_GetKeyboardState(nullptr) };

	if (pKeyboardState[SDL_SCANCODE_LSHIFT])
	{
		movementSpeed *= 3;
	}

	if (pKeyboardState[SDL_SCANCODE_W])
	{
		m_Origin += m_ForwardDirection * deltaTime * movementSpeed;
		UpdateViewMatrix();
	}

	if (pKeyboardState[SDL_SCANCODE_S])
	{
		m_Origin -= m_ForwardDirection * deltaTime * movementSpeed;
		UpdateViewMatrix();
	}

	if (pKeyboardState[SDL_SCANCODE_A])
	{
		m_Origin -= m_RightDirection * deltaTime * movementSpeed;
		UpdateViewMatrix();
	}

	if (pKeyboardState[SDL_SCANCODE_D])
	{
		m_Origin += m_RightDirection * deltaTime * movementSpeed;
		UpdateViewMatrix();
	}

	//	Mouse Input
	int mouseX, mouseY;
	const uint32_t mouseState{ SDL_GetRelativeMouseState(&mouseX, &mouseY) };

	switch (mouseState)
	{
	case SDL_BUTTON(1):
		m_Origin -= m_ForwardDirection * BASE_MOVEMENT_SPEED * float(mouseY) * MOUSE_SENSITIVITY * MOUSE_MOVEMENT_ORIGIN_CORRECTOR;
		m_TotalYaw += TO_RADIANS * fieldOfViewScalar * mouseX * MOUSE_SENSITIVITY;
		UpdateViewMatrix();
		break;

	case SDL_BUTTON(3):
		m_TotalYaw += TO_RADIANS * fieldOfViewScalar * mouseX * MOUSE_SENSITIVITY;
		m_TotalPitch += TO_RADIANS * fieldOfViewScalar * mouseY * MOUSE_SENSITIVITY;
		m_TotalPitch = std::max(-MAX_TOTAL_PITCH, std::min(m_TotalPitch, MAX_TOTAL_PITCH));
		UpdateViewMatrix();
		break;

	case SDL_BUTTON_X2:
		m_Origin -= m_UpDirection * BASE_MOVEMENT_SPEED * float(mouseY) * MOUSE_SENSITIVITY * MOUSE_MOVEMENT_ORIGIN_CORRECTOR;
		UpdateViewMatrix();
		break;
	}
}
#pragma endregion PublicMethods



#pragma region Getters
const Matrix& Camera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

const Matrix& Camera::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

const Matrix& Camera::GetViewProjectionMatrix() const
{
	return m_ViewProjectionMatrix;
}

const Vector3& Camera::GetOrigin() const
{
	return m_Origin;
}

float Camera::GetFieldOfViewValue() const
{
	return m_FieldOfViewValue;
}
#pragma endregion Getters



#pragma region Setters
void Camera::SetOrigin(const Vector3& origin)
{
	m_Origin = origin;
}

void Camera::SetFieldOfViewAngle(float angle)
{
	static constexpr float MAX_FOV_ANGLE{ TO_RADIANS * 180.0f - FLT_EPSILON };

	m_FieldOfViewAngle = std::max(FLT_EPSILON, std::min(angle, MAX_FOV_ANGLE));
	m_FieldOfViewValue = std::tanf(m_FieldOfViewAngle / 2.0f);

	system("CLS");
	std::cout
		<< CONTROLS
		<< "--------\n"
		<< "FIELD OF VIEW ANGLE: " << TO_DEGREES * m_FieldOfViewAngle << " degrees\n"
		<< "--------\n";

	UpdateProjectionMatrix();
}

void Camera::IncrementFieldOfViewAngle(float angleIncrementer)
{
	SetFieldOfViewAngle(m_FieldOfViewAngle + angleIncrementer);
}
#pragma endregion Setters



#pragma region PrivateMethods
void Camera::UpdateViewMatrix()
{
	static constexpr Vector3 WORLD_UP{ VECTOR3_UNIT_Y };

	m_ForwardDirection = Matrix(Matrix::CreateRotorX(m_TotalPitch) * Matrix::CreateRotorY(m_TotalYaw)).TransformVector(VECTOR3_UNIT_Z);
	m_RightDirection = Vector3::Cross(WORLD_UP, m_ForwardDirection).GetNormalized();
	m_UpDirection = Vector3::Cross(m_ForwardDirection, m_RightDirection).GetNormalized();

	m_ViewMatrix = Matrix
	(
		m_RightDirection.GetVector4(),
		m_UpDirection.GetVector4(),
		m_ForwardDirection.GetVector4(),
		m_Origin.GetPoint4()
	).GetInversed();

	UpdateViewProjectionMatrix();
}

void Camera::UpdateProjectionMatrix()
{
	static constexpr float NEAR_PLANE{ 0.1f };
	static constexpr float FAR_PLANE{ 1000.0f };
	static constexpr float DELTA_NEAR_FAR_PLANE{ FAR_PLANE - NEAR_PLANE };

	static const Vector4 Z_AXIS{ 0.0f, 0.0f, FAR_PLANE / DELTA_NEAR_FAR_PLANE, 1.0f };
	static const Vector4 TRANSLATOR{ 0.0f, 0.0f, -FAR_PLANE * NEAR_PLANE / DELTA_NEAR_FAR_PLANE, 0.0f };

	m_ProjectionMatrix = Matrix
	(
		Vector4(1.0f / (ASPECT_RATIO * m_FieldOfViewValue), 0.0f, 0.0f, 0.0f),
		Vector4(0.0f, 1.0f / m_FieldOfViewValue, 0.0f, 0.0f),
		Z_AXIS,
		TRANSLATOR
	);

	UpdateViewProjectionMatrix();
}

void Camera::UpdateViewProjectionMatrix()
{
	m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
}
#pragma endregion PrivateMethods