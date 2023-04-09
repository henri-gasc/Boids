#include "Boids.hpp"
#include <filesystem>

int main(int argc, char **argv) {
	Config conf = handle_arguments(argc, argv);

	if (conf.save_to_file) {
		std::filesystem::create_directories("./images");
	}

	boost::ptr_vector<Boid> all_Boids(conf.nbr_boids);
	boost::ptr_vector<SimuObject> all_Obstacles(conf.nbr_obstacles);
	Application app(&conf);
	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_width(10, conf.window_width-10);
	RandomNumberGenerator rng_height(10, conf.window_height-10);

	for (int i = 0; i < conf.nbr_boids; i++) {
		Boid *boid = new Boid(conf.window_height/2, conf.window_width/2, &rng, &conf);
		all_Boids.push_back(boid);
	}

	for (int i = 0; i < conf.nbr_obstacles; i++) {
		SimuObject *obst = new SimuObject(&conf);
		obst->pos.set(rng_width.pick(), rng_height.pick());
		obst->update_pos();
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
