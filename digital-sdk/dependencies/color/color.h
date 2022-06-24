#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class c_color
{
public:
	c_color();
	c_color(int _r, int _g, int _b);
	c_color(int _r, int _g, int _b, int _a);
	c_color(float _r, float _g, float _b) : c_color(_r, _g, _b, 1.0f) {}
	c_color(float _r, float _g, float _b, float _a)
		: c_color(
			static_cast<int>(_r * 255.0f),
			static_cast<int>(_g * 255.0f),
			static_cast<int>(_b * 255.0f),
			static_cast<int>(_a * 255.0f))
	{
	}
	explicit c_color(float* rgb) : c_color(rgb[0], rgb[1], rgb[2], rgb[3]) {}
	explicit c_color(unsigned long argb)
	{
		_CColor[2] = static_cast<unsigned char>((argb & 0x000000FF) >> (0 * 8));
		_CColor[1] = static_cast<unsigned char>((argb & 0x0000FF00) >> (1 * 8));
		_CColor[0] = static_cast<unsigned char>((argb & 0x00FF0000) >> (2 * 8));
		_CColor[3] = static_cast<unsigned char>((argb & 0xFF000000) >> (3 * 8));
	}

	void    set_raw_color(int color32);
	[[nodiscard]] int     get_raw_color() const;
	void set_alpha(int alpha);
	void    set_color(int _r, int _g, int _b, int _a = 0);
	void    set_color(float _r, float _g, float _b, float _a = 0);
	void    get_color(int& _r, int& _g, int& _b, int& _a) const;

	[[nodiscard]] std::string GetNormalnijHexColor() const;

	[[nodiscard]] int r() const { return _CColor[0]; }
	[[nodiscard]] int g() const { return _CColor[1]; }
	[[nodiscard]] int b() const { return _CColor[2]; }
	[[nodiscard]] int a() const { return _CColor[3]; }

	unsigned char& operator[](int index)
	{
		return _CColor[index];
	}
	const unsigned char& operator[](int index) const
	{
		return _CColor[index];
	}

	bool operator==(const c_color& rhs) const;
	bool operator!=(const c_color& rhs) const;
	c_color& operator=(const c_color& rhs);

	static c_color from_hsb(float hue, float saturation, float brightness)
	{
		float h = hue == 1.0f ? 0 : hue * 6.0f;
		float f = h - static_cast<int>(h);
		float p = brightness * (1.0f - saturation);
		float q = brightness * (1.0f - saturation * f);
		float t = brightness * (1.0f - (saturation * (1.0f - f)));

		if (h < 1)
		{
			return {
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(t * 255),
				static_cast<unsigned char>(p * 255)
			};
		}
		if (h < 2)
		{
			return {
				static_cast<unsigned char>(q * 255),
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(p * 255)
			};
		}
		if (h < 3)
		{
			return {
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(brightness * 255),
				static_cast<unsigned char>(t * 255)
			};
		}
		if (h < 4)
		{
			return {
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(q * 255),
				static_cast<unsigned char>(brightness * 255)
			};
		}
		if (h < 5)
		{
			return {
				static_cast<unsigned char>(t * 255),
				static_cast<unsigned char>(p * 255),
				static_cast<unsigned char>(brightness * 255)
			};
		}
		return {
			static_cast<unsigned char>(brightness * 255),
			static_cast<unsigned char>(p * 255),
			static_cast<unsigned char>(q * 255)
		};
	}

	static c_color black;
	static c_color white;
	static c_color red;
	static c_color green;
	static c_color blue;

private:
	unsigned char _CColor[4]{};
};