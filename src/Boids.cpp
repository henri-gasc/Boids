#include "Boids.hpp"

/* We need this despite beeing the same function as in SimuObject
	because otherwise there is boid appearing on screen */
void Boid::update_pos() {
	apply_borders();
	shape.setRotation(get_angle());
	shape.setPosition(pos.x, pos.y);
}

void Boid::applyForce(Vect force) {
	acceleration.addVect(force);
}

Vect Boid::Separation(boost::ptr_vector<Boid> all_boids) {
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

Vect Boid::Alignement(boost::ptr_vector<SimuObject> all_boids) {
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

Vect Boid::Cohesion(boost::ptr_vector<SimuObject> all_boids) {
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

Vect Boid::AvoidVerticalBorders() {
	Vect steering(0, 0);
	float d = 0;
	if ((pos.x > conf->window_height - border) || (pos.x < border)) {
		Vect diff(0, 0);
		if (pos.x > conf->window_height - border) {
			d = conf->window_height - pos.x;
		}
		else {
			d = pos.x;
		}
		diff.x = d;
		diff.normalize();
		diff.divScalar(d);
		steering.addVect(diff);
	}
	return steering;
}

Vect Boid::AvoidObstacles(boost::ptr_vector<SimuObject> all_obstacles) {
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

void Boid::update(boost::ptr_vector<Boid> all_boids, boost::ptr_vector<SimuObject> all_obstacles) {
	Vect sep = Separation(all_boids);
	Vect ali = Alignement(all_boids);
	Vect coh = Cohesion(all_boids);
	Vect obs = AvoidObstacles(all_obstacles);
	Vect bor = AvoidVerticalBorders();

	sep.multScalar(1.5);
	ali.multScalar(1.0);
	coh.multScalar(1.0);
	obs.multScalar(1.25);
	bor.multScalar(1.0);

	applyForce(sep);
	applyForce(coh);
	applyForce(ali);
	applyForce(obs);
	applyForce(bor);

	acceleration.multScalar(.4);
	speed.addVect(acceleration);
	speed.limit(max_speed);
	pos.addVect(speed);
	acceleration.multScalar(0);

	update_pos();
}
