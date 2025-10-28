#include <stdlib.h>
#include <stdio.h>
#include "input.h"
#include "game.h"
#include "graphicalGame.h"

#define MAX_NUMBER_OF_WORDS 100
#define SALAD_SIZE 20

int main(int argc, char *argv[])
{
    int exitCode = EXIT_SUCCESS;

    // überprüfen ob die richtige Anzahl an Argumenten übergeben wurde
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <path to file with search words>\n", argv[0]);
        exitCode = EXIT_FAILURE;
    }
    else
    {
        char words[MAX_NUMBER_OF_WORDS][MAX_WORD_LEN]; // Array für die Suchwörter
        unsigned int wordCount = 0;

        FILE *file = fopen(argv[1], "r");

        if(file != NULL)
        {
            unsigned int placedWords = 0;
            char wordSalad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN]; // 2D array um den Wortsalat zu speichern

            // Wörter zählen und einlesen
            wordCount = readWords(file, words, MAX_NUMBER_OF_WORDS);
            fclose(file);

            // Wörter in den Wortsalat einfügen
            placedWords = createWordSalad(wordSalad, SALAD_SIZE, words, wordCount);

            // TODO:
            // Check if all words were successfully placed
            // Start the game if successful
            // error message if some words couldn't be placed

            if(wordCount == 0)
            {
                fprintf(stderr, "No words found in file %s.\n", argv[1]);
                exitCode = EXIT_FAILURE;
            }
            else if(placedWords == wordCount)
            {
                printf("All %u words placed successfully. Starting game...\n", wordCount);

                /* echten GUI-Start aufrufen (prüfe Namen in graphicalGame.h) */
                startGraphicalGame(wordSalad, SALAD_SIZE, words, wordCount);
            }
            else
            {
                // Fehlermeldung, wenn nicht alle Wörter platziert werden konnten
                fprintf(stderr, "Only %u of %u words could be placed. Aborting.\n", placedWords, wordCount);
                exitCode = EXIT_FAILURE;
            }
        }
        else
        {
            //Error ausgeben, wenn die Datei nicht geöffnet werden konnte
            fprintf(stderr, "Could not open file %s for reading ...\n", argv[1]);
            exitCode = EXIT_FAILURE;
        }
    }

    return exitCode;
}