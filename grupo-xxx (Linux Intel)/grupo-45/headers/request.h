#ifndef FILE_REQUEST_SEEN
#define FILE_REQUEST_SEEN

#include <glib.h>


typedef struct request *Request;


Request createRequest();

Request initRequest(int argc, char** argv, int pidProcess);

void deleteRequest(Request request);

/* --- Getters --------------------------------------------------- */

char getRequestService(Request request);

int getNumberFiltersRequest(Request request);

int getRequestPidProcess(Request request);

char* getRequestArgs(Request request);

char* getOutputFile(Request request);

char* getInputFile(Request request);


/* --- Functionality --------------------------------------------- */

int requestSize();

char** getFilters(Request request);

void printRequest(Request request);

char* getMessage(Request request);

#endif
