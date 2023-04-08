#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <math.h>
#include <random>
#include "config.hpp"

class Application {
public:
	sf::RenderWindow window;
	sf::RenderTexture texture;
	Config *conf;

	Application(Config *config):
		window(sf::VideoMode(config->window_height, config->window_width), "SFML", sf::Style::Default)
	{
		conf = config;
		if (conf->save_to_file) {
			texture.create(conf->window_height, conf->window_width);
		}
		window.setFramerateLimit(conf->framerate);
	}

	bool isRunning() {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.key.code == sf::Keyboard::Q) {
				window.close();
			}
		}

		window.clear();
		texture.clear();
		return window.isOpen();
	}

	void draw(const sf::Drawable & drawable) {
		if (conf->show_simulations) {
			window.draw(drawable);
		}
		if (conf->save_to_file) {
			texture.draw(drawable);
		}
	}

	void display() {
		window.display();
	}

	void save(int number) {
		if (not conf->save_to_file) return;
		char name[100] = "images/part.";
		char format[6] = ".%05i";
		sprintf(strstr(name, "."), format, number);
		strcat(name, ".png");
		texture.getTexture().copyToImage().saveToFile(name);
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

	void set(float value_x, float value_y) {
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

	float dot(Vect v) {
		return x*v.x + y*v.y;
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

class SimuObject {
public:
	Vect pos;
	Vect speed;
	Vect acceleration;
	float r = 5.f;
	sf::CircleShape shape;
	Config *conf;

	SimuObject(Config *config):
		pos(0, 0),
		speed(0, 0),
		acceleration(0, 0),
		shape(2*r)
	{
		conf = config;
		update_pos();
		shape.setFillColor(sf::Color::Red);
	}

	void apply_borders() {
		if (pos.x < 0) { pos.x += conf->window_height;}
		else if (pos.x > conf->window_height) { pos.x -= conf->window_height;}
		if (pos.y < 0) { pos.y += conf->window_width;}
		else if (pos.y > conf->window_width) { pos.y -= conf->window_width;}
	}

	float get_angle() {
		float angle = atan2(speed.x, -speed.y) * 180 / M_PI;
		return angle;
	}

	void update_pos() {
		apply_borders();
		shape.setRotation(get_angle());
		shape.setPosition(pos.x, pos.y);
	}

	float distance(Vect other_pos) {
		return sqrt(pow(pos.x - other_pos.x, 2) + pow(pos.y - other_pos.y, 2));
	}
};
