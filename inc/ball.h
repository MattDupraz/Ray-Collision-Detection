#pragma once

#include "vec.h"

struct Ball {
	vec2f pos;
	float radius;

	vec2f get_collision_pos(vec2f target_pos, Ball const& other);
};
