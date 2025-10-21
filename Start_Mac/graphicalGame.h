#ifndef GUI_H
#define GUI_H

#include "game.h"

void startGame(const char wordSalad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldSize, char words[][MAX_WORD_LEN], unsigned int numberOfWords, unsigned int windowWidth);

#endif
