#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1024

int readWords(FILE *file, char words[][MAX_WORD_LEN], unsigned int maxWordCount);

#endif
