#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>

#define MAX_COLLIDERS 64

typedef struct Collider
{
    float x;
    float z;
    float width;
    float depth;
    bool active;
} Collider;

typedef struct Scene
{
    float floor_size;

    Collider colliders[MAX_COLLIDERS];
    int collider_count;

    int exit_door_collider_index;
    bool exit_door_open;
} Scene;

void init_scene(Scene* scene);
void update_scene(Scene* scene, double delta_time);
void render_scene(const Scene* scene);
void destroy_scene(Scene* scene);

bool check_collision(const Scene* scene, float x, float z, float radius);

#endif /* SCENE_H */