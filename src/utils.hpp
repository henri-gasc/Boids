#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <random>
#include "config.hpp"

/**
 * @brief Class wrapper for the screen
 * 
 */
class Application {
public:
	sf::RenderWindow window;
	sf::RenderTexture texture;
	Config *conf;

	Application(Config *config):
		window(sf::VideoMode(config->window_width, config->window_height), "SFML", sf::Style::Default)
	{
		conf = config;
		if (conf->save_to_file) {
			texture.create(conf->window_width, conf->window_height);
		}
		window.setFramerateLimit(conf->framerate);
	}

	/**
	 * @brief Test if the app is running, apply some events too
	 * 
	 * @return true if the app if running
	 * @return false if the app is not running
	 */
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

	/**
	 * @brief Draw the object on the screen
	 * 
	 * @param drawable The object to draw
	 */
	void draw(const sf::Drawable & drawable) {
		if (conf->show_simulations) {
			window.draw(drawable);
		}
		if (conf->save_to_file) {
			texture.draw(drawable);
		}
	}

	/**
	 * @brief Wrapper around the RenderWindow class
	 * 
	 */
	void display() {
		window.display();
	}

	/**
	 * @brief Save the current frame of the simulation to disk
	 * 
	 * @param number The number of the frame
	 */
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

	/**
	 * @brief Change the value
	 * 
	 * @param value_x The value for the first_coordinate
	 * @param value_y The value for the second coordinate
	 */
	void set(float value_x, float value_y) {
		x = value_x;
		y = value_y;
	}

	/**
	 * @brief Compute the magnitude of the object
	 * 
	 * @return the magnitude
	 */
	float magnitude() {
		return sqrt(x*x + y*y);
	}

	/**
	 * @brief Normalize the vector
	 * 
	 */
	void normalize() {
		float m = magnitude();
		if (m != 0) {
			x = x/m;
			y = y/m;
		}
	}

	/**
	 * @brief Apply the dot product and return the result
	 * 
	 * @param v The vector to apply the dot product to
	 * @return The dot product
	 */
	float dot(Vect v) {
		return x*v.x + y*v.y;
	}

	/**
	 * @brief Multiply the vector by the scalar
	 * 
	 * @param d The scalar in question
	 */
	void multScalar(float d) {
		x *= d;
		y *= d;
	}
	/**
	 * @brief Divide the vector by the scalar
	 * 
	 * @param d The scalar in question
	 */
	void divScalar(float d) {
		x /= d;
		y /= d;
	}

	/**
	 * @brief Add the vector to itself
	 * 
	 * @param v The vector in question
	 */
	void addVect(Vect v) {
		x += v.x;
		y += v.y;
	}
	/**
	 * @brief Subtract the vector to itself
	 * 
	 * @param v The vector in question
	 */
	void subVect(Vect v) {
		x -= v.x;
		y -= v.y;
	}

	/**
	 * @brief Limit the values of the vector to some value
	 * 
	 * @param max the maximum value for our vector
	 */
	void limit(float max) {
		float size = magnitude();
		if (size > max) {
			x /= size;
			y /= size;
		}
	}

	/**
	 * @brief Subtract two vectors and return the difference
	 * 
	 * @param v_1 The first vector
	 * @param v_2 The second vector
	 * @return The difference between the first and the second vector
	 */
	Vect subTwo(Vect v_1, Vect v_2) {
		Vect diff(0, 0);
		diff.x = v_1.x - v_2.x;
		diff.y = v_1.y - v_2.y;
		return diff;
	}
};

/**
 * @brief Allow to easily generate a random number
 * 
 */
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

	/**
	 * @brief Pick a random number in the distribution
	 * 
	 * @return a random number
	 */
	float pick() {
		return distrib(gen);
	}

	/**
	 * @brief Compute the range of value we can pick from
	 * 
	 * @return the range
	 */
	int range() {
		return max - min;
	}
};

/**
 * @brief A class for each object appearing in the simulation
 * 
 */
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

	/**
	 * @brief Put the object in the screen if it goes out of bound
	 * 
	 */
	void apply_borders() {
		if (pos.x < 0) { pos.x += conf->window_width;}
		else if (pos.x > conf->window_width) { pos.x -= conf->window_width;}
		if (pos.y < 0) { pos.y += conf->window_height;}
		else if (pos.y > conf->window_height) { pos.y -= conf->window_height;}
	}

	/**
	 * @brief Get the angle of the object
	 * 
	 * @return the angle
	 */
	float get_angle() {
		float angle = atan2(speed.x, -speed.y) * 180 / M_PI;
		return angle;
	}

	/**
	 * @brief Update the position of the object
	 * 
	 */
	void update_pos() {
		apply_borders();
		shape.setRotation(get_angle());
		shape.setPosition(pos.x, pos.y);
	}

	/**
	 * @brief Compute the distance with another object
	 * 
	 * @param other_pos The position of the other SimuObject
	 * @return the distance
	 */
	float distance(Vect other_pos) {
		return sqrt(pow(pos.x - other_pos.x, 2) + pow(pos.y - other_pos.y, 2));
	}
};
