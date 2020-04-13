#include "app.h"
#include "math.h"

#include <iostream>

App::App(unsigned int width, unsigned int height,
		sf::String const& title)
	: window(sf::VideoMode(width, height), title)
{}

App::~App() {
	for (Ball* b: balls) {
		delete b;
	}
}

void App::add_ball(vec2f pos, float radius) {
	Ball* ball(new Ball {pos, radius});
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
				sf::FloatRect visibleArea(0, 0, width, height);
				window.setView(sf::View(visibleArea));
			} if (event.type == sf::Event::MouseButtonPressed) {
				vec2f mouse_pos {event.mouseButton.x, event.mouseButton.y};
				if (event.mouseButton.button == sf::Mouse::Left) {
					for (Ball* ball : balls) {
						if ((ball->pos - mouse_pos).norm() < ball->radius) {
							dragged_ball = ball;
							selected_ball = ball;
							break;
						}
					}	
					if (dragged_ball == nullptr) {
						new_ball = new Ball{mouse_pos, 0};
						selected_ball = new_ball;
						balls.push_back(new_ball);
					}
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					for (Ball* ball : balls) {
						if ((ball->pos - mouse_pos).norm() < ball->radius) {
							remove_ball(ball);
							delete ball;

							selected_ball = nullptr;
							new_ball = nullptr;
							dragged_ball = nullptr;
							break;
						}
					}	
				}
			} if (event.type == sf::Event::MouseButtonReleased) {	
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (new_ball != nullptr && new_ball->radius < 1.0f) {
						remove_ball(new_ball);
					}
					new_ball = nullptr;
					dragged_ball = nullptr;
				}
			}
		}

		window.clear();

		update();
		draw();
		
		window.display();
	}

	return 0;
}

void App::update() {
	sf::Vector2i sf_mouse_pos(sf::Mouse::getPosition(window));
	vec2f mouse_pos { float(sf_mouse_pos.x), float(sf_mouse_pos.y) };

	if (dragged_ball != nullptr) {
		dragged_ball->pos = mouse_pos;
	}	

	if (new_ball != nullptr) {
		new_ball->radius = (new_ball->pos - mouse_pos).norm();
	}
}

void App::draw() {
	sf::Vector2i sf_mouse_pos(sf::Mouse::getPosition(window));
	vec2f mouse_pos { float(sf_mouse_pos.x), float(sf_mouse_pos.y) };

	sf::CircleShape circle;
	for (Ball* ball : balls) {
		circle.setRadius(ball->radius);
		circle.setPosition(ball->pos.x() - ball->radius,
				ball->pos.y() - ball->radius);
		if ((ball->pos - mouse_pos).norm() < ball->radius) {
			circle.setFillColor(sf::Color::Red);
		} else if (ball == selected_ball){
			circle.setFillColor(sf::Color::Yellow);
		} else {
			circle.setFillColor(sf::Color::Blue);
		}
		window.draw(circle);
	}

	if (selected_ball != nullptr) {
		sf::Vertex line[] =
		{
			 sf::Vertex(sf::Vector2f(selected_ball->pos.x(), selected_ball->pos.y())),
			 sf::Vertex(sf::Vector2f(mouse_pos.x(), mouse_pos.y()))
		};

		window.draw(line, 2, sf::Lines);

		vec2f collision_pos(mouse_pos);
		for (Ball* ball: balls) {
			if (ball == selected_ball)
				continue;
			vec2f new_collision_pos(selected_ball->get_collision_pos(mouse_pos, *ball));
			if ((selected_ball->pos - new_collision_pos).norm2()
					< (selected_ball->pos - collision_pos).norm2()) {
				collision_pos = new_collision_pos;
			}
		}

		circle.setPosition(mouse_pos.x() - selected_ball->radius,
				mouse_pos.y() - selected_ball->radius);
		circle.setRadius(selected_ball->radius);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(sf::Color::White);
		circle.setOutlineThickness(1.0f);
		window.draw(circle);

		circle.setPosition(collision_pos.x() - selected_ball->radius,
				collision_pos.y() - selected_ball->radius);
		circle.setOutlineColor(sf::Color::Red);
		window.draw(circle);
	}
}
