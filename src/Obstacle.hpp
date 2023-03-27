#pragma once
#include "utils.hpp"

class Obstacle {
public:
	Vect pos;
	float r = 5.f;
	sf::CircleShape shape;

	Obstacle(RandomNumberGenerator *rng_width, RandomNumberGenerator *rng_height):
		pos(rng_height->pick(), rng_width->pick()),
		shape(2*r)
	{
		shape.setPosition(pos.x-r, pos.y-r);
		shape.setFillColor(sf::Color::Red);
	}
};
