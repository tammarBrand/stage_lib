#include "write_record.h"

void* write_record(void* data, void* params){

    printf("write\n");

    YUV* yuv=(YUV*)data;
    free_yuv(yuv);
    return yuv;


}
