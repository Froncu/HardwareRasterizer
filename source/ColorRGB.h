#pragma once

struct ColorRGB
{
public:
	ColorRGB operator*(const ColorRGB& color) const;
	ColorRGB operator*(float scalar) const;
	ColorRGB operator/(const ColorRGB& color) const;
	ColorRGB operator/(float scalar) const;
	ColorRGB operator+(const ColorRGB& color) const;
	ColorRGB operator-(const ColorRGB& color) const;

	const ColorRGB& operator*=(const ColorRGB& color);
	const ColorRGB& operator*=(float scalar);
	const ColorRGB& operator/=(const ColorRGB& color);
	const ColorRGB& operator/=(float scalar);
	const ColorRGB& operator+=(const ColorRGB& color);
	const ColorRGB& operator-=(const ColorRGB& color);

	ColorRGB& MaxToOne();

	ColorRGB GetMaxToOne() const;

	float red;
	float green;
	float blue;
};

ColorRGB operator*(float scalar, const ColorRGB& color);

static constexpr ColorRGB RED{ 1.0f, 0.0f, 0.0f };
static constexpr ColorRGB BLUE{ 0.0f, 0.0f, 1.0f };
static constexpr ColorRGB GREEN{ 0.0f, 1.0f, 0.0f };
static constexpr ColorRGB YELLOW{ 1.0f, 1.0f, 0.0f };
static constexpr ColorRGB CYAN{ 0.0f, 1.0f, 1.0f };
static constexpr ColorRGB MAGENTA{ 1.0f, 0.0f, 1.0f };
static constexpr ColorRGB WHITE{ 1.0f, 1.0f, 1.0f };
static constexpr ColorRGB BLACK{ 0.0f, 0.0f, 0.0f };
static constexpr ColorRGB GRAY{ 0.5f, 0.5f, 0.5f };
static constexpr ColorRGB DARK_GRAY{ 0.25f, 0.25f, 0.25f };