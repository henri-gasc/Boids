#pragma once

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <getopt.h>
#include <bits/getopt_ext.h>
#include <string>

/**
 * @brief The class with the config in it
 * 
 */
class Config {
public:
	int window_width = 1500;
	int window_height = 1000;
	int framerate = 40;
	int nbr_boids = 200;
	int nbr_obstacles = 0;
	int nbr_rules = 5;
	int rules = 0b11111; // See help
	int nbr_threads = 10;
	bool save_to_file = false;
	bool show_simulations = true;
	std::string save_dir = "images";
	sf::Color color_normal_boid = sf::Color::White;
	sf::Color color_boid_followed = sf::Color::Magenta;
	sf::Color color_object = sf::Color::Red;
};

/**
 * @brief Simple function to print the help
 * 
 */
void printHelp();

/**
 * @brief Print the rules and their status
 * 
 * @param conf The config
 */
void printRules(int rules);

/**
 * @brief Study the arguments and change the configuration accordingly
 * 
 * @param argc The number of arguments
 * @param argv The list of arguments
 * @return The updated config
 */

Config handle_arguments(int argc, char **argv);
