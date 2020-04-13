#include "ball.h"
#include "math.h"

vec2f Ball::get_collision_pos(vec2f target_pos, Ball const& other) {
	vec2f AB1 = other.pos - pos;
	vec2f AB2 = other.pos - target_pos;

	float R = radius + other.radius;
	float d = 4 * pow(R, 2) * (AB1 - AB2).norm2();
	d += 4 * pow(AB1 * AB2, 2);
	d -= 4 * AB1.norm2() * AB2.norm2();

	if (d < 0.0) {
		return target_pos;
	}

	float sqrt_d = sqrt(d);

	float b = 2 * (AB1 * (AB2 - AB1));
	float a = (AB1 - AB2).norm2();

	float lambda1 = (-b - sqrt_d) / (2 * a);
	float lambda2 = (-b + sqrt_d) / (2 * a);
	
	if (lambda1 >= 0.0) {
		if (lambda1 > 1.0) {
			return target_pos;
		} else {
			return ((1 - lambda1) * pos + lambda1 * target_pos);
		}
	} else if (lambda2 >= 0.0){
		if (lambda2 > 1.0) {
			return target_pos;
		} else {
			return ((1 - lambda2) * pos + lambda2 * target_pos);
		}
	} else {
		return target_pos;
	}
}
