#include "collisions.h"
#include "math.h"
#include <algorithm>
#include "math_utils.h"
#include <iostream>
float get_collision_time(Ball const& A, Ball const& B) {
	vec2f AB(B.get_pos() - A.get_pos()); 
	vec2f vr(B.get_velocity() - A.get_velocity());
	if (norm(vr) < 1e-5) return -1.0;

	float R(A.get_radius() + B.get_radius());

	float vr2(norm2(vr));
	float AB2(norm2(AB));
	float AB_vr(dot(AB, vr));

	float D(pow(AB_vr, 2) + pow(R, 2)*vr2 - vr2*AB2);

	if (D < 0.0) {
		return -1.0;
	}

	return (-AB_vr - sqrt(D))/vr2;
}

float get_boundary_collision_time(Ball const& b, float max_x, float max_y) {
	float v_x(b.get_velocity().x);
	float v_y(b.get_velocity().y);
	
	float dx(-b.get_pos().x);
	if (v_x >= 0) {
		dx += max_x - b.get_radius();
	} else {
		dx += b.get_radius();
	}

	float dy(-b.get_pos().y);
	if (v_y >= 0) {
		dy += max_y - b.get_radius();
	} else {
		dy += b.get_radius();
	}

	if (abs(v_x) < 1e-5) {
		if (abs(v_y) < 1e-5) {
			return 0.0;
		} else {
			return dy/v_y;
		}
	} else {
		if (abs(v_y) < 1e-5) {
			return dx/v_x;
		} else {
			return std::min(dx/v_x, dy/v_y);
		}
	}
}	
