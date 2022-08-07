#include "stage_lib.h"


/* start_pipline():     parameters-sateges_handler , bit is_pipe
 *                      action- PIPELINE_ON = is_pipe, active stages
 * stop_pipeline():    parameters- sateges_handler
 *                      action- shut PIPELINE_ON , finish stages
 */
void* create_handler_api(void* pipe_header){

    handler* my_handler=(handler*)malloc(sizeof (handler));
    my_handler->head=(stage*)pipe_header;
    my_handler->PIPELINE_ON=0;

    return my_handler;
}

void free_handler_api(void* my_handler){
    handler* handle=(handler*)my_handler;
    if(handle==NULL)
        return;
    free(handle);
}

void* create_stage_api(Queue* sourse, Queue* dest, function func, void* params ,stage* next){

    stage* my_stage=(stage*)malloc(sizeof(stage));
   snapshot_t*paramsss= (snapshot_t*)params;
    my_stage->destQu=dest;
    my_stage->sourseQu=sourse;
    my_stage->func=func;
    my_stage->next=next;
    my_stage->is_active=0;
    my_stage->params=params;
    return my_stage;
}

void free_stage_api(void* my_stage){

    if(my_stage==NULL)
        return;
    free((stage*)my_stage);
}
void free_stages_api(void* my_stage){
    stage *prev, *head=(stage*)my_stage;
    prev=head;
    while(prev!=NULL){
        head=prev->next;
        free(prev);
        prev=head;
    }

}
void free_stages_and_queues_api(void* my_stage){
    stage *prev, *head=(stage*)my_stage;
    prev=head;
    Queue* q;

    while(prev!=NULL){
        head=prev->next;
        q=prev->destQu;
        freeQueue(q);
        free(prev);
        prev=head;
    }

}


void stop_pipe_api(void* my_handler){

    handler* handle=((handler*)my_handler);
    handle->PIPELINE_ON=0;
    //finish stages threads
    stage* ptr=handle->head;
    while (ptr!=NULL) {
        pthread_join(ptr->thread,NULL);
        ptr=ptr->next;
    }

    stage_params *prev_params, *params=handle->params_head;
    //free stage_params
    prev_params=params;
    while (prev_params!=NULL) {
        params=prev_params->next;
        free(prev_params);
        prev_params=params;
    }

}

/*
 * since all thread have 'do while' loop,
 * means the first thread will create at least 2 matrix,
 * the others won't be stucked in the dequeue setion.
 * */
void* main_stage_thread_api(void* arg){

    printf("stage is action \n");
    bool* PIPELINE_ON=((stage_params*)arg)->PIPELINE_ON;
    stage* my_stage=((stage_params*)arg)->my_stage;


    Node *dest ,*n=(Node*)calloc(1,sizeof(Node));// last node to enter the pipeline
    void* data;

    if(my_stage->sourseQu!=NULL){
        freeNode(n);
        n=dequeue(my_stage->sourseQu);
    }

    do{
        data=my_stage->func(n->data, my_stage->params);
        if(my_stage->destQu!=NULL){
            dest=createNode(data);
            enqueu(my_stage->destQu,dest);
        }
        if(my_stage->sourseQu!=NULL){
            freeNode(n);
            n=dequeue(my_stage->sourseQu);
            my_stage->is_active=n->data!=NULL;
        }
        else{/*
              *for first thread
              *continue only if it's in the middle of a record
              */
            my_stage->is_active=*PIPELINE_ON;
        }
        sleep(2);
    }while(my_stage->is_active);


    if(my_stage->destQu!=NULL){
        enqueu(my_stage->destQu,n);
    }
    else{//last thread to pipeline only

        freeNode(n);
    }

}
void start_pipe_api(void* handle, int is_pipe){

    handler* my_handler=(handler*)handle;
    my_handler->PIPELINE_ON=is_pipe;
    stage* ptr=my_handler->head;
    stage_params *params;
    bool first=0;
    int i=0;

    while (ptr!=NULL) {
        i++;
        params=(stage_params*)malloc(sizeof(stage_params));
        params->PIPELINE_ON=&(my_handler->PIPELINE_ON);
        params->my_stage=ptr;
        params->next=NULL;

        pthread_create(&(ptr->thread),NULL,main_stage_thread_api,params);

        if(!first){
            my_handler->params_head=params;
            first=1;
        }

        ptr=ptr->next;
    }

    printf("%d stages are active\n",i);

}


stage_t stage_api={
    .init_rgb_matrix=init_rgb_matrix,
    .create_handler=create_handler_api,
    .free_handler=free_handler_api,
    .create_stage=create_stage_api,
    .free_stage=free_stage_api,
    .free_stages=free_stages_api,
    .free_stages_and_queues=free_stages_and_queues_api,
    .start_pipe=start_pipe_api,
    .stop_pipe=stop_pipe_api,
    .main_stage_thread=main_stage_thread_api
};

