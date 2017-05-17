#ifndef STATISTIC_H
#define STATISTIC_H

#include <stdio.h>
#include "image.h"

/* Main statistic function
 * calculates the brightness of the given image (has to be initialized!) 
 * and prints the statistics to the command line */
void statistic_print(struct image * img);

/* calculates and returns the brightness of a given pixel in an image 
 * expects initialized image and the coordinates for the pixel */
int pixel_brightness(struct image * img, int x, int y);

/* calculates and returns the brightness of a given image */
int image_brightness(struct image * img);

#endif
