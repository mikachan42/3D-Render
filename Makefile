CC = gcc
CFLAGS = -Wall -Wextra -std=c11
SDL_FLAGS = $(shell sdl2-config --cflags --libs)

TARGET = build/render
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(SDL_FLAGS) -lm

clean:
	rm -f $(TARGET)

.PHONY: all clean