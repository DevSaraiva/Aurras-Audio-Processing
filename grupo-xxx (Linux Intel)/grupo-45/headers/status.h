#ifndef FILE_STATUS_SEEN
#define FILE_STATUS_SEEN

#include "listTasks.h"
#include "filtro.h"
#include "../headers/listTasks.h"
#include "../headers/status.h"

typedef struct STATUS
{
    char ** tasksExec;
    char ** config;
    int pid;

} status;


status getStatus(listTasks emExecucao, filtroConfig * config , int nFiltros, int pidPai);

#endif
