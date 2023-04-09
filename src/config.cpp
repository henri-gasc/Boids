#include "config.hpp"

void printHelp() {
	Config default_config;
	printf("usage: boids [options]\n");
	printf("\n");
	printf("options:\n");
	printf("  -h, --help              Show this help\n");
	printf("\n");
	printf("  -n, --no-show           If set, will not show the simulation (but can still save)\n");
	printf("  -s, --save              If set, will save the simulations\n");
	printf("\n");
	printf("  -b, --boids     N       Choose the number of boids     [%4i]\n", default_config.nbr_boids);
	printf("  -O, --obstacles N       Choose the number of obstacles [%4i]\n", default_config.nbr_obstacles);
	printf("  -r, --rules     N       Choose the rules to activate   [%4i (all)]\n", default_config.rules);
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
		{"save", no_argument, NULL, 's'},
		{"no-show", no_argument, NULL, 'n'},
		{"boids", required_argument, NULL, 'b'},
		{"obstacles", required_argument, NULL, 'O'},
		{"rules", required_argument, NULL, 'r'},
	};

	while ((c = getopt_long(argc, argv, "hsnb:O:r:", long_options, &option_index)) != -1) {
		switch (c) {
		case 'b':
			conf.nbr_boids = verify(optarg, "--boids");
			break;
		case 'O':
			conf.nbr_obstacles = verify(optarg, "--obstacles");
			break;
		case 'r':
			conf.rules = strtold(optarg, NULL);
			printf("Separation: %i\n", R1ITE0(conf.rules & 1));
			printf("Aligment:   %i\n", R1ITE0(conf.rules & 2));
			printf("Cohesion:   %i\n", R1ITE0(conf.rules & 4));
			printf("Obstacles:  %i\n", R1ITE0(conf.rules & 8));
			printf("Borders:    %i\n", R1ITE0(conf.rules & 16));
			break;

		case 'h':
			printHelp();
			exit(0);
		case 's':
			conf.save_to_file = true;
			printf("Images will be stored in the images/ folder.\nYou can use a command like 'ffmpeg -framerate 30 -i images/part.%%05d.png movie.mp4' to generate a video\n");
			break;
		case 'n':
			conf.show_simulations = false;
			break;

		case '?':
			printHelp();
			exit(1);
		}
	}
	return conf;
}
