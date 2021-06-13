#include <stdio.h>
#include <stdlib.h>
#include "../headers/listTasks.h"
#include "../headers/task.h"
#include <glib.h>

/*
 * struct com informação da tarefa que está na lista de espera para executar
 *  -> Numero da task
 *  -> Pid do processo
 *  -> filtros necessários [lista] indice 0 corresponde ao filtro 0, indice 1 ...
 * */
struct listTaskSv{
    GPtrArray* listTasks;
    int numberTasks; // Numero de ordem da task
};

listTasks createListTasks(){
    listTasks lTasks = malloc(sizeof(struct listTaskSv));

    lTasks->listTasks = g_ptr_array_new();
    lTasks->numberTasks = 0;

    return lTasks;
}

void addTask(listTasks wTasks, Task task){
    // TODO fazer antes uma cópia da task
    g_ptr_array_add(wTasks->listTasks , (gpointer) task);
    wTasks->numberTasks++;
}

void removeTaskIndex(listTasks wTasks, int index){
    g_ptr_array_remove_index(wTasks->listTasks ,index);
    wTasks->numberTasks--;
}

Task getTaskIndex(listTasks wTasks, int index){
    Task task = g_ptr_array_index(wTasks->listTasks,index);
    return task;
}


int getSizeListTasks(listTasks wTasks){
    return (int)wTasks->listTasks->len;
}

void deleteListTasks(listTasks wTasks){
    g_ptr_array_free(wTasks->listTasks,TRUE);
}

