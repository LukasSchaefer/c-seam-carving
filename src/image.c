#include <stdlib.h>
#include <string.h>

#include "image.h"

void image_init(struct image* img, int w, int h)
{
	if ((w < 1) || (h < 1)) {
		fprintf(stderr, "Invalid width or height for the image in image_init-call! Must both be at least 1\n");
		exit(EXIT_FAILURE);
	}
	(*img).w = w;
	(*img).h = h;
	int size = w * h;
	int * imagedata = calloc(size, sizeof(imagedata[0]));
	(*img).data = imagedata;
}

void image_destroy(struct image* img)
{
	free((*img).data);
}

void image_read_from_file(struct image* img, FILE * f)
{
    int w, h, max_val;
    if (fscanf(f, "P3\n%d %d\n%d\n", &w, &h, &max_val) <= 0)
    {
        printf("File does not match the expected format.\n");
        exit(EXIT_FAILURE);
    }
    if (w <= 0 || h <= 0) {
        printf("Invalid width and height given in file!\n");
        exit(EXIT_FAILURE);
    }
    image_init(img, w, h);
    for (int i = 0; i < (h - 1); i++)
    {
        for (int j = 0; j < w; j++)
        {
            int r, g, b;
            if (fscanf(f, "%d %d %d ", &r, &g, &b) < 1)
            {
                printf("Missing color values! Invalid syntax.\n");
                exit(EXIT_FAILURE);
            }
            if ((r > max_val) || (g > max_val) || (b > max_val) || (r < 0) || (g < 0) || (b < 0)) {
                printf("Color-value invalid!\n");
                exit(EXIT_FAILURE);
            }
            int pixel = i * (*img).w + j;
            int color = ((((r << 8) + g) << 8) + b);
            (*img).data[pixel] = color;
        }
    }
    for (int k = 0; k < w; k++)
    {
        int r, g, b;
        if (fscanf(f, "%d %d %d ", &r, &g, &b) < 3)
        {
            printf("Missing color values! Invalid syntax.\n");
            exit(EXIT_FAILURE);
        }
        if ((r > max_val) || (g > max_val) || (b > max_val) || (r < 0) || (g < 0) || (b < 0)) {
            printf("Color-value invalid!\n");
            exit(EXIT_FAILURE);
        }
        int pixel = ((*img).h - 1) * (*img).w + k;
        int color = ((((r << 8) + g) << 8) + b);
        (*img).data[pixel] = color;
    }
    int t;
    if (fscanf(f, "\n") >= 1 || fscanf(f,"%d", &t) >= 1)
    {
        printf("File is longer than expected!\n");
        exit(EXIT_FAILURE);
    }
}

void image_write_to_file(struct image* img, FILE* f)
{
    if (fprintf(f,"P3\n%d %d\n255\n", (*img).w, (*img).h) < 1)
        return;
    for (int row = 0; row < (*img).h; row++){
        for (int line = 0; line < (*img).w; line++){
            int color = (*img).data[(row * (*img).w + line)];
            int r = color >> 16;
            int g = (color & (65280)) >> 8;
            int b = color & (255);
            fprintf(f, "%3d %3d %3d ", r, g, b);
        }
    fprintf(f,"\n");
    }
    return;
}
