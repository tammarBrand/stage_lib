#include "rgb_converter.h"


void* rgb_converter(void* data, void* params){

    printf("rgb\n");
    record_t* record=((rgb_params*)params)->record;
    snapshot_t* snap=((rgb_params*)params)->snap;

    int** matrix=(int**)data;
    char *rgb_matrix=NULL;
    printf("rgb \n");
    rgb_matrix=(char *)calloc(sizeof(char)*(record->height*record->width*3),sizeof(char));
    covert_to_rgb(rgb_matrix,matrix, record->height, record->width);
    free_matrix(matrix, record->height);

    if(snap->is_snap_on)
        save_snapshot(snap,rgb_matrix);

    return rgb_matrix;

}




