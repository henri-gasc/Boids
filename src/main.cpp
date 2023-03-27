#include "Boids.hpp"
#include <filesystem>

int main(int argc, char **argv) {
	Config conf = handle_arguments(argc, argv);

	if (conf.save_to_file) {
		std::filesystem::create_directories("./images");
	}

	boost::ptr_vector<Boid> all_Boids(conf.nbr_boids);
	boost::ptr_vector<Obstacle> all_Obstacles(conf.nbr_obstacles);
	Application app(&conf);
	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_width(0, conf.window_width);
	RandomNumberGenerator rng_height(0, conf.window_height);

	for (int i = 0; i < conf.nbr_boids; i++) {
		Boid *boid = new Boid(conf.window_height/2, conf.window_width/2, &rng, &conf);
		all_Boids.push_back(boid);
	}

	for (int i = 0; i < conf.nbr_obstacles; i++) {
		Obstacle *obst = new Obstacle(&rng_width, &rng_height);
		app.draw(obst->shape);
		all_Obstacles.push_back(obst);
	}

	int counter = 0;
	app.save(counter);
	while (app.isRunning()) {
		for (int i = 0; i < conf.nbr_boids; i++) {
			Boid boid = all_Boids[i];
			boid.update(all_Boids, all_Obstacles);
			app.draw(boid.shape);
			all_Boids[i] = boid;
		}
		for (int i = 0; i < conf.nbr_obstacles; i++) {
			app.draw(all_Obstacles[i].shape);
		}
		app.save(counter);
		app.display();
		counter ++;
	}
	return 0;
}
