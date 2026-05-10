#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include <GL/gl.h>

#include "model.h"

#define MAX_COLLIDERS 160
#define MAX_GENERATORS 3
#define DRONE_WAYPOINT_COUNT 2
#define MAX_DRONES 2
#define MAX_PARTICLES 200
#define SPARK_SOURCE_COUNT 3
#define MAX_INNER_DOORS 8
#define MAX_CRATES 32
#define MAX_PLANT_CAPSULES 40

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

typedef struct Particle
{
    float x;
    float y;
    float z;

    float vx;
    float vy;
    float vz;

    float life;
    float max_life;

    bool active;
} Particle;

typedef struct SparkSource
{
    float x;
    float y;
    float z;

    float timer;
    float next_emit_time;
} SparkSource;

typedef struct InnerDoor
{
    float x;
    float z;
    float rotation_y;

    bool open;
    float open_offset;
    float auto_close_timer;
    float hinge_side;
    float open_direction;

    int collider_index;
} InnerDoor;

typedef struct Crate
{
    float x;
    float y;
    float z;
    float rotation_y;
    float scale;
    int collider_index;
} Crate;

typedef struct PlantCapsule
{
    float x;
    float y;
    float z;
    float rotation_y;
    float scale;
    int collider_index;
} PlantCapsule;

typedef struct Scene
{
    float floor_size;

    Collider colliders[MAX_COLLIDERS];
    int collider_count;
    int wall_count;

    int exit_door_collider_index;
    bool exit_door_open;

    Generator generators[MAX_GENERATORS];
    int generator_count;
    int active_generator_count;

    GLuint floor_texture;
    GLuint wall_texture;
    GLuint ceiling_texture;
    InnerDoor inner_doors[MAX_INNER_DOORS];
    int inner_door_count;
    Model inner_door_model;
    GLuint inner_door_texture;
    Model door_frame_model;
    Model door_panel_model;
    GLuint door_texture;
    Model generator_model;
    Model crate_model;
    GLuint crate_texture;

    Model console_model;
    GLuint console_texture;

    Model desk_chair_model;
    GLuint desk_chair_texture;

    PlantCapsule plant_capsules[MAX_PLANT_CAPSULES];
    int plant_capsule_count;

    Model plant_capsule_model;
    GLuint plant_capsule_texture;

    GLuint generator_texture;
    float exit_door_offset;

    float brightness;
    bool hard_mode;

   Drone drones[MAX_DRONES];
    int drone_count;
    bool game_over;
    bool game_won;

    Particle particles[MAX_PARTICLES];
    SparkSource spark_sources[SPARK_SOURCE_COUNT];
    Crate crates[MAX_CRATES];
    int crate_count;

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
bool is_player_at_exit(const Scene* scene, float player_x, float player_z);

#endif /* SCENE_H */