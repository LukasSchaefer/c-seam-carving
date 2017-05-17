#ifndef PATH_H
#define PATH_H 

#include <stdio.h>
#include "image.h"

/* calculates and returns the color difference of two pixels used for relativ energy */
int color_diff(struct image * img, int x1, int y1, int x2, int y2);

/* calculates the relative energy for every pixel in img 
 * returns img.width * img.height 2-dimensional array with relative energy values*/
int ** relative_energy_table(struct image * img, int num_deleted);

/* calculates the accumulate energy for every pixel in img
 * and returns just like relative_energy_table a 2-dimensional array with those values */ 
int ** accumulate_energy_table(struct image * img, int num_deleted);

/* finds the most energy - efficient path through the image to delete
 * without loosing much information (based on accumulate energy values) */
int * find_path(struct image * img, int num_deleted);

/* prints the with find_path caluclated path out in the terminal */
void path_print(struct image * img, int num_deleted);

/* deletes the most - efficient path out of the img's data field */
void delete_path(struct image * img, int num_deleted);

#endif
