#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "ball.h"

class App {	
	public:
		App(unsigned int width, unsigned int height,
				sf::String const& title);

		~App();

		void add_ball(vec2f pos, float radius);
		void remove_ball(Ball* ball);
		int start();

		void update();
		void draw();
	private:
		sf::RenderWindow window;

		std::vector<Ball*> balls;

		Ball* new_ball = nullptr;
		Ball* dragged_ball = nullptr;
		Ball* selected_ball = nullptr;
};
