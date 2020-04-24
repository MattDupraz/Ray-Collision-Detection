#include "ball.h"

Ball::Ball(float radius, vec2f const& pos, vec2f const& velocity)
		: velocity(velocity) {
	set_radius(radius);
	set_pos(pos);
	set_ghost_pos(pos);

	ghost_shape.setFillColor(sf::Color::Transparent);
	ghost_shape.setOutlineColor(sf::Color::White);
	ghost_shape.setOutlineThickness(1.0f);
}

void Ball::set_radius(float r) {
	shape.setRadius(r);
	shape.setOrigin(r, r);
	ghost_shape.setRadius(r);
	ghost_shape.setOrigin(r, r);
}

void Ball::set_pos(vec2f const& pos) {
	shape.setPosition(pos);
}

void Ball::set_ghost_pos(vec2f const& pos) {
	ghost_shape.setPosition(pos);
}

void Ball::draw(sf::RenderWindow& window) {
	sf::VertexArray lines(sf::Lines, 2);
	lines[0].position = get_pos();
	lines[1].position = get_ghost_pos();
	window.draw(lines);

	window.draw(ghost_shape);
	window.draw(shape);
}

bool Ball::contains(vec2f const& vec) const {
	return norm(vec - get_pos()) < get_radius();
}
