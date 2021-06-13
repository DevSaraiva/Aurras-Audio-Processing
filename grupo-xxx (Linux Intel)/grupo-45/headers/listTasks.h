#ifndef FILE_LIST_TASKS_SEEN
#define FILE_LIST_TASKS_SEEN

#include "../headers/task.h"
#include <glib.h>

typedef struct listTaskSv *listTasks;

/*
 * Create list of tasks
 * */
listTasks createListTasks();

void addTask(listTasks wTasks, Task task);

void removeTaskIndex(listTasks wTasks, int index);

Task getTaskIndex(listTasks wTasks, int index);

int getSizeListTasks(listTasks wTasks);

void deleteListTasks(listTasks wTasks);

#endif
