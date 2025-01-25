#include "pch.h"
#include "EasingFunction.h"

namespace Dotween
{
	// Linear
	float EaseLinear(float x) { return x; }

	// Sine
	float EaseInSine(float x) { return static_cast<float>(1 - std::cos((x * PI) / 2)); }
	float EaseOutSine(float x) { return static_cast<float>(std::sin((x * PI) / 2)); }
	float EaseInOutSine(float x) { return static_cast<float>(-(std::cos(PI *x) - 1) / 2); }

	// Quad
	float EaseInQuad(float x) { return static_cast<float>(x * x); }
	float EaseOutQuad(float x) { return static_cast<float>(1 - (1 - x) * (1 - x)); }
	float EaseInOutQuad(float x) { return static_cast<float>(x < 0.5 ? 2 * x * x : 1 - std::pow(-2 * x + 2, 2) / 2); }

	// Cubic
	float EaseInCubic(float x) { return static_cast<float>(x * x * x); }
	float EaseOutCubic(float x) { return static_cast<float>(1 - std::pow(1 - x, 3)); }
	float EaseInOutCubic(float x) { return static_cast<float>(-(std::cos(PI *x) - 1) / 2); }

	// Quart
	float EaseInQuart(float x) { return static_cast<float>(x * x * x * x); }
	float EaseOutQuart(float x) { return static_cast<float>(1 - std::pow(1 - x, 4)); }
	float EaseInOutQuart(float x) { return static_cast<float>(x < 0.5 ? 8 * x * x * x * x : 1 - std::pow(-2 * x + 2, 4) / 2); }

	// Quint
	float EaseInQuint(float x) { return static_cast<float>(x * x * x * x * x); }
	float EaseOutQuint(float x) { return static_cast<float>(1 - std::pow(1 - x, 5)); }
	float EaseInOutQuint(float x) { return static_cast<float>(x < 0.5 ? 16 * x * x * x * x * x : 1 - std::pow(-2 * x + 2, 5) / 2); }

	// Expo
	float EaseInExpo(float x) { return static_cast<float>(x == 0 ? 0 : std::pow(2, 10 * x - 10)); }
	float EaseOutExpo(float x) { return static_cast<float>(x == 1 ? 1 : 1 - std::pow(2, -10 * x)); }
	float EaseInOutExpo(float x)
	{
		return static_cast<float>(x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5
			? std::pow(2, 20 * x - 10) / 2
			: (2 - std::pow(2, -20 * x + 10)) / 2);
	}

	// InCirc
	float EaseInCirc(float x) { return static_cast<float>(1 - std::sqrt(1 - std::pow(x, 2))); }
	float EaseOutCirc(float x) { return static_cast<float>(std::sqrt(1 - std::pow(x - 1, 2))); }
	float EaseInOutCirc(float x)
	{
		return static_cast<float>(x < 0.5
			? (1 - std::sqrt(1 - std::pow(2 * x, 2))) / 2
			: (std::sqrt(1 - std::pow(-2 * x + 2, 2)) + 1) / 2);
	}

	// Back
	float EaseInBack(float x)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return static_cast<float>(c3 * x * x * x - c1 * x * x);
	}
	float EaseOutBack(float x)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1;

		return static_cast<float>(1 + c3 * std::pow(x - 1, 3) + c1 * std::pow(x - 1, 2));
	}
	float EaseInOutBack(float x)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return static_cast<float>(x < 0.5
			? (std::pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
			: (std::pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2);
	}

	// Elastic
	float EaseInElastic(float x)
	{
		const float c4 = (2 * PI) / 3;

		return static_cast<float>(x == 0
			? 0
			: (x == 1
				? 1
				: -std::pow(2, 10 * x - 10) * std::sin((x * 10 - 10.75) * c4)));
	}
	float EaseOutElastic(float x)
	{
		const float c4 = (2 * PI) / 3;

		return static_cast<float>(x == 0
			? 0
			: x == 1
			? 1
			: std::pow(2, -10 * x) * std::sin((x * 10 - 0.75) * c4) + 1);
	}
	float EaseInOutElastic(float x)
	{
		const float c5 = (2 * PI) / 4.5f;

		return static_cast<float>(x == 0
			? 0
			: x == 1
			? 1
			: x < 0.5
			? -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2
			: (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1);
	}

	// Bounce
	float EaseInBounce(float x) { return static_cast<float>(1 - EaseOutBounce(1 - x)); }
	float EaseOutBounce(float x)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		float value;
		if (x < 1 / d1)
			value = n1 * x * x;
		else if (x < 2 / d1)
			value = n1 * (x -= 1.5f / d1) * x + 0.75f;
		else if (x < 2.5 / d1)
			value = n1 * (x -= 2.25f / d1) * x + 0.9375f;
		else
			value = n1 * (x -= 2.625f / d1) * x + 0.984375f;
		return static_cast<float>(value);
	}
	float EaseInOutBounce(float x)
	{
		return static_cast<float>(x < 0.5
			? (1 - EaseOutBounce(1 - 2 * x)) / 2
			: (1 + EaseOutBounce(2 * x - 1)) / 2);
	}

}
