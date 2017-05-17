#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>

struct image {
    int w, h;
    int * data;
};

/* initialize image with set width w, and height h
 * we also allocate a data segment for w*h ints for the pixels
 * which are represented by their RGB values (value between 0 and 255), each 8 bits long
 * red: 16-23 bits, green: 8-15 bits, blue: 0-7 bits
 */
void image_init(struct image* img, int w, int h);

/* destroy the image by freeing the data segment
 */
void image_free(struct image* img);

/* reads image from ppm-file.
 * Sets the width and height in w, h of img and fills the data segment with color-values
 */
void image_read_from_file(struct image* img, FILE * f);

void image_write_to_file(struct image * img, FILE * f);

#endif
