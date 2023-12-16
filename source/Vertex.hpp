#pragma once

#include "ColorRGB.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

struct VertexIn
{
	Vector3 position;
	ColorRGB color;

	Vector2 UV;
};

struct VertexOut
{
	Vector4 positionNDC;
	ColorRGB color;

	Vector2 UV;
};