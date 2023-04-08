#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include "utils.hpp"

class Boid: public SimuObject {
public:
	float vision_radius = 50;
	float distance_boids = 20;
	float distance_obstacle = 3 * distance_boids;
	float max_speed = 3.5;
	float max_force = 0.5;
	float border = 50;
	sf::CircleShape shape;
	Config *conf;

	Boid(int pos_x, int pos_y, RandomNumberGenerator *rng, Config* config):
		SimuObject(config),
		shape(3.f, 3)
	{
		pos.set(pos_x, pos_y);
		speed.set(rng->pick()/100, rng->pick()/100),
		conf = config;
	};

	/* We need this despite beeing the same function as in SimuObject
	   because otherwise there is boid appearing on screen */
	void update_pos() {
		apply_borders();
		shape.setRotation(get_angle());
		shape.setPosition(pos.x, pos.y);
	}

	/**
	 * @brief Apply the force to the boid
	 * 
	 * @param force The force to apply to acceleration
	 */
	void applyForce(Vect force) {
	    acceleration.addVect(force);
	}

	/**
	 * @brief The Separation rule (keep the boids separate)
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @return The force needed
	 */
	Vect Separation(boost::ptr_vector<Boid> all_boids) {
		Vect steering(0, 0);
		int count = 0;
		for (int i = 0; i < (int) all_boids.size(); i++) {
			float d = distance(all_boids[i].pos);
			if ((d > 0) && (d < distance_boids)) {
				Vect diff(0, 0);
				diff = diff.subTwo(pos, all_boids[i].pos);
				diff.normalize();
				diff.divScalar(d);
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

	/**
	 * @brief The Alignment rule (put the boids in the same direction)
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @return The force needed
	 */
	Vect Alignement(boost::ptr_vector<SimuObject> all_boids) {
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

	/**
	 * @brief The Cohesion rule (keep the boids close)
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @return The force needed
	 */
	Vect Cohesion(boost::ptr_vector<SimuObject> all_boids) {
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

	/**
	 * @brief Not working for now
	 * 
	 * @return The force needed
	 */
	Vect AvoidVerticalBorders() {
		Vect steering(0, 0);
		if ((pos.x > conf->window_height - border) || (pos.x < border)) {
			float angle = get_angle();
			if (abs(angle) >= 90) {
				steering.x = angle*0.2;
				steering.y = 0;
			}
		}
		return steering;
	}

	/**
	 * @brief Make it so the boid avoid the obstacles
	 * 
	 * @param all_obstacles The vector containing all obstacles
	 * @return The force needed to avoid them
	 */
	Vect AvoidObstacles(boost::ptr_vector<SimuObject> all_obstacles) {
		Vect steering(0, 0);
		int count = 0;
		for (int i = 0; i < (int) all_obstacles.size(); i++) {
			float d = distance(all_obstacles[i].pos);
			if ((d > 0) && (d < distance_obstacle)) {
				Vect diff(0, 0);
				diff = diff.subTwo(pos, all_obstacles[i].pos);
				diff.normalize();
				diff.divScalar(d*d*d);
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

	/**
	 * @brief Compute the forces for the rules, scale them, apply them, update the position
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @param all_obstacles The force needed to avoid them
	 */
	void update(boost::ptr_vector<Boid> all_boids, boost::ptr_vector<SimuObject> all_obstacles) {
		Vect sep = Separation(all_boids);
		Vect ali = Alignement(all_boids);
		Vect coh = Cohesion(all_boids);
		Vect obs = AvoidObstacles(all_obstacles);

		sep.multScalar(1.5);
		ali.multScalar(1.0);
		coh.multScalar(1.0);
		obs.multScalar(1.25);

		applyForce(sep);
		applyForce(coh);
		applyForce(ali);
		applyForce(obs);

		acceleration.multScalar(.4);
		speed.addVect(acceleration);
		speed.limit(max_speed);
		pos.addVect(speed);
		acceleration.multScalar(0);

		update_pos();
	}
};
