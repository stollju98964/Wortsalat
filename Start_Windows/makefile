CC = gcc
CFLAGS = -g -Wall -I$(raylib_folder)
LDFLAGS = -lopengl32 -lgdi32 -lwinmm
BINARIES = ./windows

raylib_folder = ./raylib
unityfolder = ./unity

.PHONY: all clean test wordsalad_initial wordsalad_myversion

# --------------------------
# initiales Spiel bauen
# --------------------------
wordsalad_initial:
	$(CC) -o wordsalad_initial $(BINARIES)/libwordsalad_complete.a $(BINARIES)/libraylib.a $(LDFLAGS)

wordsalad_myversion: main.c graphicalGame.o $(BINARIES)/libwordsalad.a $(BINARIES)/libraylib.a
	$(CC) $(CFLAGS) -o wordsalad_myversion main.c graphicalGame.o $(BINARIES)/libwordsalad.a $(BINARIES)/libraylib.a $(LDFLAGS)

# --------------------------
# Normales Spiel bauen
# --------------------------
all: main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a
	$(CC) $(CFLAGS) -o wordsalad main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

input.o: input.c
	$(CC) -c $(CFLAGS) input.c

game.o: game.c
	$(CC) -c $(CFLAGS) game.c

graphicalGame.o: graphicalGame.c
	$(CC) $(CFLAGS) -I$(raylib_folder) -c graphicalGame.c

# --------------------------
# Unit Tests
# --------------------------
TEST_BIN = runTests

test: input.o game.o unit_tests.c
	$(CC) $(CFLAGS) -I$(unityfolder) -o $(TEST_BIN) input.o game.o unit_tests.c $(BINARIES)/libunity.a

# --------------------------
# Clean
# --------------------------
clean:
	del /f *.o *.exe
