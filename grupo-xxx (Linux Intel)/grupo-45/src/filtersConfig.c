#include <stdio.h>
#include <stdlib.h>
#include "../headers/task.h"
#include "../headers/request.h"
#include "../headers/filtersConfig.h"


struct filter{
    char *identificador;
    char * executavel;
    int numeroMaxExecucao;
    int emExecucao;
    pid_t * pids;
};


struct filtersConfig{
    GPtrArray* filters;
    int numberFilters; // Numero de ordem da task
};


Filter createFilter(){
    Filter new = malloc(sizeof(struct filter));
    new->emExecucao = 0;
    return new;
}

char* getIdentificadorFilter(Filter filter){
    return strdup(filter->identificador);
}

char* getExecFilter(Filter filter){
    return strdup(filter->executavel);
}

int getMaxExecucaoFilter(Filter filter){
    return filter->numeroMaxExecucao;
}

int getEmExecucaoFilter(Filter filter){
    return filter->emExecucao;
}

int getDisponivelFilter(Filter filter){
    return (filter->numeroMaxExecucao)-(filter->emExecucao);
}

void addEmExecucaoFilter(Filter filter, int emExecucao){
    int execucaoFilter = filter->emExecucao;
    filter->emExecucao = emExecucao + execucaoFilter;
}

void removeEmExecucaoFilter(Filter filter, int emExecucao){
    int execucaoFilter = filter->emExecucao;
    filter->emExecucao = execucaoFilter - emExecucao;
}

void setIdentificadorFilter(Filter filter, char* identificador){
    filter->identificador = strdup(identificador);
}

void setExecutavelFilter(Filter filter, char* exec){
    filter->executavel = strdup(exec);
}

void setMaxExecucaoFilter(Filter filter, int maxExecucao){
    filter->numeroMaxExecucao = maxExecucao;
}

void printFilter(Filter filter){
    printf("\nIdentificador: %s\n",filter->identificador);
    printf("Executavel: %s\n",filter->executavel);
    printf("NumeroMaxExecucao: %d\n",filter->numeroMaxExecucao);
    printf("EmExecucao: %d\n\n",filter->emExecucao);
}


/*
void addPidFilter(Filter filter, int pid){
    filter->pids
}
*/

FiltersConfig createFiltersConfig(){
    FiltersConfig fConfig = malloc(sizeof(struct filtersConfig));

    fConfig->filters = g_ptr_array_new();
    fConfig->numberFilters = 0;

    return fConfig;
}

void addFilterConfig(FiltersConfig fConfig, Filter filter){
    // TODO fazer antes uma cópia da task
    g_ptr_array_add(fConfig->filters , (gpointer) filter);
    fConfig->numberFilters++;
}


Filter getFilterConfigIndex(FiltersConfig fConfig, int index){
    return g_ptr_array_index(fConfig->filters,index);
}


int getNumberFiltersConfig(FiltersConfig fConfig){
    return (int)fConfig->filters->len;
}



void updateFiltersConfig(FiltersConfig fConfig, int * filtersRequired, int update){
    int i;

    if(update == 1){
        // Colocar os filtros da task em execução
        for(i=0; i<fConfig->numberFilters; i++){
            Filter filter = getFilterConfigIndex(fConfig,i);
            addEmExecucaoFilter(filter, filtersRequired[i]);
        }
    }
    else if(update == -1){
         // Colocar os filtros da task em execução
        for(i=0; i<fConfig->numberFilters; i++){
            Filter filter = getFilterConfigIndex(fConfig,i);
            removeEmExecucaoFilter(filter, filtersRequired[i]);
        }

    }
}


void deleteFiltersConfig(FiltersConfig fConfig){
    g_ptr_array_free(fConfig->filters,TRUE);
}




