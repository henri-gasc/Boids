#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <cstdlib>
#include <ctime>
#include <math.h>
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

	float magnitude() {
		return sqrt(x*x + y*y);
	}

	void normalize() {
		float m = magnitude();
		if (m != 0) {
			x = x/m;
			y = y/m;
		}
	}

	void multScalar(float d) {
		x *= d;
		y *= d;
	}
	void divScalar(float d) {
		x /= d;
		y /= d;
	}

	void addVect(Vect v) {
		x += v.x;
		y += v.y;
	}
	void subVect(Vect v) {
		x -= v.x;
		y -= v.y;
	}

	void limit(float max) {
		float size = magnitude();
	    if (size > max) {
			x /= size;
			y /= size;
    	}
	}

	Vect subTwo(Vect v_1, Vect v_2) {
		Vect diff(0, 0);
		diff.x = v_1.x - v_2.x;
		diff.y = v_1.y - v_2.y;
		return diff;
	}
};

class RandomNumberGenerator {
private:
	std::random_device rd;
	std::uniform_int_distribution<> distrib;
	std::mt19937 gen;
public:
	int min;
	int max;
	RandomNumberGenerator(int minimum = 0, int maximum = 1):
		distrib(minimum, maximum),
		gen(rd())
	{
		min = minimum;
		max = maximum;
	}

	float pick() {
		return distrib(gen);
	}

	int range() {
		return max - min;
	}
};
