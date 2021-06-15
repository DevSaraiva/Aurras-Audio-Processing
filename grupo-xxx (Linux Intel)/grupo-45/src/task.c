#include <stdio.h>
#include <stdlib.h>
#include "../headers/task.h"
#include "../headers/request.h"
#include "../headers/filtersConfig.h"

#define MAXFILTERS 40
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
    int numberFilters;
    char * comando;
    int filtersRequired[];
};


Task createTask(Request request,FiltersConfig filterConfig){
    int i,j;
    int numberFiltersConfig;

    numberFiltersConfig = getNumberFiltersConfig(filterConfig);
    Task task = (Task) malloc(sizeof(struct task) + numberFiltersConfig * sizeof(int));

    /* O numero da task fica associado a esta task quando esta é inserida na lista de tasks */
    task->numberTask = -1;
    task->pidProcess = getRequestPidProcess(request);
    char** filtersRequest = getFilters(request);

    i=0, j=0;
    while(filtersRequest[i]){
        for(j=0; j<numberFiltersConfig; j++){
            Filter filter = getFilterConfigIndex(filterConfig,j);
            char* identificadorFilter = getIdentificadorFilter(filter);
            if(strcmp(filtersRequest[i],identificadorFilter) == 0){
                task->filtersRequired[j]++;
                break;
            }
        }
        i++;
    }

    task->numberFilters = numberFiltersConfig;
    char* message = getMessage(request);
    task->comando = strdup(message);

    return task;
}

void setNumberTask(Task task, int numberTask){
    task->numberTask = numberTask;
}

int getFilter(Task task, int indexFilter){
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


void deleteTask(Task task){
    free(task);
}

void printTask(Task task){
    printf("\nnumberTask: %d\n",task->numberTask);
    printf("pidProcess: %d\n",task->pidProcess);

    int i;
    for(i=0;i<task->numberFilters;i++){
        printf("Filtro %d: %d\n",i,task->filtersRequired[i]);
    }
    printf("comando: %s\n\n",task->comando);

}
