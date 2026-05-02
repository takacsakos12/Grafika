#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include <GL/gl.h>

#include "model.h"

#define MAX_COLLIDERS 64
#define MAX_GENERATORS 3
#define DRONE_WAYPOINT_COUNT 4

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

typedef struct Drone
{
    float x;
    float y;
    float z;

    float speed;

    float detection_radius;
    float detection_angle;
    float direction_angle;

    int current_waypoint;
} Drone;

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
    float brightness;
    bool hard_mode;

    Drone drone;
    bool game_over;

} Scene;

void init_scene(Scene* scene);
void update_scene(Scene* scene, double delta_time, float player_x, float player_z);
void render_scene(const Scene* scene);

void destroy_scene(Scene* scene);

bool check_collision(const Scene* scene, float x, float z, float radius);
void interact_scene(Scene* scene, float player_x, float player_z);
void set_scene_lighting(const Scene* scene);
void change_scene_brightness(Scene* scene, float amount);
void toggle_hard_mode(Scene* scene);
void apply_scene_fog(const Scene* scene);
void reset_scene(Scene* scene);

#endif /* SCENE_H */