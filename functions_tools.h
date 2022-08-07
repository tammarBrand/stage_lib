#pragma once
#ifndef FUNCTIONS_TOOLS_H
#define FUNCTIONS_TOOLS_H
#define SNAPSHOT_HEIGHT 240
#define SNAPSHOT_WIDTH 360
#define MAX_TEMP 30
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum status_bits{
    INIT=1,
    PIPE_START=2,
    PIPE_STOP=4,
    SNAPSHOT=8,
   /* CAPTURE=16,
    RGB_CONVERTER=32,
    YUV_CONVERTER=64,
    ENCODE=128,
    WRITE_RECORD=256
    */
};



static char  rgb_per_temp[MAX_TEMP*3];

typedef struct yuv{
    char *y;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH];
    char *u;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    char *v;//[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
}YUV;

typedef struct ppm_image{
    int width;
    int height;
    char *data;
    size_t size;
    char * name;
} ppm_image;
typedef struct record_t{
    char* file_name;
    int codec;
    int width;
    int height;
    int fps;
}record_t;

typedef struct snapshot_t{
    bool is_snap_on;
    char* file_name;  //full path ?
    int width;
    int height;
    char* type;  //GPEG,PNG,ppm
    char *data;
}snapshot_t;

typedef struct rgb_params{
    snapshot_t* snap;
    record_t* record;
}rgb_params;

void init_rgb_matrix();
void randMat(int** matrix_temp, int height, int width);
void covert_to_rgb(char rgb_matrix[],int** matrix, int height, int width);
void convert_to_yuv(char rgb_matrix[],  YUV* yuv, int height, int width);
void free_matrix(int** m ,int height);
void free_rgb_matrix(char * m);
void free_yuv(YUV* yuv);

#endif // FUNCTIONS_TOOLS_H
