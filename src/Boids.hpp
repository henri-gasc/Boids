#include <SFML/Graphics/CircleShape.hpp>
#include <math.h>
#include "utils.hpp"

class Boid {
public:
	Vect pos;
	float speed;
	float acceleration;
	float angle;
	sf::CircleShape shape;

	Boid(float pos_x, float pos_y, RandomNumberGenerator *rng): 
		pos(pos_x, pos_y),
		shape(5.f, 3)
	{
		speed = rng->pick()/100;
		angle = rng->pick()/rng->range() * 2*M_PI;
		acceleration = 0;
		update_pos();
	};

	void apply_borders() {
		if (pos.x < 0) { pos.x += WINDOW_HEIGHT;}
		else if (pos.x > WINDOW_HEIGHT) { pos.x -= WINDOW_HEIGHT;}
		if (pos.y < 0) { pos.y += WINDOW_WIDTH;}
		else if (pos.y > WINDOW_WIDTH) { pos.y -= WINDOW_WIDTH;}
	}

	void update_pos() {
		pos.x += speed * cos(angle);
		pos.y += speed * sin(angle);
		apply_borders();
		shape.setPosition(pos.x, pos.y);
	}

};
