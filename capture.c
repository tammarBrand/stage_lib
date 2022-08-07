#include "capture.h"
#include "Queue.h"

void* capture(void* data, void* params){

    record_t* record=(record_t*)params;
    int** matrix;
    matrix=(int**)malloc(sizeof(int *)*record->height);
    for(int i=0;i<record->height;i++)  {
        matrix[i]=NULL;
        matrix[i]=(int *)malloc(sizeof(int)*record->width);
    }
    randMat(matrix,record->height,record->width);
    printf("capture %d\n",matrix[0][0]);
    return matrix;
}

