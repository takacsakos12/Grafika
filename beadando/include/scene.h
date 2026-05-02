#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include <GL/gl.h>

#include "model.h"

#define MAX_COLLIDERS 64
#define MAX_GENERATORS 3

typedef struct Collider
{
    float x;
    float z;
    float width;
    float depth;
    bool active;
} Collider;

typedef struct Generator
{
    float x;
    float z;
    bool active;
} Generator;

typedef struct Scene
{
    float floor_size;

    Collider colliders[MAX_COLLIDERS];
    int collider_count;

    int exit_door_collider_index;
    bool exit_door_open;

    Generator generators[MAX_GENERATORS];
    int generator_count;
    int active_generator_count;

    GLuint floor_texture;
    GLuint wall_texture;

    Model door_model;
    GLuint door_texture;

} Scene;

void init_scene(Scene* scene);
void update_scene(Scene* scene, double delta_time);
void render_scene(const Scene* scene);
void destroy_scene(Scene* scene);

bool check_collision(const Scene* scene, float x, float z, float radius);
void interact_scene(Scene* scene, float player_x, float player_z);

#endif /* SCENE_H */