#include "Boids.hpp"
#include "config.hpp"
#include "threads.hpp"
#include "utils.hpp"
#include <filesystem>

void toggleRules(int *rules, int repr) {
	*rules ^= repr;
	printRules(*rules);
}

int main(int argc, char **argv) {
	Config conf = handle_arguments(argc, argv);

	if (conf.save_to_file) {
		std::filesystem::create_directories(conf.save_dir);
	}

	boost::ptr_vector<Boid> all_Boids(conf.nbr_boids);
	boost::ptr_vector<SimuObject> all_Obstacles(conf.nbr_obstacles);
	Application app(&conf);
	app.window.setKeyRepeatEnabled(false);

	RandomNumberGenerator rng(-100, 100);
	RandomNumberGenerator rng_null(0, 0);
	RandomNumberGenerator rng_width(25, conf.window_width-25);
	RandomNumberGenerator rng_height(25, conf.window_height-25);
	// ThreadPool thread_pool(conf.nbr_threads);

	for (int i = 0; i < conf.nbr_boids; i++) {
		Boid *boid = new Boid(conf.window_width/2, conf.window_height/2, &rng, &conf);
		all_Boids.push_back(boid);
	}
	// all_Boids[0].shape.setRadius(5.f);
	// all_Boids[0].shape.setFillColor(sf::Color::Red);

	for (int i = 0; i < conf.nbr_obstacles; i++) {
		SimuObject *obst = new SimuObject(&conf, rng_width.pick(), rng_height.pick());
		app.draw(obst->shape);
		all_Obstacles.push_back(obst);
	}

	int counter = 0;
	app.save(counter);
	while (app.isRunning()) {
		counter++;

		sf::Event event;
		sf::Mouse mouse;
		sf::Vector2i mp = mouse.getPosition(app.window);
		bool ctrl = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			ctrl = true;
		}

		while (app.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Q)
				app.window.close();
			if (event.type != sf::Event::KeyPressed) {
				continue;
			}
			if (ctrl) {
				if (event.key.code == sf::Keyboard::S) {
					toggleRules(&conf.rules, 1);
				}
				else if (event.key.code == sf::Keyboard::A) {
					toggleRules(&conf.rules, 2);
				}
				else if (event.key.code == sf::Keyboard::C) {
					toggleRules(&conf.rules, 4);
				}
				else if (event.key.code == sf::Keyboard::O) {
					toggleRules(&conf.rules, 8);
				}
				else if (event.key.code == sf::Keyboard::B) {
					toggleRules(&conf.rules, 16);
				}
			}
			else if (event.key.code == sf::Keyboard::O) {
				SimuObject *o = new SimuObject(&conf, mp.x, mp.y);
				all_Obstacles.push_back(o);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && (not ctrl)) {
			Boid *b = new Boid(mp.x, mp.y, &rng_null, &conf);
			all_Boids.push_back(b);
		}

		for (size_t i = 0; i < all_Boids.size(); i++) {
			all_Boids.at(i).update(&all_Boids, &all_Obstacles);
			app.draw(all_Boids.at(i).shape);
			// if (i == 0) {
			// 	printf("\r%i, %i\n", all_Boids.at(i).grid_x, all_Boids.at(i).grid_y);
			// }
		}
		for (size_t i = 0; i < all_Obstacles.size(); i++) {
			app.draw(all_Obstacles[i].shape);
		}
		app.save(counter);
		app.display();
	}
	return 0;
}
