#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graphicalGame.h"
#include "raylib.h"

#define MAX_MESSAGE_LEN 256
#define MAX_SOLUTION_WORD_LEN 16

typedef struct
{
    Vector2 startPosition;
    Vector2 endPosition;
    int isSelected;
} MouseSelection;

typedef struct
{
    Vector2 position;
    char character[2];
    int isMarked;
} CharSquare;

typedef struct
{
    CharSquare *squares;
    unsigned int count;
    unsigned int squareSize;
    unsigned int fontSize;
    Vector2 position;
    Vector2 size;
} CharSquarePanel;

typedef struct
{
    char content[MAX_WORD_LEN];
    char *solution;
    Vector2 position;
    int wasFound;
} SearchWord;

typedef struct
{
    SearchWord *words;
    unsigned int count;
    int fontSize;
    Vector2 position;
    Vector2 size;
} SearchWordPanel;

typedef struct
{
    char content[MAX_MESSAGE_LEN];
    Vector2 position;
    unsigned int size;
} WinMessage;

typedef struct
{
    char text[MAX_MESSAGE_LEN];
    Vector2 position;
    Vector2 size;
    unsigned int fontSize;
} HelperMessage;

// Creates a helper message to guide the user
static HelperMessage createHelperMessage(unsigned int screenWidth)
{
    const char *text = "Please search below for the words located at the bottom \nand draw a line exactly on the desired characters ...";
    HelperMessage msg = {"", {0, 0}, {screenWidth, 0}, 18};

    // Copy text into msg, ensuring does not exceed max length
    strncpy(msg.text, text, MAX_MESSAGE_LEN);
    msg.text[MAX_MESSAGE_LEN-1] = '\0';

    // Set the vertical size based on font size
    msg.size.y = 3 * msg.fontSize;

    return msg;
}

// Creates a winning message when the user wins
static WinMessage createWinMessage(unsigned int screenSize)
{
    WinMessage winMsg;
    char *text = "Congratulations! You won!";

    strncpy(winMsg.content, text, MAX_MESSAGE_LEN);
    winMsg.content[MAX_MESSAGE_LEN-1] = '\0';
    winMsg.size = 30;  // Set font size

    // Calculate x and y positions for centering the message
    winMsg.position.x = (screenSize - strlen(winMsg.content)*winMsg.size*0.52) / 2;
    winMsg.position.y = screenSize / 2;

    return winMsg;
}

// Frees memory associated with a search word panel
static void freeSearchWordPanel(SearchWordPanel *panel)
{
    for(int i = 0; panel->words != NULL && i < panel->count; i++)
        free(panel->words[i].solution);  // Free solution strings
    free(panel->words);  // Free word array
    panel->words = NULL;
    panel->count = 0;
    panel->size.x = 0;
    panel->size.y = 0;
}

// Creates a panel to display a list of search words
static SearchWordPanel createSearchWordPanel(const char words[][MAX_WORD_LEN], unsigned int numberOfWords, unsigned int windowOffset)
{
    const int maxStringLenInPx = 200;  // Max width of each word
    const int fontSize = 18;  // Font size for displaying words
    const int rowHeight = fontSize * 1.2 + 5;  // Height of each row of words

    SearchWordPanel panel = {NULL, 0, fontSize, {0, windowOffset}, {windowOffset, 0}};

    unsigned int xOffset = 5;
    unsigned int yOffset = 15;

    // Allocate memory for words if any are present
    if(numberOfWords > 0)
        panel.words = (SearchWord *)malloc(sizeof(SearchWord) * numberOfWords);

    // If memory allocation is successful
    if(panel.words != NULL)
    {
        // Loop through and set up the words and their positions
        for(int i = 0; i < numberOfWords; i++)
        {
            strncpy(panel.words[i].content, words[i], MAX_SOLUTION_WORD_LEN);
            panel.words[i].content[MAX_SOLUTION_WORD_LEN-1] = '\0';

            // Truncate word if exceeds max length
            if(strlen(words[i]) > MAX_SOLUTION_WORD_LEN-1)
                strncpy(panel.words[i].content+MAX_SOLUTION_WORD_LEN-4, "...", 4);

            // Allocate memory for solution word
            panel.words[i].solution = (char *)malloc(sizeof(char) * (strlen(words[i]) + 1));

            if(panel.words[i].solution != NULL)
                strcpy(panel.words[i].solution, words[i]);
            else
            {
                freeSearchWordPanel(&panel);  // Free memory in case of failure
                numberOfWords = 0;
                break;
            }

            panel.words[i].wasFound = 0;  // Initialize "found" flag
            panel.words[i].position.x = xOffset;
            panel.words[i].position.y = yOffset;

            // Move to next position for next word
            xOffset += maxStringLenInPx + 5;

            // Move to next row if needed
            if(xOffset > windowOffset)
            {
                xOffset = 5;
                yOffset += rowHeight;
            }
        }
        panel.count = numberOfWords;  // Sets total word count

        // Adjust panel size based on last word's position
        if(numberOfWords > 0)
            panel.size.y = panel.words[numberOfWords - 1].position.y + rowHeight;
    }

    return panel;
}

// Creates a square for a character in the search grid
static CharSquare createSquare(unsigned int rowIdx, unsigned int colIdx, char character, unsigned int squareSize)
{
    CharSquare square;
    square.position.x = colIdx * squareSize;
    square.position.y = rowIdx * squareSize;
    square.character[0] = character;
    square.character[1] = '\0';
    square.isMarked = 0;  // Mark as unmarked initially

    return square;
}

// Creates a panel of character squares (the search grid)
static CharSquarePanel createCharSquarePanel(const char wordSalad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldSizeInChars, int panelSizeInPx)
{
    CharSquarePanel squarePanel;
    squarePanel.squares = (CharSquare *)malloc(sizeof(CharSquare) * searchFieldSizeInChars * searchFieldSizeInChars);
    squarePanel.count = 0;
    squarePanel.squareSize = (double)panelSizeInPx / searchFieldSizeInChars;  // Calculate the square size
    squarePanel.fontSize = squarePanel.squareSize * 0.75;  // Set font size relative to square size
    squarePanel.position.x = 0;
    squarePanel.position.y = 0;
    squarePanel.size.x = panelSizeInPx;
    squarePanel.size.y = panelSizeInPx;

    // If memory for squares is allocated successfully loop through grid and create squares for each character
    if(squarePanel.squares != NULL)
    {
        for(int i = 0; i < searchFieldSizeInChars; i++)
        {
            for(int j = 0; j < searchFieldSizeInChars; j++)
            {
                squarePanel.squares[squarePanel.count] = createSquare(i, j, wordSalad[i][j], squarePanel.squareSize);
                squarePanel.count++;
            }
        }
    }

    return squarePanel;
}

// Frees memory associated with CharSquarePanel
static void freeCharSquarePanel(CharSquarePanel *squarePanel)
{
    free(squarePanel->squares);  // Free squares array
    squarePanel->squares = NULL;
    squarePanel->count = 0;  // Reset count
}

// Draws all squares of CharSquarePanel
static void drawSquares(const CharSquarePanel squarePanel)
{
    float fontOffset = squarePanel.squareSize / 4;  // Offset for font positioning

    // Loop through all squares and draw them
    for(int i = 0; i < squarePanel.count; i++)
    {
        CharSquare square = squarePanel.squares[i];

        Vector2 squareScreenCoord = {squarePanel.position.x + square.position.x, squarePanel.position.y + square.position.y};
        Color squareColor = DARKBLUE;
        Color fontColor = LIGHTGRAY;

        // Change colors if is marked
        if(square.isMarked)
        {
            squareColor = GREEN;
            fontColor = BLACK;
        }

        // Draw square and its border
        DrawRectangle(squareScreenCoord.x, squareScreenCoord.y, squarePanel.squareSize, squarePanel.squareSize, squareColor);
        for(int i = 1; i <= 3; i++)
            DrawRectangleLines(squareScreenCoord.x, squareScreenCoord.y, squarePanel.squareSize-i, squarePanel.squareSize-i, LIGHTGRAY);

        // Draw character inside the square
        DrawText(square.character, squareScreenCoord.x + fontOffset, squareScreenCoord.y + fontOffset, squarePanel.fontSize, fontColor);
    }
}

// Checks if selected word is valid solution
static int isSolution(const char *solution, SearchWordPanel searchWordPanel)
{
    for(int i = 0; i < searchWordPanel.count; i++)
        if(strcmp(solution, searchWordPanel.words[i].solution) == 0)
        {
            // Mark word as found and return true if solution matches
            searchWordPanel.words[i].wasFound = 1;
            return 1;
        }

    return 0;  // false if not found
}

// Updates the marked squares based on user selection
static void updateSelectedSquares(const MouseSelection selection, CharSquarePanel squarePanel, SearchWordPanel searchWordPanel)
{
    unsigned int wordIdx = 0;
    char selectedWord[MAX_WORD_LEN];
    unsigned int selectedIdx[squarePanel.count];
    float radius = squarePanel.squareSize / 2;

    // Loop through all squares and check if selected
    for(int i = 0; i < squarePanel.count && wordIdx < MAX_WORD_LEN-1; i++)
    {
        Vector2 center = {squarePanel.squares[i].position.x + squarePanel.position.x, squarePanel.squares[i].position.y + squarePanel.position.y};
        center.x += radius;
        center.y += radius;

        // Check if square is selected by mouse
        if(CheckCollisionCircleLine(center, radius, selection.startPosition, selection.endPosition))
        {
            selectedWord[wordIdx] = squarePanel.squares[i].character[0];
            selectedIdx[wordIdx] = i;
            wordIdx++;
        }
    }
    selectedWord[wordIdx] = '\0';

    // If selected word is a solution, mark it
    if(isSolution(selectedWord, searchWordPanel))
    {
        for(int i = 0; i < wordIdx; i++)
            squarePanel.squares[selectedIdx[i]].isMarked = 1;
    }
}

// Handles mouse input for selecting words in grid
static void handleMouseInput(MouseSelection *selection, CharSquarePanel squarePanel, SearchWordPanel searchWordPanel)
{
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  // Start new selection
    {
        selection->startPosition = GetMousePosition();
        selection->endPosition = selection->startPosition;
        selection->isSelected = 1;
    }
    else if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))  // End selection
    {
        updateSelectedSquares(*selection, squarePanel, searchWordPanel);

        selection->isSelected = 0;
    }
    else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))  // Update end position while selecting
    {
        selection->endPosition = GetMousePosition();
    }
}

// Draws selection line on screen if selection is active
static void drawSelection(const MouseSelection selection)
{
    if(selection.isSelected)
    {
        DrawLine(selection.startPosition.x, selection.startPosition.y, selection.endPosition.x, selection.endPosition.y, WHITE);
    }
}

// Draws search word panel (list of words to be found)
static void drawSearchWordPanel(SearchWordPanel searchWordPanel)
{
    for(int i = 0; i < searchWordPanel.count; i++)
    {
        Vector2 wordScreenCoord = {searchWordPanel.words[i].position.x + searchWordPanel.position.x, searchWordPanel.words[i].position.y + searchWordPanel.position.y};
        DrawText(searchWordPanel.words[i].content, wordScreenCoord.x, wordScreenCoord.y, searchWordPanel.fontSize, WHITE);

        // If word has been found, highlight it
        if(searchWordPanel.words[i].wasFound)
        {
            int xOffset = MeasureText(searchWordPanel.words[i].content, searchWordPanel.fontSize);
            for(int i = 0; i <= 3; i++)
                DrawLine(wordScreenCoord.x - 3, wordScreenCoord.y + 5 + i, wordScreenCoord.x + xOffset + 3, wordScreenCoord.y + 5 + i, GREEN);
        }
    }
}

// Draws helper message (instructions or tips for user)
static void drawHelperMessage(const HelperMessage msg)
{
    DrawRectangle(msg.position.x, msg.position.y, msg.size.x, msg.size.y, BLACK);  // Background for message
    DrawText(msg.text, msg.position.x + 5, msg.position.y + 5, msg.fontSize, WHITE);  // Display message text
}

// Draws the entire game content, including helper message, squares, and search words
static void drawGameContent(const HelperMessage helperMsg, const CharSquarePanel squarePanel, const MouseSelection selection, const SearchWordPanel searchWordPanel)
{
    drawHelperMessage(helperMsg);
    drawSquares(squarePanel);
    drawSearchWordPanel(searchWordPanel);
    drawSelection(selection);
}

// Draws success message when player wins
static void drawSuccessContent(WinMessage msg)
{
    unsigned int textWidth = MeasureText(msg.content, msg.size);
    DrawRectangle(msg.position.x-20, msg.position.y-20, textWidth+40, msg.size+40, GREEN);  // Background for success message

    for(int i = 0; i < 5; i++)  // Draw borders around success message
        DrawRectangleLines(msg.position.x-20+i, msg.position.y-20+i, textWidth+40-i*2, msg.size+40-i*2, WHITE);

    DrawText(msg.content, msg.position.x, msg.position.y, msg.size, WHITE);  // Display success text
}

// Draws entire game screen, including game content and success message if applicable
static void drawAll(const CharSquarePanel squarePanel, const MouseSelection selection, const SearchWordPanel searchWordPanel, const HelperMessage helperMsg, const WinMessage msg, int hasWon)
{
    BeginDrawing();
    ClearBackground(BLACK);  // Clear screen with a black background
    drawGameContent(helperMsg, squarePanel, selection, searchWordPanel);  // Draw game content

    if(hasWon)  // If player has won, draw success message
        drawSuccessContent(msg);

    EndDrawing();
}

// Checks if all words in the search word panel have been found
static int allWordsFound(SearchWordPanel searchWordPanel)
{
    // Loop through all words and check if any is not found
    for(int i = 0; i < searchWordPanel.count; i++)
        if(!searchWordPanel.words[i].wasFound)
            return 0;  // Return false if any word is not found
    return 1;  // Return true if all words are found
}

// Main game loop where game is run and updated
static void gameLoop(const Vector2 screenSize, MouseSelection mouseSelection, CharSquarePanel squarePanel, SearchWordPanel searchWordPanel, const HelperMessage helperMsg,  const WinMessage winMsg)
{
    InitWindow(screenSize.x, screenSize.y, "Word Salad");  // Initialize game window

    SetTargetFPS(60);

    while (!WindowShouldClose())  // Keep running until window is closed
    {
        handleMouseInput(&mouseSelection, squarePanel, searchWordPanel);  // Handle mouse input (selection)

        // Draw all game content including helper message, squares, and search word panel
        drawAll(squarePanel, mouseSelection, searchWordPanel, helperMsg, winMsg, allWordsFound(searchWordPanel));
    }

    CloseWindow();
}

// Initializes and starts game, setting up necessary elements and entering game loop
void startGame(const char wordSalad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN], unsigned int searchFieldSize, char words[][MAX_WORD_LEN], unsigned int numberOfWords, unsigned int windowSize)
{
    const int windowWidth = windowSize;

    Vector2 screenSize;

    // Create necessary game elements like helper message, square panel, and search word panel
    HelperMessage helperMsg = createHelperMessage(windowWidth);
    CharSquarePanel squarePanel = createCharSquarePanel(wordSalad, searchFieldSize, windowWidth);
    SearchWordPanel searchWordPanel = createSearchWordPanel(words, numberOfWords, windowWidth);
    WinMessage winMsg = createWinMessage(windowWidth);

    MouseSelection mouseSelection;

    mouseSelection.isSelected = 0;  // Initialize mouse selection to not be active

    // Adjust panel positions
    squarePanel.position.y = helperMsg.size.y;
    searchWordPanel.position.y = helperMsg.size.y + squarePanel.size.y;

    // Set screen size based on the panels' sizes
    screenSize.x = squarePanel.size.x;
    screenSize.y = helperMsg.size.y + squarePanel.size.y + searchWordPanel.size.y;

    // Start game loop
    gameLoop(screenSize, mouseSelection, squarePanel, searchWordPanel, helperMsg, winMsg);

    // Free up allocated memory when game is done
    freeCharSquarePanel(&squarePanel);
    freeSearchWordPanel(&searchWordPanel);
}

/*gcc -fPIC -c input.c game.c graphicalGame.c main.c
gcc -shared -o libwortsalat.a input.o game.o graphicalGame.o main.o*/

/* Minimaler, sichtbar linker Stub. Ersetzt das durch die echte GUI-Implementierung. */
void startGraphicalGame(const char salad[MAX_SEARCH_FIELD_LEN][MAX_SEARCH_FIELD_LEN],
                        unsigned int searchFieldLen,
                        const char words[][MAX_WORD_LEN],
                        unsigned int wordCount)
{
    /* Fenstergröße basierend auf Suchfeldgröße (anpassen falls gewünscht) */
    unsigned int windowSize = searchFieldLen * 32 + 200;

    /* startGame erwartet 'char words[][MAX_WORD_LEN]' — const cast ist hier ok */
    startGame(salad, searchFieldLen, (char (*)[MAX_WORD_LEN])words, wordCount, windowSize);
}