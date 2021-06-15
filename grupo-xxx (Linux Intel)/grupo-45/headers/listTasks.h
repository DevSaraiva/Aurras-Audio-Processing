#ifndef FILE_LIST_TASKS_SEEN
#define FILE_LIST_TASKS_SEEN

#include "../headers/task.h"
#include <glib.h>

typedef struct listTaskSv *ListTasks;

/*
 * Create list of tasks
 * */
ListTasks createListTasks();

void addTask(ListTasks wTasks, Task task);

void removeTaskIndex(ListTasks wTasks, int index);

Task getTaskIndex(ListTasks wTasks, int index);

int getNumberListTasks(ListTasks wTasks);

void deleteListTasks(ListTasks wTasks);

#endif
