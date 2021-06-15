#ifndef FILE_TASK_SEEN
#define FILE_TASK_SEEN

#include <glib.h>
#include "../headers/request.h"
#include "../headers/filtersConfig.h"

/*
 * Constants
 * */
#define NUMBERFILTERS 4

typedef struct task *Task;


/*
 * Create task
 * */
Task createTask(Request request,FiltersConfig filterConfig);

void setNumberTask(Task task, int numberTask);

int getFilter(Task task, int indexFilter);

int getNumberTask(Task task);

char* getComando(Task task);

int getPidProcessTask(Task task);

void deleteTask(Task task);

void printTask(Task task);

#endif
