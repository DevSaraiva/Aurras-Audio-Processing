#include <stdio.h>
#include <stdlib.h>
#include "../headers/request.h"


#define MAXSIZEMESSAGE 400

/*
    Message send from client to server
    -------------
*/
struct request{
    int service;           /* Type of serice requested */
    int pidProcess;        /* Client Process Pid that made the request */
    char arguments[MAXSIZEMESSAGE];   /* Numero de ordem da task */
};

/*
    Métodos auxiliares
    -------------
*/

/*
Pega nos argumentos e coloca-os numa string separados por espaços. O simbolo ';' indica que não existem mais argumentos
*/
static char* parser(int argc, char ** args){

    char* message = malloc(sizeof(char) * MAXSIZEMESSAGE);

    for (int i = 0; i < argc ;i++){
       strcat(message,args[i]);
       strcat(message," ");
    }
    strcat(message,";");

    return message;
}


Request createRequest(){
    return (Request) malloc(sizeof(struct request));
}


Request initRequest(int argc, char** argv, int pidProcess){

    char* message = parser(argc,argv);

    Request request = (Request) malloc(sizeof(struct request));

    if( ! strcmp(argv[1],"transform") ) { request->service = 1; }
    else if( ! strcmp(argv[1],"status") ) { request->service = 2; }
    else return NULL;

    request->pidProcess = pidProcess;
    strcat(request->arguments,message);

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

char* getRequestArgs(Request request){
    return strdup(request->arguments);
}

/*
    Funcionalidades
    -------------
*/

int requestSize(){
    return sizeof(struct request);
}

void printRequest(Request request){
    setbuf(stdout,NULL);
    printf("Servico: %d\n",request->service);
    printf("PidProcesso: %d\n",request->pidProcess);
    printf("Argumentos: %s\n",request->arguments);
}
