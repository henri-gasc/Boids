#include "config.hpp"
#include <cstdio>

Config handle_arguments(int argc, char **argv) {
    Config conf;
    int i = 1;
    while (i < argc) {
        // fprintf(stdout, "argv[%i] = %s\n", i, argv[i]);
        if (strcmp(argv[i], "--framerate") == 0) {
            conf.framerate = *argv[i] - '0';
            i++;
        }
        else if (strcmp(argv[i], "--save") == 0) {
            conf.save_to_file = true;
            printf("Images will be stored in the images/ folder.\nYou can use a command like 'ffmpeg -framerate 30 -i images/part.%%05d.png movie.mp4' to generate a video\n");
        }
        else if (strcmp(argv[i], "--no-show") == 0) {
            conf.show_simulations = false;
        }
        i ++;
    }
    return conf;
}
