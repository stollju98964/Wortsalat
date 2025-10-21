CC = gcc
CFLAGS = -g -Wall -I$(raylibfolder)
LDFLAGS = -lGL -lX11 -lm
BINARIES = ./linux

raylib_folder = ./raylib
unityfolder = ./unity

# --------------------------
# initiales Spiel bauen
# --------------------------
wordsalad_initial:
	$(CC) -o wordsalad_initial -L. $(BINARIES)/libwordsalad_complete.a $(BINARIES)/libraylib.a $(LDFLAGS)

# --------------------------
# Normales Spiel bauen
# --------------------------
wordsalad: main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a
	$(CC) $(CFLAGS) -o wordsalad main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

input.o: input.c
	$(CC) $(CFLAGS) -c input.c

game.o: game.c
	$(CC) $(CFLAGS) -c game.c

graphicalGame.o: graphicalGame.c
	$(CC) $(CFLAGS) -c graphicalGame.c

# --------------------------
# Unit Tests
# --------------------------
TEST_BIN = runTests

test: input.o game.o unit_tests.c
	$(CC) -I$(unityfolder) -o $(TEST_BIN) input.o game.o unit_tests.c $(BINARIES)/libunity.a

# --------------------------
# Clean
# --------------------------
clean:
	rm -f *.o wordsalad $(TEST_BIN)
