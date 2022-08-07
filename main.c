#include <stdio.h>
#include <pthread.h>

#include "stage_lib.h"
#include "rgb_converter.h"


#include "yuv_converter.h"
#include "encode.h"
#include "write_record.h"
#include "capture.h"
#include "functions_tools.h"

void main(int argc, char *argv[])
{

    //init function

    init_rgb_matrix();



   //start_record function
    snapshot_t *snap=(snapshot_t *)malloc(sizeof(snapshot_t));

    snap->file_name="img.jpg";
    snap->height=240;
    snap->width=360;
    snap->is_snap_on=0;
    snap->type=strdup("jpg");

    record_t* record=(record_t*)malloc(sizeof(record_t));
    record->height=240;
    record->width=360;
    record->codec=1;
    record->file_name="record";
    record->fps=1;

    Queue* q1=createQueue(10);
    Queue* q2=createQueue(10);
    Queue* q3=createQueue(10);
    Queue* q4=createQueue(10);

    rgb_params * rgb_p=(rgb_params *)malloc(sizeof(rgb_params));
    rgb_p->record=record;
    rgb_p->snap=snap;

    stage* st5=create_stage(q4,NULL,write_record,(void*)record,NULL);
    stage* st4=create_stage(q3,q4,encode,(void*)record,st5);
    stage* st3=create_stage(q2,q3,yuv_converter,(void*)record,st4);
    stage* st2=create_stage(q1,q2,rgb_converter,(void*)rgb_p,st3);
    stage* st1=create_stage(NULL,q1,capture,(void*)record,st2);
    printf("%d\n",st1->sourseQu==NULL);


    void* my_handler=create_handler(st1);
    start_pipe(my_handler,1);

    sleep(3);
    //do_snapshot function
    snap->is_snap_on=1;
    sleep(5);

    //stop_record function
    stop_pipe(my_handler);


    free_stages_and_queues(st1);

    //free_handler(my_handler);


}
