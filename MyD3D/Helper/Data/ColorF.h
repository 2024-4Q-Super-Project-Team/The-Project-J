#pragma once

// 1/255ÀÇ °ª
#define DEV255 0.00392157f

class ColorF
{
public:
    ColorF(float* _rgba)
        : r(_rgba[0]), g(_rgba[1]), b(_rgba[2]), a(_rgba[3]) {}
	ColorF(float _r = 0.0f, float _g = 0.0f, float _b = 0.0f, float _a = 1.0f)
		: r(_r), g(_g), b(_b), a(_a) {}
	ColorF(int _r, int _g, int _b, float _a = 1.0f)
		: r((float)_r * DEV255), g((float)_g * DEV255), b((float)_b * DEV255), a(_a) {}
public:
	UINT32 GetRGB()
	{
		UINT8 r_ = static_cast<UINT8>(r * 255.0f);
		UINT8 g_ = static_cast<UINT8>(g * 255.0f);
		UINT8 b_ = static_cast<UINT8>(b * 255.0f);
		return (r_ << 16) | (g_ << 8) | (b_);
	}
public:
    operator float* () { return &r; }
    operator Vector4() { return Vector4(r, g, b, a); }
public:
    float r, g, b, a;
    // Static Func
public:
	static ColorF Gray(float _alpha = 1.0f) { return ColorF(0.3f, 0.3f, 0.3f, _alpha); }
	static ColorF Black(float _alpha = 1.0f) { return ColorF(0.0f, 0.0f, 0.0f, _alpha); }
	static ColorF White(float _alpha = 1.0f) { return ColorF(1.0f, 1.0f, 1.0f, _alpha); }
	static ColorF Red(float _alpha = 1.0f) { return ColorF(1.0f, 0.0f, 0.0f, _alpha); }
	static ColorF Green(float _alpha = 1.0f) { return ColorF(0.0f, 1.0f, 0.0f, _alpha); }
	static ColorF Blue(float _alpha = 1.0f) { return ColorF(0.0f, 0.0f, 1.0f, _alpha); }
	// ¿¹»Û »ö±òµé!!
	static ColorF MintBlue(float _alpha = 1.0f) { return ColorF(0.4666666f, 0.86666666f, 0.9058823f, _alpha); }
	static ColorF LavenderPurple(float _alpha = 1.0f) { return ColorF(0.58823529f, 0.48235294f, 0.71372549f, _alpha); }
	static ColorF DeepCoral(float _alpha = 1.0f) { return ColorF(1.0f, 0.4f, 0.4f, _alpha); }
	static ColorF CadetBlue(float _alpha = 1.0f) { return ColorF(0.37f, 0.62f, 0.62f, _alpha); }
	static ColorF LightSlateBlue(float _alpha = 1.0f) { return ColorF(0.35f, 0.60f, 0.70f, _alpha); }
};
