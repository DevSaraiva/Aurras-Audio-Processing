#ifndef FILE_WAITING_TASKS_SEEN
#define FILE_WAITING_TASKS_SEEN

#include "../headers/task.h"
#include <glib.h>

typedef struct waitingTasks *WaitingTasks;

/*
 * Create list of waiting tasks
 * */
WaitingTasks createWaitingTasks();

void addTask(WaitingTasks wTasks, Task task);

void removeTaskIndex(WaitingTasks wTasks, int index);

Task getTaskIndex(WaitingTasks wTasks, int index);

int getNumberWaitingTasks(WaitingTasks wTasks);

void deleteWaitingTasks(WaitingTasks wTasks);

#endif
