CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -framework OpenGL -framework CoreFoundation -framework CoreGraphics -framework IOKit -framework Cocoa -framework CoreVideo
ARCH := $(shell uname -m)
BINARIES = ./macos-$(ARCH)

raylib_folder = ./raylib
unityfolder = ./unity

# --------------------------
# initiales Spiel bauen
# --------------------------
wordsalad_initial:
	$(CC) -o wordsalad_initial $(BINARIES)/libwordsalad_complete.a $(BINARIES)/libraylib.a $(LDFLAGS)

# --------------------------
# Normales Spiel bauen
# --------------------------
wordsalad: main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a
	$(CC) $(CFLAGS) -o wordsalad main.o input.o game.o graphicalGame.o $(BINARIES)/libraylib.a $(LDFLAGS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c

input.o: input.c
	$(CC) -c $(CFLAGS) input.c

game.o: game.c
	$(CC) -c $(CFLAGS) game.c

graphicalGame.o: graphicalGame.c
	$(CC) -I$(raylibfolder) -c $(CFLAGS) graphicalGame.c


# --------------------------
# Unit Tests
# --------------------------
TEST_BIN = runTests

test: input.o game.o unit_tests.c $(BINARIES)/libunity.a
	$(CC) -Wall -I$(unityfolder) -o $(TEST_BIN) input.o game.o unit_tests.c $(BINARIES)/libunity.a
# --------------------------
# Clean
# --------------------------
clean:
	rm -f *.o wordsalad
