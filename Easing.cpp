#define _USE_MATH_DEFINES
#include "Easing.h"
#include "Math.h"

float constant(float t){
	return t;
}

float easeInSine(float t)
{
	return 1 - cosf((t * (float)M_PI) / 2);
}

float easeOutSine(float t)
{
	return sinf((t * (float)M_PI) / 2);
}

float easeInOutSine(float t)
{
	return -(cosf((float)M_PI * t) - 1) / 2;

}

float easeInQuad(float t)
{
	return t * t;
}

float easeOutQuad(float t)
{
	return 1 - (1 - t) * (1 - t);
}

float easeInOutQuad(float t)
{
	return t < 0.5f ? 2 * t * t : 1 - powf(-2 * t + 2, 2) / 2;
}

float easeInCubic(float t)
{
	return t * t * t;
}

float easeOutCubic(float t)
{
	return 1 - powf(1 - t, 3);
}

float easeInOutCubic(float t)
{
	return t < 0.5f ? 4 * t * t * t : 1 - powf(-2 * t + 2, 3) / 2;
}

float easeInQuart(float t)
{
	return t * t * t * t;
}

float easeOutQuart(float t)
{
	return 1 - powf(1 - t, 4);
}

float easeInOutQuart(float t)
{
	return t < 0.5f ? 8 * t * t * t * t : 1 - powf(-2 * t + 2, 4) / 2;
}

float easeInQuint(float t)
{
	return t * t * t * t * t;
}

float easeOutQuint(float t)
{
	return 1 - powf(1 - t, 5);
}

float easeInOutQuint(float t)
{
	return t < 0.5f ? 16 * t * t * t * t * t : 1 - powf(-2 * t + 2, 5) / 2;
}

float easeInExpo(float t)
{
	return t == 0 ? 0 : powf(2, 10 * t - 10);
}

float easeOutExpo(float t)
{
	return t == 1 ? 1 : 1 - powf(2, -10 * t);
}

float easeInOutExpo(float t)
{
	return t == 0
		? 0
		: t == 1
		? 1
		: t < 0.5f ? powf(2, 20 * t - 10) / 2
		: (2 - powf(2, -20 * t + 10)) / 2;
}

float easeInCirc(float t)
{
	return 1 - sqrtf(1 - powf(t, 2));
}

float easeOutCirc(float t)
{
	return sqrtf(1 - powf(t - 1, 2));
}

float easeInOutCirc(float t)
{
	return t < 0.5f
		? (1 - sqrtf(1 - powf(2 * t, 2))) / 2
		: (sqrtf(1 - powf(-2 * t + 2, 2)) + 1) / 2;
}

float easeInBack(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * t * t * t - c1 * t * t;
}

float easeOutBack(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * powf(t - 1, 3) + c1 * powf(t - 1, 2);
}

float easeInOutBack(float t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return t < 0.5f
		? (powf(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
		: (powf(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

float easeInElastic(float t)
{
	const float c4 = (2 * (float)M_PI) / 3;

	return t == 0
		? 0
		: t == 1
		? 1
		: -powf(2, 10 * t - 10) * sinf((t * 10 - 10.75f) * c4);
}

float easeOutElastic(float t)
{
	const float c4 = (2 * (float)M_PI) / 3;

	return t == 0
		? 0
		: t == 1
		? 1
		: powf(2, -10 * t) * sinf((t * 10 - 0.75f) * c4) + 1;
}

float easeInOutElastic(float t)
{
	const float c5 = (2 * (float)M_PI) / 4.5f;

	return t == 0
		? 0
		: t == 1
		? 1
		: t < 0.5f
		? -(powf(2, 20 * t - 10) * sinf((20 * t - 11.125f) * c5)) / 2
		: (powf(2, -20 * t + 10) * sinf((20 * t - 11.125f) * c5)) / 2 + 1;
}

float easeInBounce(float t)
{
	
	return  1 - easeOutBounce(1 - t);
}

float easeOutBounce(float t)
{
	if (t < (1 / 2.75f)) {
		return 7.5625f * t * t;
	}
	else if (t < 2 / 2.75f) {
		return 7.5625f * (t -= 1.5f / 2.75f) * t + 0.75f;
	}
	else if (t < 2.5 / 2.75f) {
		return 7.5625f * (t -= 2.25f / 2.75f) * t + 0.9375f;
	}
	else {
		return 7.5625f * (t -= 2.625f / 2.75f) * t + 0.984375f;
	};

}

float easeInOutBounce(float t)
{
	
	return t < 0.5 ? (1 - easeOutBounce(1 - 2 * t)) / 2
		: (1 + easeOutBounce(2 * t - 1)) / 2;

}

