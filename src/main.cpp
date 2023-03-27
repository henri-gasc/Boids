#include "Boids.hpp"
#include "Obstacle.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <cstdio>
#include <filesystem>

int main() {
	const int nbr_boids = 200;
	const int nbr_obstacles = 0;
	const bool save_to_file = true;
	if (save_to_file) {
		std::filesystem::create_directories("./images");
	}

	boost::ptr_vector<Boid> all_Boids(nbr_boids);
	boost::ptr_vector<Obstacle> all_Obstacles(nbr_obstacles);
	WindowHandler app;
	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_width(0, WINDOW_WIDTH);
	RandomNumberGenerator rng_height(0, WINDOW_HEIGHT);

	for (int i = 0; i < nbr_boids; i++) {
		Boid *boid = new Boid(WINDOW_HEIGHT/2, WINDOW_WIDTH/2, &rng);
		all_Boids.push_back(boid);
	}

	for (int i = 0; i < nbr_obstacles; i++) {
		Obstacle *obst = new Obstacle(&rng_width, &rng_height);
		app.draw(obst->shape);
		all_Obstacles.push_back(obst);
	}

	int counter = 0;
	if (save_to_file) {
		app.save(counter);
	}
	while (app.isRunning()) {
		for (int i = 0; i < nbr_boids; i++) {
			Boid boid = all_Boids[i];
			boid.update(all_Boids, all_Obstacles);
			app.draw(boid.shape);
			all_Boids[i] = boid;
		}
		for (int i = 0; i < nbr_obstacles; i++) {
			app.draw(all_Obstacles[i].shape);
		}
		if (save_to_file) {
			app.save(counter);
		}
		app.display();
		counter ++;
	}
	return 0;
}
