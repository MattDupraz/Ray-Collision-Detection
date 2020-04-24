#include "math_utils.h"
#include "math.h"

float dot(vec2f v1, vec2f v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

float norm2(vec2f v) {
	return dot(v, v);
}

float norm(vec2f v) {
	return sqrt(norm2(v));
}
