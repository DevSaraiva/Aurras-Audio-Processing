#ifndef FILE_FILTERS_CONFIG_SEEN
#define FILE_FILTERS_CONFIG_SEEN

#include <glib.h>


typedef struct filtersConfig *FiltersConfig;

typedef struct filter *Filter;



Filter createFilter();

char* getIdentificadorFilter(Filter filter);

char* getExecFilter(Filter filter);

int getMaxExecucaoFilter(Filter filter);

int getEmExecucaoFilter(Filter filter);

int getDisponivelFilter(Filter filter);

void addEmExecucaoFilter(Filter filter, int emExecucao);

void removeEmExecucaoFilter(Filter filter, int emExecucao);

void setIdentificadorFilter(Filter filter, char* identificador);

void setExecutavelFilter(Filter filter, char* exec);

void setMaxExecucaoFilter(Filter filter, int maxExecucao);

void printFilter(Filter filter);


FiltersConfig createFiltersConfig();

void addFilterConfig(FiltersConfig fConfig, Filter filter);

Filter getFilterConfigIndex(FiltersConfig fConfig, int index);

int getNumberFiltersConfig(FiltersConfig fConfig);

void updateFiltersConfig(FiltersConfig fConfig, int filterRequired[], int update);

void deleteFiltersConfig(FiltersConfig fConfig);

void printFilterConfig(FiltersConfig fConfig);


#endif
