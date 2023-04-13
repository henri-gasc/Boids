#include "Boids.hpp"
#include "config.hpp"
#include "threads.hpp"
#include "applications.hpp"
#include <filesystem>

int main(int argc, char **argv) {
	Config conf = handle_arguments(argc, argv);

	if (conf.save_to_file) {
		std::filesystem::create_directories(conf.save_dir);
	}

	Application app(&conf);
	app.window.setKeyRepeatEnabled(false);

	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_width(25, conf.window_width-25);
	RandomNumberGenerator rng_height(25, conf.window_height-25);
	// ThreadPool thread_pool(conf.nbr_threads);

	for (int i = 0; i < conf.nbr_boids; i++) {
		Boid *boid = new Boid(conf.window_width/2, conf.window_height/2, &rng, &conf);
		app.all_Boids.push_back(boid);
	}
	// all_Boids[0].shape.setRadius(5.f);
	// all_Boids[0].shape.setFillColor(sf::Color::Red);

	for (int i = 0; i < conf.nbr_obstacles; i++) {
		SimuObject *obst = new SimuObject(&conf, rng_width.pick(), rng_height.pick());
		app.draw(obst->shape);
		app.all_Obstacles.push_back(obst);
	}

	int counter = 0;
	app.save(counter);
	while (app.isRunning()) {
		counter++;

		for (size_t i = 0; i < app.all_Boids.size(); i++) {
			app.all_Boids.at(i).update(&app.all_Boids, &app.all_Obstacles);
			app.draw(app.all_Boids.at(i).shape);
			// if (i == 0) {
			// 	printf("\r%i, %i\n", all_Boids.at(i).grid_x, all_Boids.at(i).grid_y);
			// }
		}
		for (size_t i = 0; i < app.all_Obstacles.size(); i++) {
			app.draw(app.all_Obstacles[i].shape);
		}
		app.save(counter);

		if (app.boid_followed != NULL) {
			app.main_view.setCenter(sf::Vector2f(app.boid_followed->pos.x, app.boid_followed->pos.y));
		}
		app.window.setView(app.main_view);
		app.display();
	}
	return 0;
}
