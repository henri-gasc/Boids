#include <SFML/Graphics/CircleShape.hpp>
#include "utils.hpp"

class Boid {
public:
	Vect pos;
	Vect speed;
	Vect acceleration;
	sf::CircleShape shape;

	Boid(float pos_x, float pos_y, float speed_x, float speed_y): 
		pos(pos_x, pos_y),
		speed(speed_x, speed_y),
		acceleration(0, 0),
		shape(5.f, 3)
	{
		update_pos();
	};

	void apply_borders() {
		if (pos.x < 0) { pos.x += WINDOW_HEIGHT;}
		else if (pos.x > WINDOW_HEIGHT) { pos.x -= WINDOW_HEIGHT;}
		if (pos.y < 0) { pos.y += WINDOW_WIDTH;}
		else if (pos.y > WINDOW_WIDTH) { pos.y -= WINDOW_WIDTH;}
	}

	void update_pos() {
		pos.x += speed.x;
		pos.y += speed.y;
		apply_borders();
		shape.setPosition(pos.x, pos.y);
	}

};
