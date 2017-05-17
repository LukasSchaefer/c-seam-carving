#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>

#include "statistic.h"
#include "image.h"
#include "path.h"

void print_help()
{
    printf("usage: carve [-h] [-s] [-p] [-n <count>] <image file>\n");
    printf("IMPORTANT: Image file must be given if there is no -h option!\n");
    printf("all options:\n");
    printf("-h\tprints the usage-syntax just as all possible options available\n");
    printf("-s\tprints statistics about the image file (including width, height and brightness)\n");
    printf("-p\tprints the column-coordinates of the pixels which are part of the most efficient minimal path throughout the image\n");
    printf("-n <count>\texecutes <count> steps of the algorithm with the given image\n");
    printf("\n");
    printf("If there is only an image file given the algorithm will be executed until the image is fully processed.\n");
    printf("This program is designed and implemented inside the Seam-Carving-Project of the Programming II Lecture at the Saarland University lead by Prof. Dr. Sebastian Hack.\n");
}

int main(int const argc, char** const argv)
{
    if (argc > 7 || argc == 0) {
        printf("Invalid Syntax!\nSyntax: ./%s [-h] [-s] [-p] [-n <count>] <image_file>\n", argv[0]);
        print_help();
        exit(EXIT_FAILURE);
    }
    
    char * endptr;
    int opt;
    int steps;
    int help = 0;
    int statistic = 0;
    int path = 0;
    int step = 0;
    while ((opt = getopt(argc, argv, "hspn:")) != -1) {
        switch(opt) {
           case 'h':
                help = 1;
                break;
            case 's':
                statistic = 1;
                break;
            case 'p':
                path = 1;
                break;
            case 'n':
                steps = strtol(optarg, &endptr, 10);
                if (strcmp(endptr, "") != 0) {
                    printf("Option -n needs an operand! Was missing or invalid.\n");
                    exit(EXIT_FAILURE);
                }
                step = 1;
                break;
            case ':':
                printf("The option -n needs an operand!\n");
                print_help();
                exit(EXIT_FAILURE);
            case '?':
                printf("Invalid option given.\n");
                print_help();
                exit(EXIT_FAILURE);
        }
    }

    char * filepath = argv[argc - 1];
    FILE * f = fopen(filepath, "r");
    if (f == NULL) {
        printf("No file or invalid file-path given!\n");
        print_help();
        exit(EXIT_FAILURE);
    }
    struct image img;
    image_read_from_file(&img, f);
    fclose(f);

    if (help == 1) {
        print_help();
        image_destroy(&img);
        exit(EXIT_SUCCESS);
    }
    else {
        if (statistic == 1) {
            statistic_print(&img);
            image_destroy(&img);
            exit(EXIT_SUCCESS);
        }
        else {
            if (path == 1) {
                path_print(&img, 0);
                image_destroy(&img);
                exit(EXIT_SUCCESS);
            }
            else {
                if (step == 1) {
                    if (steps < -1) {
                        printf("Invalid Step counter! Option -n expects a number of steps (greater 0).\n");
                        print_help();
                        image_destroy(&img);
                        exit(EXIT_FAILURE);
                    } 
                    else 
                        if (steps == -1) {
                            for (int i = 0; i < img.w; i++)
                                delete_path(&img, i);
                            FILE * g = fopen("out.ppm", "w");
                            image_write_to_file(&img, g);
                            fclose(g);
                            image_destroy(&img);
                            exit(EXIT_SUCCESS);
                        }
                    for (int i = 0; i < steps; i++)
                        delete_path(&img, i);
                    FILE * g = fopen("out.ppm", "w");
                    image_write_to_file(&img, g);
                    fclose(g);
                    image_destroy(&img);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }
    for (int i = 0; i < img.w; i++)
        delete_path(&img, i);
    FILE * g = fopen("out.ppm", "w");
    image_write_to_file(&img, g);
    fclose(g);
    image_destroy(&img);
    exit(EXIT_SUCCESS);
}
