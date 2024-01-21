#pragma once

#include "Mathematics.hpp"
#include "Vector3.h"
#include "Matrix.h"

class Timer;

class Camera final
{
public:
	Camera(const Vector3& origin = Vector3(0.0f, 0.0f, 0.0f), float fieldOfViewAngle = TO_RADIANS * 45.0f);

	Camera(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(const Camera&) = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	~Camera() = default;

	void Update(const Timer& timer);

	const Matrix& GetViewMatrix() const;
	const Matrix& GetProjectionMatrix() const;
	const Matrix& GetViewProjectionMatrix() const;
	const Vector3& GetOrigin() const;
	float GetFieldOfViewValue() const;

	void SetOrigin(const Vector3& origin);
	void SetFieldOfViewAngle(float angle);
	void IncrementFieldOfViewAngle(float angleIncrementer);

private:
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void UpdateViewProjectionMatrix();

	Vector3 m_Origin;
	Vector3 m_ForwardDirection;
	Vector3 m_RightDirection;
	Vector3 m_UpDirection;

	float m_FieldOfViewAngle;
	float m_FieldOfViewValue;

	float m_TotalPitch;
	float m_TotalYaw;

	Matrix m_ViewMatrix;
	Matrix m_ProjectionMatrix;
	Matrix m_ViewProjectionMatrix;
};