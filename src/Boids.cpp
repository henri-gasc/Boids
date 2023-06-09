#include "Boids.hpp"

/* We need this despite beeing the same function as in SimuObject
	because otherwise there is boid appearing on screen */
void Boid::update_pos() {
	apply_borders();
	shape.setRotation(get_angle());
	shape.setPosition(pos.x, pos.y);
	grid_x = (int) pos.x/vision_radius;
	grid_y = (int) pos.y/vision_radius;
}

void Boid::applyForce(Vect force) {
	acceleration.addVect(force);
}

Vect Boid::rulesBoid(boost::ptr_vector<Boid> *all_boids) {
	Vect sep(0, 0); // Separation
	Vect ali(0, 0); // Alignement
	Vect coh(0, 0); // Cohesion

	Vect avg_speed(0, 0);
	Vect avg_locat(0, 0);
	int count_sep = 0;
	int count_ali_coh = 0;
	for (size_t i = 0; i < all_boids->size(); i++) {
		int x = all_boids->at(i).grid_x;
		int y = all_boids->at(i).grid_y;
		if ((x < grid_x - 1) || (x > grid_x + 1) || (y < grid_y - 1) || (y > grid_y + 1)) {
			continue;
		}
		float d = distance(all_boids->at(i).pos);
		if (d <= 0) { continue; }
		if ((d < distance_boids) && (conf->rules & 1)) {
			Vect diff(0, 0);
			diff = diff.subTwo(pos, all_boids->at(i).pos);
			diff.normalize();
			diff.divScalar(d);
			sep.addVect(diff);
			count_sep++;
		}
		if (d < vision_radius) {
			avg_speed.addVect(all_boids->at(i).speed);
			avg_locat.addVect(all_boids->at(i).pos);
			count_ali_coh++;
		}
	}

	ali.setVect(avg_speed);

	if ((count_sep > 0) && (conf->rules & 1)) {
		sep.divScalar(count_sep);
		sep.normalize();
		sep.multScalar(max_speed);
		sep.subVect(speed);
		sep.limit(max_force);	
		}
	if (count_ali_coh > 0) {
		if (conf->rules & 2) {
			avg_speed.divScalar(count_ali_coh);
			avg_speed.normalize();
			avg_speed.multScalar(max_speed);
			ali = ali.subTwo(avg_speed, speed);
			ali.limit(max_force);
		}
		if (conf->rules & 4) {
			avg_locat.divScalar(count_ali_coh);
			coh.subTwo(avg_locat, pos);
			coh.normalize();
			coh.multScalar(max_speed);
			coh.subVect(speed);
			coh.limit(max_force);
		}
	}
	Vect sum(0, 0);
	sep.multScalar(1.5);
	ali.multScalar(1.0);
	coh.multScalar(1.0);
	sum.addVect(sep);
	sum.addVect(ali);
	sum.addVect(coh);
	return sum;
}

Vect Boid::AvoidObstacles(boost::ptr_vector<SimuObject> *all_obstacles) {
	Vect steering(0, 0);
	int count = 0;
	for (size_t i = 0; i < all_obstacles->size(); i++) {
		float d = distance(all_obstacles->at(i).pos);
		if ((d > 0) && (d < distance_obstacle)) {
			Vect diff(0, 0);
			diff = diff.subTwo(pos, all_obstacles->at(i).pos);
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

Vect Boid::AvoidVerticalBorders() {
	Vect steering(0, 0);
	float x = 0;
	float y = 0;
	if (pos.x > conf->window_width - border) {
		x = - conf->window_width - pos.x;
	}
	else if (pos.x < border) {
		x = pos.x;
	}
	if (pos.y > conf->window_height - border) {
		y = - conf->window_height - pos.y;
	}
	else if (pos.y < border) {
		y = pos.y;
	}
	steering.set(x, y);
	steering.limit(4*max_force);
	return steering;
}

void Boid::update(boost::ptr_vector<Boid> *all_boids, boost::ptr_vector<SimuObject> *all_obstacles) {

	// Separation, Aligment, and Cohesion
	applyForce(rulesBoid(all_boids));

	// Obstacles
	if (conf->rules & 8) {
		Vect obs = AvoidObstacles(all_obstacles);
		obs.multScalar(1.25);
		applyForce(obs);
	}

	// Borders
	if (conf->rules & 16) {
		Vect bor = AvoidVerticalBorders();
		bor.multScalar(1.0);
		applyForce(bor);
	}

	acceleration.multScalar(.4);
	speed.addVect(acceleration);
	speed.limit(max_speed);
	pos.addVect(speed);
	acceleration.multScalar(0);

	update_pos();
}
