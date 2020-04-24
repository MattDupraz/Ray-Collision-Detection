#pragma once

#include "ball.h"

float get_collision_time(Ball const& A, Ball const& B);
float get_boundary_collision_time(Ball const& b, float max_x, float max_y);
