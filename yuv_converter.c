#include "yuv_converter.h"

void* yuv_converter(void* data, void* params){

    printf("yuv\n");

    record_t* record=(record_t*)params;
    char*rgb_matrix=(char*)data;

    /*    char y[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH];
    char u[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    char v[SNAPSHOT_HEIGHT*SNAPSHOT_WIDTH/4];
    */
    YUV * yuv=(YUV*)malloc(sizeof(YUV));
    yuv->y=(char*)malloc(record->width*record->height);
    yuv->u=(char*)malloc(record->width*record->height/4);
    yuv->v=(char*)malloc(record->width*record->height/4);
    convert_to_yuv(rgb_matrix,yuv, record->height, record->width);
    free_rgb_matrix(rgb_matrix);

    return yuv;


}
