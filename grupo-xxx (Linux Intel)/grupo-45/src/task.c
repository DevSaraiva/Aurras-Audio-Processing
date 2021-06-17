#include <stdio.h>
#include <stdlib.h>
#include "../headers/task.h"
#include "../headers/request.h"
#include "../headers/filtersConfig.h"
#include "../headers/answer.h"

#define MAXFILTERS 40
#define FIFOSERVERCLIENTS "/tmp/fifo"
/*
 * struct com informação da tarefa que está na lista de espera para executar
 *  -> Numero da task
 *  -> Pid do processo
 *  -> filtros necessários [lista] indice 0 corresponde ao filtro 0, indice 1 ...
 * -> comando passado pelo cliente
 * */



struct task{
    int numberTask; // Numero de ordem da task
    int pidProcess;
    int numberTotalFilters;
    char * comando;
    char ** execsFilters;
    int numberFiltersTask;
    char * inputFile; 
    char * outputFile;
    int filtersRequired[];
};


Task createTask(Request request,FiltersConfig filterConfig){
    int i,j;
    int numberFiltersConfig;
    int filtersComand = 0;
    numberFiltersConfig = getNumberFiltersConfig(filterConfig);
    Task task = (Task) malloc(sizeof(struct task) + numberFiltersConfig * sizeof(int));

    /* O numero da task fica associado a esta task quando esta é inserida na lista de tasks */
    task->numberTask = -1;
    task->pidProcess = getRequestPidProcess(request);
    char** filtersRequest = getFilters(request);
    int numberFiltersRequest = getNumberFiltersRequest(request);

    i=0, j=0;
    for(i=0;i<numberFiltersConfig;i++){
        task->filtersRequired[i] = 0;
    }

    for(i=0;i<numberFiltersRequest;i++){
        for(j=0; j<numberFiltersConfig; j++){
            Filter filter = getFilterConfigIndex(filterConfig,j);
            char* identificadorFilter = getIdentificadorFilter(filter);
            if(strcmp(filtersRequest[i],identificadorFilter) == 0){
                task->filtersRequired[j]++;
                break;
            }
        }

    }

    filtersComand = i;
    char** execsFilters = malloc(sizeof(char*) * filtersComand);
    task->numberFiltersTask = filtersComand;
    
    i=0;
    for(i=0;i<numberFiltersRequest;i++){
        for(j=0; j<numberFiltersConfig; j++){
            Filter filter = getFilterConfigIndex(filterConfig,j);
            char* identificadorFilter = getIdentificadorFilter(filter);
            if(strcmp(filtersRequest[i],identificadorFilter) == 0){
                char* exec = getExecFilter(filter);
                execsFilters[i] = strdup(exec);
                break;
            }
        }
    }
    task->inputFile = strdup(getInputFile(request));
    task->outputFile = strdup(getOutputFile(request));
    task->execsFilters = execsFilters;
    task->numberTotalFilters = numberFiltersConfig;
    char* message = getMessage(request);
    task->comando = strdup(message);

    return task;
}

void setNumberTask(Task task, int numberTask){
    task->numberTask = numberTask;
}

int getFilterRequiredIndex(Task task, int indexFilter){
    return task->filtersRequired[indexFilter];
}

int getNumberFiltersTask(Task task){
    return task->numberFiltersTask;
}

int* getFiltersRequired(Task task){
   int* p = malloc(task->numberTotalFilters * sizeof(int));
   memcpy(p, task->filtersRequired, task->numberTotalFilters * sizeof(int));
   return p;
}

char** getExecsFilters(Task task, FiltersConfig filterConfig){
    int i;
    int numberFilters = getNumberFiltersTask(task);
    char** copyfiltersExecs = malloc(sizeof(char*) * numberFilters);

    
    for(i = 0;i<numberFilters;i++){
        copyfiltersExecs[i] = strdup(task->execsFilters[i]);
    }

    return copyfiltersExecs;
}

int getFilterRequired(Task task, int indexFilter){
    return task->filtersRequired[indexFilter];
}

int getNumberTask(Task task){
    return task->numberTask;
}

char* getComando(Task task){
    return strdup(task->comando);
}

int getPidProcessTask(Task task){
    return task->pidProcess;
}


int validateTaskProcessing(FiltersConfig fConfig ,Task t){
    int i;
    int filterDisponivel;
    int * filtersRequired = getFiltersRequired(t);
    int pipeAnswer = 0;
    char pipeClient[30];

    // Colocar os filtros da task em execução
    for(i=0; i<getNumberFiltersConfig(fConfig); i++){
        Filter filter = getFilterConfigIndex(fConfig,i);
        filterDisponivel = getDisponivelFilter(filter);
        if(filterDisponivel < filtersRequired[i]){
            return -1;
        }
    }

    sprintf(pipeClient,"%s%d",FIFOSERVERCLIENTS,getPidProcessTask(t));

        if( (pipeAnswer = open(pipeClient, O_WRONLY)) == -1){
            perror("fifo between server and clients Read");
        }
    
    Answer a = createAnswer1(2);

    write(pipeAnswer,a,answerSize());


    return 1;
}


void deleteTask(Task task){
    free(task);
}

void printTask(Task task){
    printf("\nnumberTask: %d\n",task->numberTask);
    printf("pidProcess: %d\n",task->pidProcess);

    int i;
    for(i=0;i<task->numberTotalFilters;i++){
        printf("Filtro %d: %d\n",i,task->filtersRequired[i]);
    }
    for(i=0;i<task->numberFiltersTask;i++){
        printf("execFilter: %s\n",task->execsFilters[i]);
    }
    printf("comando: %s\n\n",task->comando);

}


char * getInputFileTask(Task t){
    return strdup(t->inputFile);
}

char * getOutputFileTask(Task t){
    return strdup(t->outputFile);
}
