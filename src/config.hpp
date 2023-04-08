#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>

/**
 * @brief The class with the config in it
 * 
 */
class Config {
public:
	int window_height = 1500;
	int window_width = 1000;
	int framerate = 40;
	int nbr_boids = 200;
	int nbr_obstacles = 0;
	bool save_to_file = false;
	bool show_simulations = true;
};

/**
 * @brief Simple function to print the help
 * 
 */
void printHelp();

/**
 * @brief Study the arguments and change the configuration accordingly
 * 
 * @param argc The number of arguments
 * @param argv The list of arguments
 * @return The updated config
 */

Config handle_arguments(int argc, char **argv);
