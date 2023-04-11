#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include "utils.hpp"

class Boid: public SimuObject {
public:
	float distance_boids = 20;
	float distance_obstacle = 3 * distance_boids;
	float max_speed = 3.5;
	float max_force = 0.5;
	float border = 100;
	sf::CircleShape shape;
	Config *conf;

	Boid(int pos_x, int pos_y, RandomNumberGenerator *rng, Config* config):
		SimuObject(config),
		shape(3.f, 3)
	{
		pos.set(pos_x + rng->pick(), pos_y + rng->pick());
		speed.set(rng->pick()/100, rng->pick()/100),
		conf = config;
	};

	/* We need this despite beeing the same function as in SimuObject
	   because otherwise there is boid appearing on screen */
	void update_pos();

	/**
	 * @brief Apply the force to the boid
	 * 
	 * @param force The force to apply to acceleration
	 */
	void applyForce(Vect force);

	/**
	 * @brief All boids-on-boids rules
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @return The force needed
	 */
	Vect rulesBoid(boost::ptr_vector<Boid> *all_boids);
	// To view all rules as separate function, view cb9a4aa

	/**
	 * @brief Not working for now
	 * 
	 * @return The force needed
	 */
	Vect AvoidVerticalBorders();

	/**
	 * @brief Make it so the boid avoid the obstacles
	 * 
	 * @param all_obstacles The vector containing all obstacles
	 * @return The force needed to avoid them
	 */
	Vect AvoidObstacles(boost::ptr_vector<SimuObject> *all_obstacles);

	/**
	 * @brief Compute the forces for the rules, scale them, apply them, update the position
	 * 
	 * @param all_boids The vector containing all Boid objects
	 * @param all_obstacles The force needed to avoid them
	 */
	void update(boost::ptr_vector<Boid> *all_boids, boost::ptr_vector<SimuObject> *all_obstacles);
};
