#include "Boids.hpp"
#include "threads.hpp"
#include "utils.hpp"
#include <filesystem>

void update_boid(const int i, boost::ptr_vector<Boid> *boids, boost::ptr_vector<SimuObject> *obstacles, Application *app) {
	Boid boid = boids->at(i);
	boid.update(boids, obstacles);
	app->draw(boid.shape);
	boids->at(i) = boid;
	// if (i == 0) {
	// 	printf("%f\n", boid.pos.x);
	// }
}

int main(int argc, char **argv) {
	Config conf = handle_arguments(argc, argv);

	if (conf.save_to_file) {
		std::filesystem::create_directories(conf.save_dir);
	}

	boost::ptr_vector<Boid> all_Boids(conf.nbr_boids);
	boost::ptr_vector<SimuObject> all_Obstacles(conf.nbr_obstacles);
	Application app(&conf);
	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_width(25, conf.window_width-25);
	RandomNumberGenerator rng_height(25, conf.window_height-25);
	ThreadPool thread_pool(conf.nbr_threads);

	for (int i = 0; i < conf.nbr_boids; i++) {
		Boid *boid = new Boid(conf.window_width/2, conf.window_height/2, &rng, &conf);
		all_Boids.push_back(boid);
	}
	// all_Boids[0].shape.setRadius(5.f);
	// all_Boids[0].shape.setFillColor(sf::Color::Red);

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
			all_Boids.at(i).update(&all_Boids, &all_Obstacles);
			// thread_pool.addTask([i, &all_Boids, &all_Obstacles] {
			// 	all_Boids.at(i).update(&all_Boids, &all_Obstacles);
			// });
		}
		thread_pool.waitForCompletion();
		for (int i = 0; i < conf.nbr_boids; i++) {
			app.draw(all_Boids.at(i).shape);
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
