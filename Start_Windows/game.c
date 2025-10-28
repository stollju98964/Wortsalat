#include "game.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RAND_TRIES_PER_WORD 10
#define EMPTY_CHAR 0

//TODO: Spiellogik implementieren:
/* * Wörter aus der Wortliste zufällig horizontal oder vertikal platzieren
   * restliche Felder mit zufälligen Buchstaben füllen */

   
int createWordSalad(char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldLen, const char words[][MAX_WORD_LEN], unsigned int wordCount)
{
    unsigned int placed = 0;

    if(searchFieldLen == 0)
        return 0;

    // array mit leeren Zeichen initialisieren
    for(unsigned int r = 0; r < searchFieldLen; ++r)
        for(unsigned int c = 0; c < searchFieldLen; ++c)
            salad[r][c] = EMPTY_CHAR;

    //random seed setzen
    srand((unsigned)time(NULL));

    for(unsigned int w = 0; w < wordCount; ++w)
    {
        const char *word = words[w];
        size_t len = strlen(word);

        if(len == 0 || len > searchFieldLen)
            continue; // wort zu lang oder leer

        int placed_this = 0;

        for(int attempt = 0; attempt < MAX_RAND_TRIES_PER_WORD && !placed_this; ++attempt)
        {
            int horiz = rand() % 2; // 0 = horizontal, 1 = vertical
            unsigned int row, col;

            if(horiz == 0)
            {
                row = rand() % searchFieldLen;
                col = rand() % (searchFieldLen - (unsigned)len + 1);
            }
            else
            {
                row = rand() % (searchFieldLen - (unsigned)len + 1);
                col = rand() % searchFieldLen;
            }

            //überprüfen, ob das wort hier platziert werden kann
            int ok = 1;
            for(unsigned int i = 0; i < len; ++i)
            {
                unsigned int r = horiz ? row : row + i;
                unsigned int c = horiz ? col + i : col;
                char existing = salad[r][c];
                if(existing != EMPTY_CHAR && existing != word[i])
                {
                    ok = 0;
                    break;
                }
            }

            if(!ok)
                continue;

            //wort platzieren
            for(unsigned int i = 0; i < len; ++i)
            {
                unsigned int r = horiz ? row : row + i;
                unsigned int c = horiz ? col + i : col;
                salad[r][c] = word[i];
            }

            placed_this = 1;
            ++placed;
        }
    }

    //restliche felder mit zufälligen buchstaben füllen
    for(unsigned int r = 0; r < searchFieldLen; ++r)
    {
        for(unsigned int c = 0; c < searchFieldLen; ++c)
        {
            if(salad[r][c] == EMPTY_CHAR)
                salad[r][c] = (char)('A' + (rand() % 26));
        }
    }

    return placed;
}

// gibt den Wortsalat auf der Konsole aus
void showWordSalad(const char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldLen)
{
    for(unsigned int r = 0; r < searchFieldLen; ++r)
    {
        for(unsigned int c = 0; c < searchFieldLen; ++c)
        {
            putchar(salad[r][c]);
            if(c + 1 < searchFieldLen) putchar(' ');
        }
        putchar('\n');
    }
}
