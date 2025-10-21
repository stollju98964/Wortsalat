#ifndef GAME_H
#define GAME_H

#include "input.h"

#define MAX_SEARCH_FIELD_LEN 100

int createWordSalad(char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldLen, const char words[][MAX_WORD_LEN], unsigned int wordCount);
void showWordSalad(const char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldLen);

#endif
