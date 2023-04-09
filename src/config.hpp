#pragma once

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
	int nbr_threads = 0;
	bool save_to_file = false;
	bool show_simulations = true;
	std::string save_dir = "images";
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
