CC = gcc

TARGET = game

SRC = src/main.c src/app.c src/camera.c src/scene.c src/utils.c

CFLAGS = -Iinclude -Wall -Wextra -std=c99

LIBS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lm

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	del $(TARGET).exe