#include "stage_lib.h"


void* main_stage_thread_api(void* arg);

void* create_handler_api(void* pipe_header){

    stage_handler* my_handler=(stage_handler*)malloc(sizeof (stage_handler));
    my_handler->head=(stage*)pipe_header;
    my_handler->PIPELINE_ON=0;
    return my_handler;
}
void free_handler_api(void* my_handler){
    stage_handler* handle=(stage_handler*)my_handler;
    if(handle==NULL)
        return;
    free(handle);
}
void* create_stage_api(Queue* sourse, Queue* dest, function func, void* params ,stage* next){

    stage* my_stage=(stage*)malloc(sizeof(stage));
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
void start_pipe_api(void* handle, int stages_number,int is_pipe){

    stage_handler* my_handler=(stage_handler*)handle;
    my_handler->PIPELINE_ON=is_pipe;
    stage* ptr=my_handler->head;
    stage_params *params;
    bool first=0;
    int i=stages_number;

    while (ptr!=NULL&&i>0) {
        i--;
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
    i=stages_number-i;

    printf("%d stages are active\n",i);

}
void stop_pipe_api(void* my_handler){

    stage_handler* handle=((stage_handler*)my_handler);
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


    Node *message;// last node to enter the pipeline

    do{
        sleep(2);
        if(my_stage->sourseQu==NULL){
            /* for first stage ->  creating a new message to send*/
            message=createNode(NULL);
          }
        else{
            /* for the others stages get message to send from the previous stage's data */
            message=dequeue(my_stage->sourseQu);
            }

        message->data=my_stage->func(message->data, my_stage->params);

        if(my_stage->destQu==NULL){
            /* for last stage finish the message's cycle */
             freeNode(message);
        }
        else{
            /*for the others sending the message */
            enqueu(my_stage->destQu,message);
        }

        if(my_stage->sourseQu==NULL){
            /* first stage condition for stoping is PIPELINE_ON variable */
            my_stage->is_active=*PIPELINE_ON;
          }
        else{
            /* other stages' condition for stoping is geting NULL data value*/
            my_stage->is_active=message->data!=NULL;
            }


    }while(my_stage->is_active);

}

stage_t stage_api={

    .create_handler=create_handler_api,
    .free_handler=free_handler_api,
    .create_stage=create_stage_api,
    .free_stage=free_stage_api,
    /* .free_stages=free_stages_api,
        .free_stages_and_queues=free_stages_and_queues_api,*/
    .start_pipe=start_pipe_api,
    .stop_pipe=stop_pipe_api,
    .main_stage_thread=main_stage_thread_api
};


/*
 *  TOPUT:
             if(!my_stage->is_active)
                printf("---000---- capture is finishing---0000---\n");
            if(!my_stage->is_active)
                printf("---000----finishing---0000---\n");
*/
