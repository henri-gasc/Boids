#pragma once

#include "Boids.hpp"
#include "utils.hpp"

inline void toggleRules(int *rules, int repr) {
	*rules ^= repr;
	printRules(*rules);
}

/**
 * @brief Class wrapper for the screen
 * 
 */
class Application {
public:
	sf::RenderWindow window;
	sf::RenderTexture texture;
	sf::View main_view;
	sf::Vector2f center;
	RandomNumberGenerator rng_null;
	Config *conf;
	boost::ptr_vector<Boid> all_Boids;
	boost::ptr_vector<SimuObject> all_Obstacles;
	Boid *boid_followed = NULL;

	Application(Config *config):
		window(sf::VideoMode(config->window_width, config->window_height), "SFML", sf::Style::Default),
        center((float) config->window_width/2, (float) config->window_height/2),
		rng_null(),
		all_Boids(config->nbr_boids),
		all_Obstacles(config->nbr_obstacles)
	{
		conf = config;
		main_view.reset(sf::FloatRect(0.f, 0.f, conf->window_width, conf->window_height));
		window.setView(main_view);
		if (conf->save_to_file) {
			texture.create(conf->window_width, conf->window_height);
		}
		window.setFramerateLimit(conf->framerate);
	}

	void handleEvents() {
		sf::Event event;
		sf::Mouse mouse;
		sf::Vector2i mp = mouse.getPosition(window);
		bool ctrl = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			ctrl = true;
		}

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Q)
				window.close();
		    if (event.type == sf::Event::Resized) {
		        sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
        		window.setView(sf::View(visibleArea));
			}
			if (event.key.code == sf::Keyboard::F) {
				if (event.type == sf::Event::KeyReleased) {
					boid_followed->shape.setFillColor(conf->color_normal_boid);
					boid_followed->shape.setRadius(boid_followed->r);
					boid_followed = NULL;
					main_view.zoom(2.f);
					main_view.setCenter(center);
				}
				else {
					SimuObject click(conf, mp.x, mp.y);
					float d = conf->window_height;
					for (size_t i = 0; i < all_Boids.size(); i++) {
						float a = click.distance(all_Boids.at(i).pos);
						if (d >= a) {
							d = a;
							boid_followed = &all_Boids.at(i);
						}
					}
					boid_followed->shape.setFillColor(conf->color_boid_followed);
					boid_followed->shape.setRadius(2*boid_followed->r);
					main_view.zoom(0.5);
				}
			}
			if (event.type != sf::Event::KeyPressed) {
				continue;
			}
			if (ctrl) {
				if (event.key.code == sf::Keyboard::S) {
					toggleRules(&conf->rules, 1);
				}
				else if (event.key.code == sf::Keyboard::A) {
					toggleRules(&conf->rules, 2);
				}
				else if (event.key.code == sf::Keyboard::C) {
					toggleRules(&conf->rules, 4);
				}
				else if (event.key.code == sf::Keyboard::O) {
					toggleRules(&conf->rules, 8);
				}
				else if (event.key.code == sf::Keyboard::B) {
					toggleRules(&conf->rules, 16);
				}
			}
			else if (event.key.code == sf::Keyboard::O) {
				SimuObject *o = new SimuObject(conf, mp.x, mp.y);
				all_Obstacles.push_back(o);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && (not ctrl)) {
			Boid *b = new Boid(mp.x, mp.y, &rng_null, conf);
			all_Boids.push_back(b);
		}
	}

	/**
	 * @brief Test if the app is running, apply some events too
	 * 
	 * @return true if the app if running
	 * @return false if the app is not running
	 */
	bool isRunning() {
		handleEvents();
		window.clear();
		texture.clear();
		return window.isOpen();
	}

	/**
	 * @brief Draw the object on the screen
	 * 
	 * @param drawable The object to draw
	 */
	void draw(const sf::Drawable & drawable) {
		if (conf->show_simulations) {
			window.draw(drawable);
		}
		if (conf->save_to_file) {
			texture.draw(drawable);
		}
	}

	/**
	 * @brief Wrapper around the RenderWindow class
	 * 
	 */
	void display() {
		window.display();
	}

	/**
	 * @brief Save the current frame of the simulation to disk
	 * 
	 * @param number The number of the frame
	 */
	void save(int number) {
		if (not conf->save_to_file) return;
		char name[100] = "";
		strcat(name, conf->save_dir.c_str());
		strcat(name, "/part.");
		char format[6] = ".%05i";
		sprintf(strstr(name, "."), format, number);
		strcat(name, ".png");
		texture.getTexture().copyToImage().saveToFile(name);
		printf("Saved frame %i\n", number);
	}
};
