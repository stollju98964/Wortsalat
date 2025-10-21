#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unity.h"

#include "input.h"
#include "game.h"

// ANSI Escape Codes für Farben
// Hinweis: damit das in CLion mit der integrierten Konsole/Output funktioniert, muss man auf die Run-Config gehen und
// das Häkchen bei 'emulate terminal in the output console' setzen
// (s. auch: https://stackoverflow.com/questions/32742850/how-to-show-colored-console-output-in-clion)
#define RESET       "\033[0m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define YELLOW      "\033[33m"
#define CYAN        "\033[36m"
#define BOLD        "\033[1m"

// Eine Funktion, um die Test-Ergebnisse zu färben
void print_test_result(int result) {
    if (result == 0) {
        // Test war erfolgreich
        printf(GREEN "OK\n" RESET);
    } else {
        // Test ist fehlgeschlagen
        printf(RED "FAILED\n" RESET);
    }
}

// ---------- Tests für input.c ----------
void test_readWords_simple(void) {
    FILE *f = fopen("testwords_simple.txt", "r");

    char words[10][MAX_WORD_LEN];
    int count = readWords(f, words, 10);
    fclose(f);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_STRING("APFEL", words[0]);
    TEST_ASSERT_EQUAL_STRING("BANANE", words[1]);
    TEST_ASSERT_EQUAL_STRING("KIWI", words[2]);
}

void test_readWords_with_delimiters(void) {
    FILE *f = fopen("testwords_delims.txt", "r");

    char words[10][MAX_WORD_LEN];
    int count = readWords(f, words, 10);
    fclose(f);

    TEST_ASSERT_EQUAL_INT(3, count);
    TEST_ASSERT_EQUAL_STRING("HUND", words[0]);
    TEST_ASSERT_EQUAL_STRING("KATZE", words[1]);
    TEST_ASSERT_EQUAL_STRING("MAUS", words[2]);
}

void test_readWords_empty_file(void) {
    FILE *f = fopen("testwords_empty.txt", "r");

    char words[10][MAX_WORD_LEN];
    int count = readWords(f, words, 10);
    fclose(f);

    TEST_ASSERT_EQUAL_INT(0, count);
}

// ---------- Tests für game.c ----------
void test_createWordSalad_all_fit(void) {
    char words[3][MAX_WORD_LEN] = {"CAT", "DOG", "MOUSE"};
    char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN];

    int placed = createWordSalad(salad, 20, words, 3);

    TEST_ASSERT_EQUAL_INT(3, placed);

    // Sicherstellen, dass alle Felder gefüllt sind
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            TEST_ASSERT_GREATER_OR_EQUAL('A', salad[i][j]);
            TEST_ASSERT_LESS_OR_EQUAL('Z', salad[i][j]);
        }
    }
}

void test_createWordSalad_too_small(void) {
    char words[2][MAX_WORD_LEN] = {"ELEPHANT", "GIRAFFE"};
    char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN];

    int placed = createWordSalad(salad, 5, words, 2);

    TEST_ASSERT_GREATER_OR_EQUAL(0, placed);
    TEST_ASSERT_LESS_OR_EQUAL(2, placed);

    // Feld muss vollständig gefüllt sein
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            TEST_ASSERT_GREATER_OR_EQUAL('A', salad[i][j]);
            TEST_ASSERT_LESS_OR_EQUAL('Z', salad[i][j]);
        }
    }
}

// ---------- Test Setup und TearDown Funktionen ----------

// Hier Setup- und TearDown-Funktionen definieren,
// falls Vor- und Nachbereitungen für die Tests benötigt.

void setUp(void) {
    FILE *f = fopen("testwords_delims.txt", "w");
    fprintf(f, "Hund,Katze; Maus\n");
    fclose(f);

    f = fopen("testwords_simple.txt", "w");
    fprintf(f, "Apfel\nBanane\nKiwi");
    fclose(f);

    f = fopen("testwords_empty.txt", "w");
    fclose(f);
}

void tearDown(void) {
    remove("testwords_delims.txt");
    remove("testwords_simple.txt");
    remove("testwords_empty.txt");
}

// ---------- Test Runner ----------

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_readWords_simple);
    RUN_TEST(test_readWords_with_delimiters);
    RUN_TEST(test_readWords_empty_file);
    RUN_TEST(test_createWordSalad_all_fit);
    RUN_TEST(test_createWordSalad_too_small);

    int result = UNITY_END();  // Test-Ergebnisse
    print_test_result(result);

    return result;
}
