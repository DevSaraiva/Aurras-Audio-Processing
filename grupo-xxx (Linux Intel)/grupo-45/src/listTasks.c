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

ListTasks createListTasks(){
    ListTasks wTasks = malloc(sizeof(struct listTaskSv));

    wTasks->listTasks = g_ptr_array_new();
    wTasks->numberTasks = 0;

    return wTasks;
}

void addTask(ListTasks wTasks, Task task){
    // TODO fazer antes uma cópia da task
    g_ptr_array_add(wTasks->listTasks , (gpointer) task);
    wTasks->numberTasks++;
}

void removeTaskIndex(ListTasks wTasks, int index){
    g_ptr_array_remove_index(wTasks->listTasks ,index);
    wTasks->numberTasks--;
}

Task getTaskIndex(ListTasks wTasks, int index){
    Task task = g_ptr_array_index(wTasks->listTasks,index);
    return task;
}


int getNumberListTasks(ListTasks wTasks){
    return (int)wTasks->listTasks->len;
}

void deleteListTasks(ListTasks wTasks){
    g_ptr_array_free(wTasks->listTasks,TRUE);
}

