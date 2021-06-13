#include <stdio.h>
#include <string.h>
#include "../headers/status.h"



status getStatus(listTasks emExecucao, filtroConfig * config , int nFiltros, int pidPai){

    status sts;
    
    //Tasks em execução
    
    int numTasks = getSizeListTasks(emExecucao);
    char ** tasks = malloc(sizeof(char *) * numTasks);
    
    for(int i = 0; i < numTasks; i++){
        Task t = getTaskIndex(emExecucao,i);
        char * comando = strdup(getComando(t));
        tasks[i] = malloc(sizeof(char)*100);
        sprintf(tasks[i],"Task #%d: %s",getNumberTask(t),comando);

        
    }
    
    sts.tasksExec = tasks;

    //Config

    char ** conf = malloc(sizeof(char *) * nFiltros);

    for(int i = 0; i < numTasks; i++){
        
        conf[i] = malloc(sizeof(char)*100);
        sprintf(conf[i], "filter %s: %d/%d (running/max)",config[i].identificador,config[i].emExecucao,config[i].numeroMaxExecucao);

    }

    sts.config = conf;

    // pid

    sts.pid = pidPai;



    return sts;
}