#ifndef FILE_LIST_TASKS_SEEN
#define FILE_LIST_TASKS_SEEN

#include "../headers/task.h"
#include "../headers/filtersConfig.h"
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

int removeTask (ListTasks wTasks,int num);

void processWaitingTasks(ListTasks wTasks, ListTasks runningTasks, FiltersConfig filtersConfig);


Task getTask (ListTasks wTasks,int num);

 int removeTaskByNumber (ListTasks wTasks,int num);

#endif
