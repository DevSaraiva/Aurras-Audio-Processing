#include <stdio.h>
#include <stdlib.h>
#include "../headers/waitingTasks.h"
#include "../headers/task.h"
#include <glib.h>

/*
 * struct com informação da tarefa que está na lista de espera para executar
 *  -> Numero da task
 *  -> Pid do processo
 *  -> filtros necessários [lista] indice 0 corresponde ao filtro 0, indice 1 ...
 * */
struct waitingTasks{
    GPtrArray* listTasks;
    int numberTasks; // Numero de ordem da task
};

WaitingTasks createWaitingTasks(){
    WaitingTasks wTasks = malloc(sizeof(struct waitingTasks));

    wTasks->listTasks = g_ptr_array_new();
    wTasks->numberTasks = 0;

    return wTasks;
}

void addTask(WaitingTasks wTasks, Task task){
    // TODO fazer antes uma cópia da task
    g_ptr_array_add(wTasks->listTasks , (gpointer) task);
    wTasks->numberTasks++;
}

void removeTaskIndex(WaitingTasks wTasks, int index){
    g_ptr_array_remove_index(wTasks->listTasks ,index);
    wTasks->numberTasks--;
}

Task getTaskIndex(WaitingTasks wTasks, int index){
    Task task = g_ptr_array_index(wTasks->listTasks,index);
    return task;
}


int getNumberWaitingTasks(WaitingTasks wTasks){
    return (int)wTasks->listTasks->len;
}

void deleteWaitingTasks(WaitingTasks wTasks){
    g_ptr_array_free(wTasks->listTasks,TRUE);
}

