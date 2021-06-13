#ifndef FILE_REQUEST_SEEN
#define FILE_REQUEST_SEEN

#include <glib.h>


typedef struct request *Request;


Request createRequest();

Request initRequest(int argc, char** argv, int pidProcess);

void deleteRequest(Request request);

/* --- Getters --------------------------------------------------- */

char getRequestService(Request request);

int getRequestPidProcess(Request request);

char* getRequestArgs(Request request);


/* --- Functionality --------------------------------------------- */

int requestSize();

void printRequest(Request request);

#endif
