#pragma once

#include <SFML/Graphics.hpp>
#include "math_utils.h"

class Ball {
	public:
		Ball (float radius, vec2f const& pos,
				vec2f const& velocity = vec2f(0.0, 0.0));
		Ball (float radius, float x, float y, float v_x = 0.0, float v_y = 0.0)
			: Ball(radius, vec2f(x, y), vec2f(v_x, v_y)) {}

		vec2f get_pos() const { return shape.getPosition(); };
		vec2f get_ghost_pos() const { return ghost_shape.getPosition(); }
		float get_radius() const { return shape.getRadius(); }

		void set_pos(vec2f const& v);
		void set_ghost_pos(vec2f const& v);
		void set_ghost_pos(float t)
			{ set_ghost_pos(get_pos() + t*get_velocity()); }
		void set_radius(float f);

		void set_velocity(vec2f const& v) { velocity = v; }
		vec2f get_velocity() const { return velocity; }

		bool is_selected() const { return selected; }
		void set_selected(bool b) { selected = b; }

		bool is_mouse_over() const { return mouse_over; }
		bool set_mouse_over(bool b) { mouse_over = b; }

		bool contains(vec2f const& v) const;

		void draw(sf::RenderWindow& window);

	private:
		vec2f velocity;

		sf::CircleShape shape;
		sf::CircleShape ghost_shape;

		bool selected = false;
		bool mouse_over = false;
};
