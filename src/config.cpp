#include "config.hpp"
#include <getopt.h>
#include <bits/getopt_ext.h>

void printHelp(Config default_config) {
	printf("usage: boids [options]\n\n");
	printf("options:\n");
	printf("  -h, --help              Show this help\n\n");
	printf("  -n, --no-show           If set, will not show the simulation (but can still save)\n");
	printf("  -s, --save              If set, will save the simulations\n\n");
	printf("  -b, --boids     N       Choose the number of boids     [default=%4i]\n", default_config.nbr_boids);
	printf("  -o, --obstacles N       Choose the number of obstacles [default=%4i]\n", default_config.nbr_obstacles);
}

int verify(char *integer_to_convert, const char option_name[]) {
	int tmp = strtold(integer_to_convert, NULL);
	if (tmp > 0) {
		return tmp;
	}
	fprintf(stderr, "'%i' is not a valid value for option '%s'", tmp, option_name);
	exit(1);
}

Config handle_arguments(int argc, char **argv) {
	Config def;
	Config conf;
	int option_index = 0;
	int c;

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"save", no_argument, NULL, 's'},
		{"no-show", no_argument, NULL, 'n'},
		{"boids", required_argument, NULL, 'b'},
		{"obstacles", required_argument, NULL, 'O'},
	};

	while ((c = getopt_long(argc, argv, "hsnb:O:", long_options, &option_index)) != -1) {
		switch (c) {
		case 'b':
			conf.nbr_boids = verify(optarg, "--boids");
			break;
		case 'O':
			conf.nbr_obstacles = verify(optarg, "--obstacles");
			break;

		case 'h':
			printHelp(def);
			exit(0);
		case 's':
			conf.save_to_file = true;
			printf("Images will be stored in the images/ folder.\nYou can use a command like 'ffmpeg -framerate 30 -i images/part.%%05d.png movie.mp4' to generate a video\n");
			break;
		case 'n':
			conf.show_simulations = false;
			break;

		case '?':
			printHelp(def);
			exit(1);
		}
	}
	return conf;
}
