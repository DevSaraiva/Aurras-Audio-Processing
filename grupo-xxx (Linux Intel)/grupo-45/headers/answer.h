#ifndef FILE_ANSWER_SEEN
#define FILE_ANSWER_SEEN

#include "../headers/filtersConfig.h"
#include "../headers/listTasks.h"
#include <glib.h>

typedef struct answer *Answer;

Answer createAnswer1(int x);

Answer createAnswer2(FiltersConfig filtersConfig, ListTasks runningTasks);

void deleteAnswer(Answer answer);

int comparaToEnd(Answer a);


/* --- Functionality --------------------------------------------- */

int answerSize();

void printAnswer(Answer answer);

char* getMessageAnswer(Answer answer);

Answer createAnswer3();

int toInt(Answer a);

#endif
