#include <SFML/Graphics/CircleShape.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdio>
#include <math.h>
#include "utils.hpp"

class Boid {
public:
	Vect pos;
	Vect speed;
	Vect acceleration;
	float vision_radius;
	float wanted_distance;
	float max_speed = 4;
	float max_force = 0.5;
	sf::CircleShape shape;

	Boid(int pos_x, int pos_y, RandomNumberGenerator *rng): 
		pos(pos_x, pos_y),
		speed(rng->pick()/100, rng->pick()/100),
		acceleration(0, 0),
		shape(5.f, 3)
	{
		vision_radius = 70;
		wanted_distance = 25;
	};

	void apply_borders() {
		if (pos.x < 0) { pos.x += WINDOW_HEIGHT;}
		else if (pos.x > WINDOW_HEIGHT) { pos.x -= WINDOW_HEIGHT;}
		if (pos.y < 0) { pos.y += WINDOW_WIDTH;}
		else if (pos.y > WINDOW_WIDTH) { pos.y -= WINDOW_WIDTH;}
	}

	float put_angle() {
		float angle = atan2(speed.x, -speed.y) * 180 / M_PI;
		shape.setRotation(angle);
		return angle;
	}

	void update_pos() {
		apply_borders();
		put_angle();
		shape.setPosition(pos.x, pos.y);
	}

	float distance(Vect other_pos) {
		return sqrt(pow(pos.x - other_pos.x, 2) + pow(pos.y - other_pos.y, 2));
	}

	void applyForce(Vect force) {
	    acceleration.addVect(force);
	}

	Vect Separation(boost::ptr_vector<Boid> all_boids) {
		Vect steering(0, 0);
		int count = 0;
		for (int i = 0; i < (int) all_boids.size(); i++) {
			float d = distance(all_boids[i].pos);
			if ((d > 0) && (d < wanted_distance)) {
				Vect diff(0, 0);
				diff = diff.subTwo(pos, all_boids[i].pos);
				diff.normalize();
				diff.multScalar(d);
				steering.addVect(diff);
				count++;
			}
		}
		if (count > 0) {
			steering.divScalar(count);
		}
		if (steering.magnitude() > 0) {
			steering.normalize();
			steering.multScalar(max_speed);
			steering.subVect(speed);
			steering.limit(max_force);
		}
		return steering;
	}

	Vect Alignement(boost::ptr_vector<Boid> all_boids) {
		Vect avg_speed(0, 0);
		int count = 0;
		for (int i = 0; i < (int) all_boids.size(); i++) {
			float d = distance(all_boids[i].pos);
			if ((d > 0) && (d < vision_radius)) {
				avg_speed.addVect(all_boids[i].speed);
				count++;
			}
		}
		if (count > 0) {
			avg_speed.divScalar(count);
			avg_speed.normalize();
			avg_speed.multScalar(max_speed);
			Vect steering(0, 0);
			steering  = steering.subTwo(avg_speed, speed);
			steering.limit(max_force);
			return steering;
		}
		else {
			return avg_speed;
		}
	}

	Vect Cohesion(boost::ptr_vector<Boid> all_boids) {
		Vect avg_loc(0, 0);
		int count = 0;
		for (int i = 0; i < (int) all_boids.size(); i++) {
			float d = distance(all_boids[i].pos);
			if ((d > 0) && (d < vision_radius)) {
				avg_loc.addVect(all_boids[i].pos);
				count++;
			}
		}
		if (count > 0) {
			Vect desire(0, 0);
			avg_loc.divScalar(count);
			desire.subVect(avg_loc);
			desire.normalize();
			desire.multScalar(max_speed);
			acceleration.subTwo(desire, speed);
			acceleration.limit(max_force);
			return acceleration;
		}
		else {
			Vect tmp(0, 0);
			return tmp;
		}
	}

	void update(boost::ptr_vector<Boid> all_boids) {
		Vect sep = Separation(all_boids);
		Vect ali = Alignement(all_boids);
		Vect coh = Cohesion(all_boids);

		applyForce(sep);
		applyForce(coh);
		applyForce(ali);

		acceleration.multScalar(.4);
		speed.addVect(acceleration);
		speed.limit(max_speed);
		pos.addVect(speed);
		acceleration.multScalar(0);

		update_pos();
	}
};
