#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "../headers/answer.h"
#include "../headers/filtersConfig.h"
#include "../headers/listTasks.h"



#define MAXSIZEMESSAGE 500

/*
    Message send from server to client
    -------------
*/
struct answer{       
    char message[MAXSIZEMESSAGE];
};


Answer mallocAnswer(){
    return malloc(sizeof(struct answer));
}



Answer createAnswer2(FiltersConfig filtersConfig, ListTasks runningTasks){
    int i;
    char* identificador;
    int maxExecucao;
    int emExecucao;
    char line[100];
    char* comando;
    int numberFilters = getNumberFiltersConfig(filtersConfig);
    int numberTasks = getNumberListTasks(runningTasks);
    int pid = getppid();
    char ppid[20];
    sprintf(ppid,"pid: %d",pid);

    Answer answer = (Answer) malloc(sizeof(struct answer));
    
    for( i=0; i < numberTasks; i++){
        Task task = getTaskIndex(runningTasks, i);        
        comando = getComando(task);
        sprintf(line,"Task #%d: %s\n",getNumberTask(task),comando);
        strcat(answer->message,line);
    }

    for( i=0; i < numberFilters; i++){
        Filter filter = getFilterConfigIndex(filtersConfig,i);
        identificador = getIdentificadorFilter(filter);
        maxExecucao = getMaxExecucaoFilter(filter);
        emExecucao = getEmExecucaoFilter(filter);
        sprintf(line,"filter %s: %d/%d (running/max)\n",identificador,emExecucao,maxExecucao);
        strcat(answer->message,line);
    }

    strcat(answer->message,ppid);
   
    return answer;
    
}


void deleteAnswer(Answer answer){
    free(answer);
}

/*
    Getters
    -------------
*/
char* getMessageAnswer(Answer answer){
    return strdup(answer->message);
}

/*
    Funcionalidades
    -------------
*/
int answerSize(){
    return sizeof(struct answer);
}

void printAnswer(Answer answer){
    printf("%s\n",answer->message);
}
