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
    int filtersRequired[NUMBERFILTERS];
    char * comando;
};

Task createTask(int taskNumber, int pidProcess, int filtersRequired[]){
    int i;

    Task task = (Task) malloc(sizeof(struct task));

    task->numberTask = taskNumber;
    task->pidProcess = pidProcess;
    for( i=0; i<NUMBERFILTERS; i++){
        task->filtersRequired[i] = filtersRequired[i];
    }

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
