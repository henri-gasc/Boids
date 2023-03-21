#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <cstdlib>
#include <ctime>
#include <random>

#define WINDOW_HEIGHT 960
#define WINDOW_WIDTH 540

class WindowHandler {
public:
	sf::RenderWindow window;

	WindowHandler():
		window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "SFML", sf::Style::Default)
	{
		window.setFramerateLimit(40);
	}

	bool isRunning() {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
		return window.isOpen();
	}

	void draw(const sf::Drawable & drawable) {
		window.draw(drawable);
	}

	void display() {
		window.display();
	}
};

class Vect {
public:
	float x;
	float y;
	Vect(float value_x, float value_y) {
		x = value_x;
		y = value_y;
	}
};

class RandomNumberGenerator {
public:
	std::random_device rd;
	std::uniform_int_distribution<> distrib;
	std::mt19937 gen;

	RandomNumberGenerator(int minimum = 0, int maximum = 1):
		distrib(minimum, maximum),
		gen(rd())
	{}

	float pick() {
		return distrib(gen);
	}
};
