#pragma once
#include "utils.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class Obstacle {
public:
	Vect pos;
	sf::CircleShape shape;

	Obstacle(RandomNumberGenerator *rng_width, RandomNumberGenerator *rng_height):
		pos(rng_height->pick(), rng_width->pick()),
		shape(10.f)
	{
		shape.setPosition(pos.x, pos.y);
		shape.setFillColor(sf::Color::Red);
	}
};
