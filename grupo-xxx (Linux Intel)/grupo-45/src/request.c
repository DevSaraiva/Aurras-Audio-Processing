#include <stdio.h>
#include <stdlib.h>
#include "../headers/request.h"


#define MAXARGS 10
#define MAXSIZEARG 40
/*
    Message send from client to server
    -------------
*/
struct request{
    int service;           /* Type of serice requested */
    int pidProcess;        /* Client Process Pid that made the request */
    char arguments[MAXARGS][MAXSIZEARG];
    int numberArguments;
};

/*
    Métodos auxiliares
    -------------
*/


char* getInputFile(Request request){
    if(request->service == 2) return NULL;
    return request->arguments[1];
}

char* getOutputFile(Request request){
    if(request->service == 2) return NULL;
    return request->arguments[2];
}

char** getFilters(Request request){
    int i;
    if(request->service == 2) return NULL;
    char** filters = malloc(sizeof(char*) * (request->numberArguments)); 

    for(i=3;i<request->numberArguments;i++){
        filters[i-3] = malloc(sizeof(char) * strlen(request->arguments[i]));
        strcat(filters[i-3],request->arguments[i]);
    }
    return filters;
}


Request createRequest(){
    return (Request) malloc(sizeof(struct request));
}


Request initRequest(int argc, char** argv, int pidProcess){
    int i;
    if(argc < 2) return NULL;

    Request request = (Request) malloc(sizeof(struct request));

    if( ! strcmp(argv[1],"transform") && argc>4) {
        request->service = 1;
    }
    else if( ! strcmp(argv[1],"status") && argc == 2){
        request->service = 2;
    }
    else{
        printf("Argumentos não são válidos");
        return NULL;
    }

    request->pidProcess = pidProcess;

    request->numberArguments = 0;
    for(i = 1; i<argc; i++){
        strcat(request->arguments[i-1],argv[i]);
        request->numberArguments++;
    }

    return request;
}

void deleteRequest(Request request){
    free(request);
}

/*
    Getters
    -------------
*/
char getRequestService(Request request){
    return request->service;
}

int getRequestPidProcess(Request request){
    return request->pidProcess;
}

char* getMessage(Request request){
    char* message = malloc(sizeof(char) * MAXARGS * MAXSIZEARG);

    for (int i = 0; i < request->numberArguments ;i++){
       strcat(message,request->arguments[i]);
       strcat(message," ");
    }

    return message;
}

/*
    Funcionalidades
    -------------
*/
/*
int* filtersRequest(Request request){
    if(request->service == 2) return 0;

    int filtersNeeded[];
    char* token;

   token = strtok(str, s);

   while( token != NULL ) {
      printf( " %s\n", token );

      token = strtok(NULL, s);
   }
}
*/
int requestSize(){
    return sizeof(struct request);
}

void printRequest(Request request){
    int i = 0;
    setbuf(stdout,NULL);
    printf("Servico: %d\n",request->service);
    printf("PidProcesso: %d\n",request->pidProcess);

    printf("Argumentos\n");
    for(i=0; i<request->numberArguments; i++){
        printf("    :%s\n",request->arguments[i]);
    }
}
