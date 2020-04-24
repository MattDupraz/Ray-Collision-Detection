#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "ball.h"
#include "math_utils.h"

class App {	
	public:
		App(unsigned int width, unsigned int height,
				sf::String const& title);

		~App();

		void add_ball(float radius, vec2f pos, vec2f vel = {0.0, 0.0});
		void remove_ball(Ball* ball);
		int start();

		void update();
		void draw();
	private:
		sf::RenderWindow window;
		unsigned int width;
		unsigned int height;

		std::vector<Ball*> balls;

		sf::Clock clock;

		Ball* new_ball = nullptr;
		Ball* dragged_ball = nullptr;
		Ball* selected_ball = nullptr;

		void event_resized(unsigned int width, unsigned int height);
		void event_mouse_pressed(sf::Mouse::Button const& button,
				vec2f const& pos);
		void event_mouse_released(sf::Mouse::Button const& button,
				vec2f const& pos);
};
