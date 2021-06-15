#ifndef FILE_TASK_SEEN
#define FILE_TASK_SEEN

#include <glib.h>
#include <unistd.h>
#include <fcntl.h>
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

int getFilterRequiredIndex(Task task, int indexFilter);

int getNumberFiltersTask(Task task);

char** getExecsFilters(Task task, FiltersConfig filterConfig);

int* getFiltersRequired(Task task);

int getNumberTask(Task task);

char* getComando(Task task);

int getPidProcessTask(Task task);

void deleteTask(Task task);

void printTask(Task task);

int validateTaskProcessing(FiltersConfig fConfig ,Task t);


#endif
