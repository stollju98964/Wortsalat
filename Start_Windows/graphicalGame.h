#ifndef GUI_H
#define GUI_H

#include "game.h"

/* Fügt hier die Deklaration der GUI-Startfunktion ein.
   Prüfe Rückgabewert / Parameter, falls in deiner implementation anders. */
void startGraphicalGame(const char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN],
                        unsigned int searchFieldLen,
                        const char words[][MAX_WORD_LEN],
                        unsigned int wordCount);

#endif
