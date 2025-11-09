#include "BaseFunctions.h"

float lerpFunc(float a, float b, float t)
{
	return a + t * (b - a);
}