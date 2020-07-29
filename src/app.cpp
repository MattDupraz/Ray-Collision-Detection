#include "app.h"

#include "collisions.h"
#include <algorithm>
#include <utility>
#include <iostream>

App::App(unsigned int width, unsigned int height,
		sf::String const& title)
	: window(sf::VideoMode(width, height), title),
	width(width), height(height)
{
	clock.restart();
}

App::~App() {
	for (Ball* b: balls) {
		delete b;
	}
}

void App::add_ball(float radius, vec2f pos, vec2f vel) {
	Ball* ball(new Ball(radius, pos, vel));
	if (selected_ball == nullptr) {
		selected_ball = ball;
	}
	balls.push_back(ball);
}

void App::remove_ball(Ball* ball) {
	for (std::size_t i(0); i < balls.size(); ++i) {
		if (ball == balls[i]) {
			balls.erase(balls.begin() + i);
			--i;
		}
	}
}

int App::start() {	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} if (event.type == sf::Event::Resized) {
				unsigned int width = event.size.width;
				unsigned int height = event.size.height;
				event_resized(width, height);
			} if (event.type == sf::Event::MouseButtonPressed) {
				vec2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
				event_mouse_pressed(event.mouseButton.button, mouse_pos);
			} if (event.type == sf::Event::MouseButtonReleased) {	
				vec2f mouse_pos(event.mouseButton.x, event.mouseButton.y);
				event_mouse_released(event.mouseButton.button, mouse_pos);
			}
		}

		window.clear();

		update();
		draw();
		
		window.display();
	}

	return 0;
}

void App::event_resized(unsigned int w, unsigned int h) {
	width = w;
	height = h;
	sf::FloatRect visibleArea(0, 0, width, height);
	window.setView(sf::View(visibleArea));
}

void App::event_mouse_pressed(sf::Mouse::Button const& button, vec2f const& pos)
{
	if (button == sf::Mouse::Left) {
		for (Ball* ball : balls) {
			if (ball->contains(pos)) {
				dragged_ball = ball;
				selected_ball = ball;
				break;
			}
		}	
		if (dragged_ball == nullptr) {
			new_ball = new Ball(5.0, pos);
			selected_ball = new_ball;
			balls.push_back(new_ball);
		}
	}
	if (button == sf::Mouse::Right) {
		for (Ball* ball : balls) {
			if (ball->contains(pos)) {
				remove_ball(ball);
				delete ball;

				selected_ball = nullptr;
				new_ball = nullptr;
				dragged_ball = nullptr;
				break;
			}
		}	
	}
}

void App::event_mouse_released(sf::Mouse::Button const& button, vec2f const& pos)
{
	if (button == sf::Mouse::Left) {
		new_ball = nullptr;
		dragged_ball = nullptr;
	}
}

void App::update() {
	float dt = clock.restart().asSeconds();

	vec2f mouse_pos(sf::Mouse::getPosition(window));

	if (dragged_ball != nullptr) {
		dragged_ball->set_velocity((1.0f/dt) * (mouse_pos - dragged_ball->get_pos()));
		dragged_ball->set_pos(mouse_pos);
	}	

	if (new_ball != nullptr) {
		new_ball->set_radius(std::max(5.0f,
					norm(new_ball->get_pos() - mouse_pos)));
	}


	std::vector<Ball*> queue(balls);
	while (!queue.empty()) {
		bool found(false);
		float best_time;
		std::size_t best_A;
		std::size_t best_B;

		for (std::size_t i(0); i < queue.size(); ++i)
		for (std::size_t j(i + 1); j < queue.size(); ++j) {
			float time(get_collision_time(*queue[i], *queue[j]));
			if (time >= 0 && (!found || time < best_time)) {
				best_time = time;
				best_A = i;
				best_B = j;
				found = true;
			}
		}

		if (!found) {
			while (!queue.empty()) {
				Ball* ball = queue.back();
				float time = (get_boundary_collision_time(*ball, width, height));
				ball->set_ghost_pos(time);
				queue.pop_back();
			}
		} else {
			Ball* ball_A(queue[best_A]);
			Ball* ball_B(queue[best_B]);
			float time_A(get_boundary_collision_time(*ball_A, width, height));
			float time_B(get_boundary_collision_time(*ball_B, width, height));
			if ((time_A != 0.0 && time_A < best_time)
					|| (time_B != 0.0 && time_B < best_time)) {
				ball_A->set_ghost_pos(time_A);
				ball_B->set_ghost_pos(time_B);
			} else {
				ball_A->set_ghost_pos(best_time);
				ball_B->set_ghost_pos(best_time);
			}
			queue.erase(queue.begin() + best_B);
			queue.erase(queue.begin() + best_A);
		}
	}

	for (Ball* ball: balls) {
		if (ball == dragged_ball) continue;

		vec2f pos(ball->get_pos());
		vec2f vel(ball->get_velocity());
		float radius(ball->get_radius());

		ball->set_pos(pos + dt * vel);
		if ((pos.x > width - radius && vel.x > 0.0f)
				|| (pos.x < radius && vel.x < 0.0f)) {
			ball->set_velocity(vec2f(-vel.x, vel.y));
		}	
		if ((pos.y > height - radius && vel.y > 0.0f)
				|| (pos.y < radius && vel.y < 0.0f)) {
			ball->set_velocity(vec2f(vel.x, -vel.y));
		}		
	}

	
	for (std::size_t i(0); i < balls.size(); ++i)
	for (std::size_t j(i+1); j < balls.size(); ++j)
	{
		Ball* ball_A(balls[i]);
		Ball* ball_B(balls[j]);

		vec2f pos_A(ball_A->get_pos());
		vec2f pos_B(ball_B->get_pos());
		vec2f vel_A(ball_A->get_velocity());
		vec2f vel_B(ball_B->get_velocity());
		float R_A(ball_A->get_radius());
		float R_B(ball_B->get_radius());
		
		if (norm(pos_A - pos_B) < R_A + R_B) {
			vec2f AB = pos_B - pos_A;
			AB = AB * (1.0f / norm(AB));
			ball_A->set_velocity(vel_A - 2.0f*dot(AB, vel_A) * AB);
			vec2f BA = -AB;
			ball_B->set_velocity(vel_B - 2.0f*dot(BA, vel_B) * BA);
			if (ball_A == dragged_ball) {
				ball_B->set_pos(pos_B - BA);
			} else if (ball_B == dragged_ball) {
				ball_A->set_pos(pos_A - AB);
			} else {
				ball_A->set_pos(pos_A - 0.5f*AB);
				ball_B->set_pos(pos_B - 0.5f*BA);
			}
		}
	}
}

void App::draw() {
	for (Ball* ball : balls) {
		ball->draw(window);
	}
}
