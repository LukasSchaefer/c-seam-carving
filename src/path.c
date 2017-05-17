#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "path.h"

int color_diff(struct image * img, int x1, int y1, int x2, int y2)
{
    if ((x1 < 0) || (x1 >= (*img).w) || (y1 < 0) || (y1 >= (*img).h) || (x2 < 0) || (x2 >= (*img).w) || (y2 < 0) || (y2 >= (*img).h) ) {
        printf("Invalid coordinates for color_diff call!\n");
        exit(EXIT_FAILURE);
    }
    int pixel1 = y1 * (*img).w + x1;
    int color1 = (*img).data[pixel1];
    int r1 = color1 >> 16;
    int g1 = (color1 & (65280)) >> 8;
    int b1 = color1 & (255); 

    int pixel2 = y2 * (*img).w + x2;
    int color2 = (*img).data[pixel2];
    int r2 = color2 >> 16;
    int g2 = (color2 & (65280)) >> 8;
    int b2 = color2 & (255); 

    int diff = (r1 - r2) * (r1 - r2) + (g1 - g2) * (g1 - g2) + (b1 - b2) * (b1 - b2);
    return diff;
}

int ** relative_energy_table(struct image * img, int num_deleted)
{
    int ** table = malloc(((*img).w - num_deleted) * sizeof(int*));
    for (int i = 0; i < ((*img).w - num_deleted); i++)
        table[i] = calloc((*img).h, sizeof(int));
    table[0][0] = 0;
    for (int i = 1; i < ((*img).w - num_deleted); i++) {                    // initialize the first row
        table[i][0] = color_diff(img, i, 0, i-1, 0);
    }

    for (int j = 1; j < (*img).h; j++) {                    // and the first column
        table[0][j] = color_diff(img, 0, j, 0, j-1);
    }

    for (int i = 1; i < ((*img).w - num_deleted); i++) {                    // fill the rest of the table
        for (int j = 1; j < (*img).h; j++) {
            table[i][j] = color_diff(img, i, j, i-1, j) + color_diff(img, i, j, i, j-1);
        }
    }
    return table;
}

int ** accumulate_energy_table(struct image * img, int num_deleted)
{
    int ** rel_table = relative_energy_table(img, num_deleted);
    int ** table = malloc(((*img).w - num_deleted) * sizeof(int*));
    for (int i = 0; i < ((*img).w - num_deleted); i++)
    {
        table[i] = calloc((*img).h, sizeof(int));
        table[i][0] = rel_table[i][0];
    }
    for (int j = 1; j < (*img).h; j++) {
        if (table[0][j-1] <= table[1][j-1])
            table[0][j] = table[0][j-1] + rel_table[0][j];
        else
            table[0][j] = table[1][j-1] + rel_table[0][j];
        for (int i = 1; i < ((*img).w - 1 - num_deleted); i++) {
            if (table[i-1][j-1] <= table[i+1][j-1])
                if (table[i-1][j-1] < table[i][j-1])
                    table[i][j] = table[i-1][j-1] + rel_table[i][j];
                else
                    table[i][j] = table[i][j-1] + rel_table[i][j];
            else
                if (table[i+1][j-1] < table[i][j-1])
                    table[i][j] = table[i+1][j-1] + rel_table[i][j];
                else
                    table[i][j] = table[i][j-1] + rel_table[i][j];
        }
        if (table[(*img).w - 1 - num_deleted][j-1] <= table[(*img).w - 2 - num_deleted][j-1])
            table[(*img).w - 1 - num_deleted][j] = table[(*img).w - 1 - num_deleted][j-1] + rel_table[(*img).w - 1 - num_deleted][j];
        else
            table[(*img).w - 1 - num_deleted][j] = table[(*img).w - 2 - num_deleted][j-1] + rel_table[(*img).w - 1 - num_deleted][j];
    }
    for (int i = 0; i < ((*img).w - num_deleted); i++) 
        free(rel_table[i]);
    free(rel_table);
    return table;
}

int * find_path(struct image * img, int num_deleted)
{
    int ** acc_table = accumulate_energy_table(img, num_deleted);
    int * path = calloc((*img).h, sizeof(int));
    int start = 0;
    int val = acc_table[0][(*img).h -1];
    for (int i = 1; i < ((*img).w - num_deleted); i++) {                    // find start of the best path (in the last row)
        if (acc_table[i][(*img).h - 1] < val) {
            start = i;
            val = acc_table[i][(*img).h - 1];
        }
    }
    path[0] = start;

    for (int i = 1; i < (*img).h; i++)
    {
        if (start == 0) {
            if (acc_table[0][(*img).h - 1 - i] <= acc_table[1][(*img).h - 1 - i]) {
                path[i] = 0;
                start = 0;
                continue;
            }
            else {
                path[i] = 1;
                start = 1;
                continue;
            }
        }
        else {
            if (start == (*img).w - 1 - num_deleted) {
                if (acc_table[(*img).w - 1 - num_deleted][(*img).h - 1 - i] <= acc_table[(*img).w - 2 - num_deleted][(*img).h - 1 - i]) {
                    path[i] = (*img).w - 1 - num_deleted;
                    start = (*img).w - 1 - num_deleted;
                    continue;
                }
                else {
                    path[i] = (*img).w - 2 - num_deleted;
                    start = (*img).w - 2 - num_deleted;
                    continue;
                }
            }
            else {
                if (acc_table[start][(*img).h - 1 - i] <= acc_table[start - 1][(*img).h - 1 - i]) {
                    if (acc_table[start][(*img).h - 1 - i] <= acc_table[start + 1][(*img).h - 1 - i]) {
                        path[i] = start;
                        start = start;
                        continue;
                    }
                    else {
                        path[i] = start + 1;
                        start = start + 1;
                        continue;
                    }
                }
                else {
                    if (acc_table[start - 1][(*img).h - 1 - i] <= acc_table[start + 1][(*img).h - 1 - i]) {
                        path[i] = start - 1;
                        start = start - 1;
                        continue;
                    }
                    else {
                        path[i] = start + 1;
                        start = start + 1;
                        continue;
                    }
                }
            }
        }
    }
    free(acc_table);
    return path;
}

void path_print(struct image * img, int num_deleted)
{
    int * path = find_path(img, num_deleted);
    for (int i = 0; i < (*img).h ; i++)
        printf("%d\n", path[i]);
}

void delete_path(struct image * img, int num_deleted)
{
    int * path = find_path(img, num_deleted);
    int start;
    for (int row = 0; row < (*img).h; row++) {
        start = path[((*img).h - 1) - row];
        for (int i = start; i < ((*img).w - 1); i++) {                  // - num_deleted
             int pos = row * (*img).w + i;
             (*img).data[pos] = (*img).data[pos + 1];
        }
        int last_pos =  row * (*img).w + (*img).w - 1 - num_deleted;
        (*img).data[last_pos] = 0;
    }
    free(path);
}
