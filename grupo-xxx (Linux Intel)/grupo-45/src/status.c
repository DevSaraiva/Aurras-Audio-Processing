
#include "../headers/status.h"


struct STATUS
{
    char ** tasksExec;
    char ** config;
    int pid;
};



status getStatus(listTasks emExecucao, filtroConfig * config , int nFiltros){

    status sts;
    char tasks [nFiltros][100];
    int numTasks = getSizeListTasks(emExecucao);
    for(int i = 0; i < numTasks; i++){
        
    }
    



    

}