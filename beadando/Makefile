CC = gcc

TARGET = game

SRC = src/main.c \
      src/app.c \
      src/camera.c \
      src/scene.c \
      src/utils.c \
      src/texture.c \
      src/model.c \
      src/scene_builder.c \
      src/scene_map.c \
      src/scene_collision.c \
      src/scene_particles.c \
      src/scene_drone.c \
      src/scene_render.c \
      src/scene_assets.c \
      src/scene_interaction.c \
      src/scene_environment.c \
      src/scene_update.c

CFLAGS = -Iinclude -Wall -Wextra -std=c99

LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lopengl32 -lm

all:
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET) $(LIBS)

run: all
	./$(TARGET)

clean:
	del $(TARGET).exe