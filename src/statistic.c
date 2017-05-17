#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "statistic.h"
#include "path.h"

int pixel_brigthness(struct image * img, int x, int y) 
{
    if ((x < 0) || (y < 0) || (x >= (*img).w) || (y >= (*img).h)) { 
        printf("Invalid pixel-coordinates for pixel_brigthness call.\n");
        exit(EXIT_FAILURE);
    }
    int pixel = y * (*img).w + x;
    int color = (*img).data[pixel];
    int r = color >> 16;
    int g = (color & (65280)) >> 8;
    int b = color & (255); 
    int pix_brightness = ((r + g + b) /3);
    return pix_brightness;
}

int image_brightness(struct image * img)
{
    int sum = 0;

    for (int y = 0; y < (*img).h; y++) {
        for (int x = 0; x < (*img).w; x++){
            sum += pixel_brigthness(img, x, y);
        }
    }

    int pix_number = (*img).h * (*img).w;
    int pic_brightness = (sum /pix_number);
    return pic_brightness;
}

void statistic_print(struct image * img) 
{
    int pic_brightness = image_brightness(img);
    printf("width: %u\n", (*img).w);
    printf("height: %u\n", (*img).h);
    printf("brightness: %u\n", pic_brightness);
}
