#include <SFML/Graphics/CircleShape.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdio>
#include <math.h>
#include "utils.hpp"

class Boid {
public:
	Vect pos;
	float speed;
	float angle;
	float acceleration;
	float theta_der;
	float vision_radius;
	sf::CircleShape shape;

	Boid(int pos_x, int pos_y, RandomNumberGenerator *rng): 
		pos(pos_x, pos_y),
		shape(5.f, 3)
	{
		speed = rng->pick()/100;
		angle = rng->pick()/rng->range() * 2*M_PI;
		acceleration = 0;
		theta_der = 0;
		vision_radius = 10;
		update_pos();
	};

	void apply_borders() {
		if (pos.x < 0) { pos.x += WINDOW_HEIGHT;}
		else if (pos.x > WINDOW_HEIGHT) { pos.x -= WINDOW_HEIGHT;}
		if (pos.y < 0) { pos.y += WINDOW_WIDTH;}
		else if (pos.y > WINDOW_WIDTH) { pos.y -= WINDOW_WIDTH;}
	}

	void update_pos() {
		speed += acceleration*0.01;
		angle += theta_der*0.01;
		pos.x += speed * cos(angle);
		pos.y += speed * sin(angle);
		apply_borders();
		shape.setPosition(pos.x, pos.y);
		shape.setRotation(- angle*180/M_PI);
		acceleration += 0.1;
	}

	float distance(Vect other_pos) {
		return sqrt(pow(pos.x - other_pos.x, 2) + pow(pos.y - other_pos.y, 2));
	}

	void apply_flock(boost::ptr_vector<Boid> all_boids) {
		float mid_speed = 0;
		float mid_angle = 0;
		// Vect mid_position(0, 0);
		int boid_in_flock = 0;

		for (int i = 0; i < (int) all_boids.size(); i++) {
			if (distance(all_boids[i].pos) < vision_radius) {
				mid_speed += all_boids[i].speed;
				// mid_position.x += all_boids[i].pos.x;
				// mid_position.y += all_boids[i].pos.y;
				mid_angle += all_boids[i].angle;
				boid_in_flock++;
			}
		}
		if (boid_in_flock != 0) {
			acceleration = (mid_speed / boid_in_flock) - speed;
			theta_der = (mid_angle / boid_in_flock) - angle;
			// pos.x = mid_position.x / boid_in_flock;
			// pos.y = mid_position.y / boid_in_flock;
		}
	}

};
