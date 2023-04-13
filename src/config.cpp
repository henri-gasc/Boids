#include "config.hpp"
#include <bits/getopt_core.h>

void printHelp() {
	Config default_config;
	printf("usage: boids [options]\n");
	printf("\n");
	printf("options:\n");
	printf("  -h, --help                Show this help\n");
	printf("\n");
	printf("  -n, --no-show             If set, will not show the simulation (but can still save)\n");
	printf("  -s, --save [images_dir]   If set, will save the simulations\n");
	printf("              Specify where the images will be saved [%s]\n", default_config.save_dir.c_str());
	printf("\n");
	printf("  -b, --boids     N         Choose the number of boids     [%4i]\n", default_config.nbr_boids);
	printf("  -O, --obstacles N         Choose the number of obstacles [%4i]\n", default_config.nbr_obstacles);
	printf("  -r, --rules     N         Choose the rules to activate   [%4i (all)]\n", default_config.rules);
	printf("        +1 separation, +2 aligment, +4 cohesion, +8 obstacles, +16 borders\n");
}

int R1ITE0(const bool cond) {
	if (cond) {
		return 1;
	}
	else {
		return 0;
	}
}

void printRules(int rules) {
	printf("Current rules:\n");
	printf("  Separation: %i\n", R1ITE0(rules & 1));
	printf("  Aligment:   %i\n", R1ITE0(rules & 2));
	printf("  Cohesion:   %i\n", R1ITE0(rules & 4));
	printf("  Obstacles:  %i\n", R1ITE0(rules & 8));
	printf("  Borders:    %i\n", R1ITE0(rules & 16));
	printf("\n");
}

/**
 * @brief Make sure the argument is a number greater than 1
 * 
 * @param integer_to_convert The string to convert to int
 * @param option_name The name of the option (used when errors)
 * @return The number converted
 */
int verify(char *integer_to_convert, const char option_name[]) {
	int tmp = strtold(integer_to_convert, NULL);
	if (tmp > 0) {
		return tmp;
	}
	fprintf(stderr, "'%i' is not a valid value for option '%s'", tmp, option_name);
	exit(1);
}

Config handle_arguments(int argc, char **argv) {
	Config conf;
	int option_index = 0;
	int c;

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"no-show", no_argument, NULL, 'n'},
		{"save", optional_argument, NULL, 's'},
		{"boids", required_argument, NULL, 'b'},
		{"obstacles", required_argument, NULL, 'O'},
		{"rules", required_argument, NULL, 'r'},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "hns::b:O:r:", long_options, &option_index)) != -1) {
		switch (c) {
		case 'b':
			conf.nbr_boids = verify(optarg, "--boids");
			break;
		case 'O':
			conf.nbr_obstacles = verify(optarg, "--obstacles");
			break;
		case 'r':
			conf.rules = strtold(optarg, NULL);
			break;

		case 's':
			conf.save_to_file = true;
			if (optarg == NULL && argv[optind] != NULL && argv[optind][0] != '-') {
				conf.save_dir = argv[optind];
				++optind;
			}
			else if (optarg != NULL) {
				conf.save_dir = optarg;
			}
			printf("Images will be stored in '%s'.\nYou can use a command like 'ffmpeg -framerate 30 -i %s/part.%%05d.png movie.mp4' to generate a video\n", conf.save_dir.c_str(), conf.save_dir.c_str());
			break;

		case 'h':
			printHelp();
			exit(0);
		case 'n':
			conf.show_simulations = false;
			break;

		case '?':
			printHelp();
			exit(1);
		}
	}
	printRules(conf.rules);
	return conf;
}
