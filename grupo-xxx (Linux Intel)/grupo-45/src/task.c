#include <stdio.h>
#include <stdlib.h>
#include "../headers/task.h"



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
    int *filtersRequired;
    char * comando;
};

Task createTask(int taskNumber, int pidProcess, int filtersRequired[], int numFilters, char * comando){
    int i;

    Task task = (Task) malloc(sizeof(struct task));

    task->numberTask = 10000000;
    task->pidProcess = pidProcess;
    //task->filtersRequired = malloc(sizeof(int) * numFilters);
    
    for( i=0; i<numFilters; i++){
        
      //  task->filtersRequired[i] = filtersRequired[i];
    }
    
    task->comando = comando;

    return task;
}

int getFilter(Task task, int indexFilter){
    return task->filtersRequired[indexFilter];
}

int getNumberTask(Task task){
    return task->numberTask;
}

int getPidProcess(Task task){
    return task->pidProcess;
}

void deleteTask(Task task){
    free(task);
}

char * getComando(Task task){
    return task ->comando;
}
