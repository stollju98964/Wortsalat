#include "input.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// Read words from file and store in 'words' array
int readWords(FILE *file, char words[][MAX_WORD_LEN], unsigned int maxWordCount)
{
    if (file == NULL || words == NULL || maxWordCount == 0)
        return 0;

    unsigned int count = 0;
    char line[512];

    while (count < maxWordCount && fgets(line, sizeof(line), file))
    {
        char *p = line;
        char cleaned[MAX_WORD_LEN];
        unsigned int ci = 0;
        int inword = 0;

        for (; *p != '\0' && count < maxWordCount; ++p)
        {
            if (isalpha((unsigned char)*p))
            {
                inword = 1;
                if (ci + 1 < MAX_WORD_LEN) /* Platz für Zeichen + Terminator */
                    cleaned[ci++] = (char)toupper((unsigned char)*p);
                /* sonst: überschüssige Zeichen verwerfen bis Wortende */
            }
            else
            {
                if (inword)
                {
                    cleaned[ci] = '\0';
                    if (ci > 0)
                    {
                        strncpy(words[count], cleaned, MAX_WORD_LEN);
                        words[count][MAX_WORD_LEN - 1] = '\0';
                        ++count;
                    }
                    /* reset für das nächste Token */
                    ci = 0;
                    inword = 0;
                }
            }
        }

        /* falls die Zeile mit einem Wort endet, noch hinzufügen */
        if (inword && count < maxWordCount)
        {
            cleaned[ci] = '\0';
            if (ci > 0)
            {
                strncpy(words[count], cleaned, MAX_WORD_LEN);
                words[count][MAX_WORD_LEN - 1] = '\0';
                ++count;
            }
        }
    }

    return count;
}