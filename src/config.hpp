#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

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

Config handle_arguments(int argc, char **argv);
