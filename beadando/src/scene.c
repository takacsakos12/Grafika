#include "scene.h"
#include "texture.h"
#include "model.h"
#include "camera.h"

#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static float random_float(float min, float max)
{
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

static void init_particles(Scene* scene)
{
    int i;

    for (i = 0; i < MAX_PARTICLES; ++i) {
        scene->particles[i].active = false;
        scene->particles[i].life = 0.0f;
        scene->particles[i].max_life = 0.0f;
    }
}


static void emit_spark(Scene* scene, float x, float y, float z)
{
    int i;

    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            scene->particles[i].x = x;
            scene->particles[i].y = y;
            scene->particles[i].z = z;

            scene->particles[i].vx = random_float(-1.5f, 1.5f);
            scene->particles[i].vy = random_float(1.0f, 3.0f);
            scene->particles[i].vz = random_float(-1.5f, 1.5f);

            scene->particles[i].max_life = random_float(0.3f, 0.8f);
            scene->particles[i].life = scene->particles[i].max_life;

            scene->particles[i].active = true;

            return;
        }
    }
}


static void update_particles(Scene* scene, double delta_time)
{
    int i;
    int source_index;

    float dt = (float)delta_time;

    /* új szikrák kibocsátása időzítve */
    for (source_index = 0; source_index < SPARK_SOURCE_COUNT; ++source_index) {
        scene->spark_sources[source_index].timer += dt;

        if (scene->spark_sources[source_index].timer >= scene->spark_sources[source_index].next_emit_time) {
            int burst_count;
            int j;

            burst_count = rand() % 4 + 2; /* 2..5 szikra egyszerre */

            for (j = 0; j < burst_count; ++j) {
                emit_spark(
                    scene,
                    scene->spark_sources[source_index].x,
                    scene->spark_sources[source_index].y,
                    scene->spark_sources[source_index].z
                );
            }

            scene->spark_sources[source_index].timer = 0.0f;
            scene->spark_sources[source_index].next_emit_time = random_float(0.3f, 1.5f);
        }
    }

    /* meglévő szikrák mozgatása */
    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            continue;
        }

        scene->particles[i].life -= dt;

        if (scene->particles[i].life <= 0.0f) {
            scene->particles[i].active = false;
            continue;
        }

        scene->particles[i].x += scene->particles[i].vx * dt;
        scene->particles[i].y += scene->particles[i].vy * dt;
        scene->particles[i].z += scene->particles[i].vz * dt;

        scene->particles[i].vy -= 5.0f * dt;
    }
}

static void draw_particles(const Scene* scene)
{
    int i;
    float alpha;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glPointSize(4.0f);

    glBegin(GL_POINTS);

    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            continue;
        }

        alpha = scene->particles[i].life / scene->particles[i].max_life;

        glColor4f(1.0f, 0.65f, 0.05f, alpha);

        glVertex3f(
            scene->particles[i].x,
            scene->particles[i].y,
            scene->particles[i].z
        );
    }

    glEnd();

    glPointSize(1.0f);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
}

static void init_spark_sources(Scene* scene)
{
    scene->spark_sources[0].x = -8.0f;
    scene->spark_sources[0].y = 0.6f;
    scene->spark_sources[0].z = 3.0f;
    scene->spark_sources[0].timer = 0.0f;
    scene->spark_sources[0].next_emit_time = random_float(0.2f, 1.0f);

    scene->spark_sources[1].x = 7.0f;
    scene->spark_sources[1].y = 0.6f;
    scene->spark_sources[1].z = 2.0f;
    scene->spark_sources[1].timer = 0.0f;
    scene->spark_sources[1].next_emit_time = random_float(0.2f, 1.0f);
}
/* ---------- Scene helper functions ---------- */

static void add_collider(Scene* scene, float x, float z, float width, float depth)
{
    if (scene->collider_count >= MAX_COLLIDERS) {
        return;
    }

    scene->colliders[scene->collider_count].x = x;
    scene->colliders[scene->collider_count].z = z;
    scene->colliders[scene->collider_count].width = width;
    scene->colliders[scene->collider_count].depth = depth;
    scene->colliders[scene->collider_count].active = true;

    scene->collider_count++;
}

static const float drone_waypoints[DRONE_WAYPOINT_COUNT][2] = {
    {-12.0f, -12.0f},
    { 12.0f, -12.0f},
    { 12.0f,  12.0f},
    {-12.0f,  12.0f}
};

static void add_generator(Scene* scene, float x, float z)
{
    if (scene->generator_count >= MAX_GENERATORS) {
        return;
    }

    scene->generators[scene->generator_count].x = x;
    scene->generators[scene->generator_count].z = z;
    scene->generators[scene->generator_count].active = false;

    scene->generator_count++;
}

/* ---------- Drawing ---------- */

static void draw_floor(float size, GLuint texture)
{
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glNormal3f(0.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-size, 0.0f, -size);

    glTexCoord2f(8.0f, 0.0f);
    glVertex3f(size, 0.0f, -size);

    glTexCoord2f(8.0f, 8.0f);
    glVertex3f(size, 0.0f, size);

    glTexCoord2f(0.0f, 8.0f);
    glVertex3f(-size, 0.0f, size);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_box(float x, float z, float width, float height, float depth)
{
    float x1 = x - width / 2.0f;
    float x2 = x + width / 2.0f;
    float y1 = 0.0f;
    float y2 = height;
    float z1 = z - depth / 2.0f;
    float z2 = z + depth / 2.0f;

    float tx = width;
    float ty = height;
    float tz = depth;

    if (tx < 1.0f) {
        tx = 1.0f;
    }

    if (ty < 1.0f) {
        ty = 1.0f;
    }

    if (tz < 1.0f) {
        tz = 1.0f;
    }

    glBegin(GL_QUADS);

    /* front */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z2);

    glTexCoord2f(tx, 0.0f);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(tx, ty);
    glVertex3f(x2, y2, z2);

    glTexCoord2f(0.0f, ty);
    glVertex3f(x1, y2, z2);

    /* back */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(tx, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(tx, ty);
    glVertex3f(x1, y2, z1);

    glTexCoord2f(0.0f, ty);
    glVertex3f(x2, y2, z1);

    /* left */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(tz, 0.0f);
    glVertex3f(x1, y1, z2);

    glTexCoord2f(tz, ty);
    glVertex3f(x1, y2, z2);

    glTexCoord2f(0.0f, ty);
    glVertex3f(x1, y2, z1);

    /* right */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(tz, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(tz, ty);
    glVertex3f(x2, y2, z1);

    glTexCoord2f(0.0f, ty);
    glVertex3f(x2, y2, z2);

    /* top */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y2, z2);

    glTexCoord2f(tx, 0.0f);
    glVertex3f(x2, y2, z2);

    glTexCoord2f(tx, tz);
    glVertex3f(x2, y2, z1);

    glTexCoord2f(0.0f, tz);
    glVertex3f(x1, y2, z1);

    /* bottom */
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x1, y1, z1);

    glTexCoord2f(tx, 0.0f);
    glVertex3f(x2, y1, z1);

    glTexCoord2f(tx, tz);
    glVertex3f(x2, y1, z2);

    glTexCoord2f(0.0f, tz);
    glVertex3f(x1, y1, z2);

    glEnd();
}

static void draw_wall(float x, float z, float width, float depth, GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1.0f, 1.0f, 1.0f);

    draw_box(x, z, width, 3.0f, depth);

    glDisable(GL_TEXTURE_2D);
}

static void draw_exit_door(const Scene* scene)
{
    if (scene->exit_door_open) {
        return;
    }
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->door_texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, 19.6f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(0.03f, 0.03f, 0.03f);

    render_model(&scene->door_model);

    glPopMatrix();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_generator(const Generator* generator)
{
    if (generator->active) {
        glColor3f(0.0f, 0.8f, 1.0f);
    }
    else {
        glColor3f(0.1f, 0.6f, 0.2f);
    }

    draw_box(generator->x, generator->z, 1.0f, 1.0f, 1.0f);
}

/* ---------- Collision ---------- */

static float clamp_float(float value, float min, float max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

static bool circle_intersects_collider(
    float circle_x,
    float circle_z,
    float radius,
    const Collider* collider
)
{
    float min_x;
    float max_x;
    float min_z;
    float max_z;

    float closest_x;
    float closest_z;

    float dx;
    float dz;

    if (!collider->active) {
        return false;
    }

    min_x = collider->x - collider->width / 2.0f;
    max_x = collider->x + collider->width / 2.0f;

    min_z = collider->z - collider->depth / 2.0f;
    max_z = collider->z + collider->depth / 2.0f;

    closest_x = clamp_float(circle_x, min_x, max_x);
    closest_z = clamp_float(circle_z, min_z, max_z);

    dx = circle_x - closest_x;
    dz = circle_z - closest_z;

    return dx * dx + dz * dz < radius * radius;
}

bool check_collision(const Scene* scene, float x, float z, float radius)
{
    int i;

    for (i = 0; i < scene->collider_count; ++i) {
        if (circle_intersects_collider(x, z, radius, &scene->colliders[i])) {
            return true;
        }
    }

    return false;
}

/* ---------- Interaction ---------- */

void interact_scene(Scene* scene, float player_x, float player_z)
{
    int i;
    float dx;
    float dz;
    float distance_squared;
    float interaction_radius = 2.0f;

    for (i = 0; i < scene->generator_count; ++i) {
        if (scene->generators[i].active) {
            continue;
        }

        dx = player_x - scene->generators[i].x;
        dz = player_z - scene->generators[i].z;

        distance_squared = dx * dx + dz * dz;

        if (distance_squared <= interaction_radius * interaction_radius) {
            scene->generators[i].active = true;
            scene->active_generator_count++;

            printf("Generator activated: %d/%d\n",
                scene->active_generator_count,
                scene->generator_count
            );

            if (scene->active_generator_count == scene->generator_count) {
                scene->exit_door_open = true;

                if (scene->exit_door_collider_index >= 0) {
                    scene->colliders[scene->exit_door_collider_index].active = false;
                }

                printf("Exit door opened!\n");
            }

            return;
        }
    }

    printf("No interactable object nearby.\n");
}

void set_scene_lighting(const Scene* scene)
{
    float b = scene->brightness;

    GLfloat ambient_light[] = {
        0.05f * b,
        0.05f * b,
        0.07f * b,
        1.0f
    };

    GLfloat diffuse_light[] = {
        1.00f * b,
        1.00f * b,
        1.00f * b,
        1.0f
    };

    GLfloat specular_light[] = {
        0.20f * b,
        0.20f * b,
        0.20f * b,
        1.0f
    };

    GLfloat light_position[] = {
        0.0f,
        8.0f,
        0.0f,
        1.0f
    };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void change_scene_brightness(Scene* scene, float amount)
{
    scene->brightness += amount;

    if (scene->brightness > 2.0f) {
        scene->brightness = 2.0f;
    }

    if (scene->brightness < 0.2f) {
        scene->brightness = 0.2f;
    }

    printf("Brightness: %.1f\n", scene->brightness);
}

void apply_scene_fog(const Scene* scene)
{
    GLfloat fog_color[] = {0.10f, 0.11f, 0.12f, 1.0f};

    if (scene->hard_mode) {
        glEnable(GL_FOG);

        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fog_color);

        glFogf(GL_FOG_START, 0.25f);
        glFogf(GL_FOG_END, 18.0f);

        glHint(GL_FOG_HINT, GL_NICEST);

        glClearColor(0.10f, 0.11f, 0.12f, 1.0f);
    }
    else {
        glDisable(GL_FOG);
        glClearColor(0.02f, 0.02f, 0.03f, 1.0f);
    }
}

void toggle_hard_mode(Scene* scene)
{
    scene->hard_mode = !scene->hard_mode;

    if (scene->hard_mode) {
        printf("Hard Mode: ON\n");
    }
    else {
        printf("Hard Mode: OFF\n");
    }
}

static void init_drone(Drone* drone)
{
    drone->x = drone_waypoints[0][0];
    drone->y = 1.4f;
    drone->z = drone_waypoints[0][1];

    drone->speed = 2.0f;

    drone->detection_radius = 5.0f;
    drone->detection_angle = (float)M_PI / 2.0f;
    drone->direction_angle = 0.0f;

    drone->current_waypoint = 1;
}

static void update_drone(Drone* drone, double delta_time)
{
    float target_x;
    float target_z;

    float dx;
    float dz;
    float distance;
    float step;

    target_x = drone_waypoints[drone->current_waypoint][0];
    target_z = drone_waypoints[drone->current_waypoint][1];

    dx = target_x - drone->x;
    dz = target_z - drone->z;

    distance = sqrtf(dx * dx + dz * dz);

    if (distance < 0.1f) {
        drone->current_waypoint++;

        if (drone->current_waypoint >= DRONE_WAYPOINT_COUNT) {
            drone->current_waypoint = 0;
        }

        return;
    }

    drone->direction_angle = atan2f(dz, dx);

    step = drone->speed * (float)delta_time;

    drone->x += dx / distance * step;
    drone->z += dz / distance * step;
}

static bool is_player_detected_by_drone(const Drone* drone, float player_x, float player_z)
{
    float dx;
    float dz;
    float distance_squared;

    float player_angle;
    float angle_difference;

    dx = player_x - drone->x;
    dz = player_z - drone->z;

    distance_squared = dx * dx + dz * dz;

    if (distance_squared > drone->detection_radius * drone->detection_radius) {
        return false;
    }

    player_angle = atan2f(dz, dx);
    angle_difference = player_angle - drone->direction_angle;

    while (angle_difference > (float)M_PI) {
        angle_difference -= 2.0f * (float)M_PI;
    }

    while (angle_difference < -(float)M_PI) {
        angle_difference += 2.0f * (float)M_PI;
    }

    return fabsf(angle_difference) <= drone->detection_angle / 2.0f;
}

static void draw_detection_sector(float x, float z, float radius, float direction_angle, float sector_angle)
{
    int i;
    int segments = 32;

    float start_angle;
    float end_angle;
    float angle;

    start_angle = direction_angle - sector_angle / 2.0f;
    end_angle = direction_angle + sector_angle / 2.0f;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 0.0f, 0.0f, 0.22f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex3f(x, 0.04f, z);

    for (i = 0; i <= segments; ++i) {
        angle = start_angle + (end_angle - start_angle) * ((float)i / (float)segments);

        glVertex3f(
            x + cosf(angle) * radius,
            0.04f,
            z + sinf(angle) * radius
        );
    }

    glEnd();

    glDisable(GL_BLEND);

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);

    for (i = 0; i <= segments; ++i) {
        angle = start_angle + (end_angle - start_angle) * ((float)i / (float)segments);

        glVertex3f(
            x + cosf(angle) * radius,
            0.05f,
            z + sinf(angle) * radius
        );
    }

    glEnd();

    glBegin(GL_LINES);

    glVertex3f(x, 0.05f, z);
    glVertex3f(
        x + cosf(start_angle) * radius,
        0.05f,
        z + sinf(start_angle) * radius
    );

    glVertex3f(x, 0.05f, z);
    glVertex3f(
        x + cosf(end_angle) * radius,
        0.05f,
        z + sinf(end_angle) * radius
    );

    glEnd();

    glEnable(GL_LIGHTING);
}

static void draw_drone(const Drone* drone)
{
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(drone->x, drone->y, drone->z);
    glRotatef(-drone->direction_angle * 180.0f / (float)M_PI, 0.0f, 1.0f, 0.0f);

    /* test */
    glColor3f(0.15f, 0.15f, 0.18f);
    draw_box(0.0f, 0.0f, 0.8f, 0.25f, 0.5f);

    /* elülső jelzőfény */
    glColor3f(1.0f, 0.0f, 0.0f);
    draw_box(0.45f, 0.0f, 0.12f, 0.12f, 0.12f);

    /* bal kar */
    glColor3f(0.25f, 0.25f, 0.28f);
    draw_box(0.0f, -0.45f, 0.25f, 0.08f, 0.9f);

    /* jobb kar */
    draw_box(0.0f, 0.45f, 0.25f, 0.08f, 0.9f);

    glPopMatrix();
}

void reset_scene(Scene* scene)
{
    int i;

    scene->exit_door_open = false;

    if (scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = true;
    }

    scene->active_generator_count = 0;

    for (i = 0; i < scene->generator_count; ++i) {
        scene->generators[i].active = false;
    }

    scene->game_over = false;
    scene->game_won = false;

    init_drone(&scene->drone);
    init_particles(scene);
    init_spark_sources(scene);

    printf("Game restarted.\n");
}

bool is_player_at_exit(const Scene* scene, float player_x, float player_z)
{
    float exit_x = 0.0f;
    float exit_z = 19.0f;
    float exit_radius = 2.0f;

    float dx;
    float dz;
    float distance_squared;

    if (!scene->exit_door_open) {
        return false;
    }

    dx = player_x - exit_x;
    dz = player_z - exit_z;

    distance_squared = dx * dx + dz * dz;

    return distance_squared <= exit_radius * exit_radius;
}



/* ---------- Scene lifecycle ---------- */

void init_scene(Scene* scene)
{
    scene->floor_size = 20.0f;

    scene->collider_count = 0;

    scene->exit_door_open = false;
    scene->exit_door_collider_index = -1;

    scene->generator_count = 0;
    scene->active_generator_count = 0;
    scene->brightness = 1.0f;
    scene->hard_mode = false;
    scene->game_over = false;
    scene->game_won = false;
    init_drone(&scene->drone);

    srand((unsigned int)time(NULL));
    init_particles(scene);
    init_spark_sources(scene);
    scene->floor_texture = load_texture("assets/textures/floor.bmp");
    scene->wall_texture = load_texture("assets/textures/wall.bmp");
    scene->door_texture = load_texture("assets/textures/door.bmp");
    load_model(&scene->door_model, "assets/models/door.obj");

    /* Kulso falak */
    add_collider(scene, 0.0f, -20.0f, 40.0f, 0.4f);
    add_collider(scene, 0.0f,  20.0f, 40.0f, 0.4f);
    add_collider(scene, -20.0f, 0.0f, 0.4f, 40.0f);
    add_collider(scene,  20.0f, 0.0f, 0.4f, 40.0f);

    /* Belso falak */
    add_collider(scene, -3.0f, 0.0f, 0.4f, 8.0f);
    add_collider(scene,  4.0f, -2.0f, 0.4f, 6.0f);
    add_collider(scene,  1.0f, 4.0f, 6.0f, 0.4f);

    /* Kijarati ajto */
    scene->exit_door_collider_index = scene->collider_count;
    add_collider(scene, 0.0f, 19.7f, 3.0f, 0.4f);

    /* Generatorok */
    add_generator(scene, -6.0f, -6.0f);
    add_generator(scene,  6.0f, -6.0f);
    add_generator(scene,  6.0f,  6.0f);

    /* Generatorok colliderjei */
    add_collider(scene, -6.0f, -6.0f, 1.0f, 1.0f);
    add_collider(scene,  6.0f, -6.0f, 1.0f, 1.0f);
    add_collider(scene,  6.0f,  6.0f, 1.0f, 1.0f);
}

void update_scene(Scene* scene, double delta_time, float player_x, float player_z)
{
    if (scene->exit_door_open && scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = false;
    }

    update_particles(scene, delta_time);

    if (scene->game_over || scene->game_won) {
        return;
    }

    update_drone(&scene->drone, delta_time);

    if (is_player_detected_by_drone(&scene->drone, player_x, player_z)) {
        scene->game_over = true;
        printf("GAME OVER: Drone detected the player!\n");
        return;
    }

    if (is_player_at_exit(scene, player_x, player_z)) {
        scene->game_won = true;
        printf("MISSION COMPLETE: Player escaped!\n");
        return;
    }
}

void render_scene(const Scene* scene)
{
    int i;

    draw_floor(scene->floor_size, scene->floor_texture);

    /* 0-3: kulso falak, 4-6: belso falak */
    for (i = 0; i < 7; ++i) {
        draw_wall(
            scene->colliders[i].x,
            scene->colliders[i].z,
            scene->colliders[i].width,
            scene->colliders[i].depth,
            scene->wall_texture
        );
    }

    draw_exit_door(scene);

    for (i = 0; i < scene->generator_count; ++i) {
        draw_generator(&scene->generators[i]);
    }

    draw_detection_sector(
    scene->drone.x,
    scene->drone.z,
    scene->drone.detection_radius,
    scene->drone.direction_angle,
    scene->drone.detection_angle
    );

    draw_drone(&scene->drone);
    draw_particles(scene);
}

void destroy_scene(Scene* scene)
{
    if (scene->floor_texture != 0) {
        glDeleteTextures(1, &scene->floor_texture);
        scene->floor_texture = 0;
    }

    if (scene->wall_texture != 0) {
        glDeleteTextures(1, &scene->wall_texture);
        scene->wall_texture = 0;
    }

    if (scene->door_texture != 0) {
        glDeleteTextures(1, &scene->door_texture);
        scene->door_texture = 0;
    }

    destroy_model(&scene->door_model);
}