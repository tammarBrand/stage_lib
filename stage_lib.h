#ifndef STAGE_LIB_H
#define STAGE_LIB_H

#pragma once
#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "functions_tools.h"



struct handler;

typedef void* (*function)(void* data, void* handler);

typedef struct stage{
    function func;
    pthread_t thread;
    Queue* sourseQu;
    Queue* destQu;
    bool is_active;
    struct stage* next;
    void* params;
}stage;
struct stage_params;
typedef struct handler{
    stage * head;
    struct stage_params* params_head;
    bool PIPELINE_ON;

}handler;

typedef struct stage_params{
    stage* my_stage;
    bool* PIPELINE_ON;
    struct stage_params *next;
}stage_params;


/*
 * stage:                   sourse_queue, dest_queue, action_func, params
 * sateges_handler:  status_bits{PIPELINE_ON}, stage* head_stage
 * start_pipline():     parameters-sateges_handler , bit is_pipe
 *                             action- PIPELINE_ON = is_pipe, active stages
 * stop_pipeline():    parameters- sateges_handler
 *                              action- shut PIPELINE_ON , finish stages
 *
 *
 * capture():            parameters- NULL , NULL
 *                   	        action- do  { at least one capture} while PIPELINE_ON
 * rgb_converter(): parameters- data,  snapshot_t* .
 *                             action- rgb_converter, if snapshot_t->is_active :
 *                                              { save_snapshot(snapshot_t) , snapshot_t->is_active=0 }
 * yuv_converter(): parameters- data, NULL
 *                             action- yuv_converter
 * encode():            parameters- data, NULL
 *                            action- encode
 * write_record():  parameters- data, record_t
 *                            action- write_record
 *
 *
 *
 * gas_cam_handler: status_bits{ RECORD_START, RECORD_STOP, DO_SNAPSHOT},
 *                 	           snapshot_struct , record_struct.
 *
 * start_record: light RECORD_START ,create fitten stages(5) and queues(4)
 *                        and start_pipeline(1).
 *
 * stop_record:  light RECORD_STOP , stop pipeline and free stages and queues,
 *                       shut RECORD_START, shut RECORD_STOP.
 *
 * do_snapshot:  action- light DO_SNAPSHOT ,
 *                        if RECORD_START:
 *                            light gas_cam_handler->snapshot_t->is_active
 *                        else
 *                            create fitten stages(2) and queues(1) , start pipeline(0) , stop pipeline.
 *                       shut DO_SNAPSHOT
 *
 */

/*
 * status_bits in the gas_cam_lib are needed for another component called 'controller' we created,
 * wich lights physical flashes according to the status_bit
 */

/*                      ??????????????????????????????????????????????
 * should save/write will be part of the stages.
 * yes-> they shouls know the definitions of the snap or record
 * no->  they will return value
 */


//TODO: change capture parameter to record_t and all the rest


typedef struct stage_t{
    void (*init_rgb_matrix)();
    void* (*create_handler)(void* pipe_header);
    void (*free_handler)(void* my_handler);
    void* (*create_stage)(Queue* sourse, Queue* dest, function func, void* params ,stage* next);
    void (*free_stage)(void* my_stage);
    void (*free_stages)(void* my_stage);
    void (*free_stages_and_queues)(void* my_stage);
    void (*start_pipe)(void* my_handler, int is_pipe);
    void (*stop_pipe)(void* my_handler);
    void* (*main_stage_thread)(void* arg);

}stage_t;
/*
void* create_handler(void* pipe_header);
void free_handler(void* my_handler);
void* create_stage(Queue* sourse, Queue* dest, function func, void* params ,stage* next);
void free_stage(void* my_stage);
void free_stages(void* my_stage);
void free_stages_and_queues(void* my_stage);
void start_pipe(void* my_handler, int is_pipe);
void stop_pipe(void* my_handler);
void* main_stage_thread(void* arg);
*/

#endif // STAGE_LIB_H
