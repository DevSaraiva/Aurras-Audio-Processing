#ifndef FILE_TASK_SEEN
#define FILE_TASK_SEEN

#include <glib.h>

/*
 * Constants
 * */
typedef struct task *Task;


/*
 * Create task
 * */

Task createTask(int taskNumber, int pidProcess, int filtersRequired[], int numFilters, char * comando);

int getFilter(Task task, int indexFilter);

int getNumberTask(Task task);

int getPidProcess(Task task);

void deleteTask(Task task);

char * getComando(Task task);

#endif
