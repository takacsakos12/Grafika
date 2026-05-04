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

static void draw_static_model(
    const Model* model,
    GLuint texture,
    float x,
    float y,
    float z,
    float rotation_y,
    float scale
)
{
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    glTranslatef(x, y, z);
    glRotatef(rotation_y, 0.0f, 1.0f, 0.0f);
    glScalef(scale, scale, scale);

    render_model(model);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
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
    scene->spark_sources[0].x = -16.5f;
    scene->spark_sources[0].y = 0.8f;
    scene->spark_sources[0].z = 5.5f;
    scene->spark_sources[0].timer = 0.0f;
    scene->spark_sources[0].next_emit_time = random_float(0.2f, 1.0f);

    scene->spark_sources[1].x = 13.0f;
    scene->spark_sources[1].y = 0.8f;
    scene->spark_sources[1].z = 3.0f;
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

static const float drone_waypoints[MAX_DRONES][DRONE_WAYPOINT_COUNT][2] = {
    {
        {-8.0f, -3.5f},
        {-8.0f,  7.5f}
    },
    {
        {16.0f, -6.5f},
        {16.0f,  8.5f}
    }
};

static void draw_drone_path(int index)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.8f, 0.0f);

    glBegin(GL_LINES);

    glVertex3f(
        drone_waypoints[index][0][0],
        0.06f,
        drone_waypoints[index][0][1]
    );

    glVertex3f(
        drone_waypoints[index][1][0],
        0.06f,
        drone_waypoints[index][1][1]
    );

    glEnd();

    glEnable(GL_LIGHTING);
}

static void add_crate(
    Scene* scene,
    float x,
    float y,
    float z,
    float rotation_y,
    float scale
)
{
    if (scene->crate_count >= MAX_CRATES) {
        return;
    }

    scene->crates[scene->crate_count].x = x;
    scene->crates[scene->crate_count].y = y;
    scene->crates[scene->crate_count].z = z;
    scene->crates[scene->crate_count].rotation_y = rotation_y;
    scene->crates[scene->crate_count].scale = scale;
    scene->crates[scene->crate_count].collider_index = -1;

    scene->crate_count++;
}

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

static void add_hwall(Scene* scene, float x1, float x2, float z)
{
    float center_x = (x1 + x2) / 2.0f;
    float width = fabsf(x2 - x1);

    add_collider(scene, center_x, z, width, 0.4f);
}

static void add_vwall(Scene* scene, float x, float z1, float z2)
{
    float center_z = (z1 + z2) / 2.0f;
    float depth = fabsf(z2 - z1);

    add_collider(scene, x, center_z, 0.4f, depth);
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

static void draw_ceiling(float size, GLuint texture)
{
    float y = 3.0f;

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(0.75f, 0.75f, 0.75f);

    glBegin(GL_QUADS);

    /*
        Lefelé néző normál, mert alulról látjuk a tetőt.
    */
    glNormal3f(0.0f, -1.0f, 0.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-size, y,  size);

    glTexCoord2f(8.0f, 0.0f);
    glVertex3f( size, y,  size);

    glTexCoord2f(8.0f, 8.0f);
    glVertex3f( size, y, -size);

    glTexCoord2f(0.0f, 8.0f);
    glVertex3f(-size, y, -size);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_exit_door(const Scene* scene)
{
    float door_x = 3.0f;
    float door_y = 0.0f;
    float door_z = 19.6f;
    float door_scale = 3.0f;
    float max_visible_offset = 2.0f;

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->door_texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1.0f, 1.0f, 1.0f);

    /* fix ajtókeret */
    glPushMatrix();

    glTranslatef(door_x, door_y, door_z);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(door_scale, door_scale, door_scale);

    render_model(&scene->door_frame_model);

    glPopMatrix();

    /*
        mozgó ajtópanel
        Ha már teljesen elcsúszott, nem rajzoljuk tovább,
        így nem látszik ki oldalt.
    */
    if (scene->exit_door_offset < max_visible_offset) {
        glPushMatrix();

        glTranslatef(
            door_x + scene->exit_door_offset,
            door_y,
            door_z
        );

        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(door_scale, door_scale, door_scale);

        render_model(&scene->door_panel_model);

        glPopMatrix();
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_generator(const Scene* scene, const Generator* generator)
{
    float generator_scale = 0.01f;
    float generator_y = 0.5f;

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->generator_texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    if (generator->active) {
        glColor3f(0.5f, 1.0f, 1.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    glPushMatrix();

    glTranslatef(generator->x, generator_y, generator->z);
    glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
    glScalef(generator_scale, generator_scale, generator_scale);

    render_model(&scene->generator_model);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void add_inner_door(
    Scene* scene,
    float x,
    float z,
    float rotation_y,
    float hinge_side,
    float open_direction
)
{
    if (scene->inner_door_count >= MAX_INNER_DOORS) {
        return;
    }

    scene->inner_doors[scene->inner_door_count].x = x;
    scene->inner_doors[scene->inner_door_count].z = z;
    scene->inner_doors[scene->inner_door_count].rotation_y = rotation_y;

    scene->inner_doors[scene->inner_door_count].hinge_side = hinge_side;
    scene->inner_doors[scene->inner_door_count].open_direction = open_direction;

    scene->inner_doors[scene->inner_door_count].open = false;
    scene->inner_doors[scene->inner_door_count].open_offset = 0.0f;
    scene->inner_doors[scene->inner_door_count].auto_close_timer = 0.0f;

    scene->inner_doors[scene->inner_door_count].collider_index = scene->collider_count;

    if ((int)rotation_y == 90 || (int)rotation_y == 270) {
    add_collider(scene, x, z, 0.4f, 1.5f);
    }
    else {
    add_collider(scene, x, z, 1.5f, 0.4f);
}

    scene->inner_door_count++;
}

static void draw_inner_door(const Scene* scene, const InnerDoor* door)
{
    float door_y = 0.0f;
    float door_scale = 1.5f;

    /*
        Az OBJ ajtó szélessége lokális Z irányban van.
        Raw Z méret kb. 0.99, scale 1.5 mellett kb. 1.48.
        Ezért a fél szélesség kb. 0.74.
    */
    float hinge_offset = 0.74f;

    /*
        Az OBJ nincs teljesen középen a saját Z tengelyén.
        Ez korrigálja, hogy ne egyik oldalon legyen nagyobb rés.
    */
    float model_center_fix_z = -0.10f;

    float base_rotation = door->rotation_y + 90.0f;
    float angle = door->open_offset * door->open_direction;

    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->inner_door_texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();

    /*
        1. Ajtó közepe a pályán.
    */
    glTranslatef(door->x, door_y, door->z);

    /*
        2. Alap irány.
    */
    glRotatef(base_rotation, 0.0f, 1.0f, 0.0f);

    /*
        3. Zsanér az ajtó egyik oldalán.
        Fontos: itt Z irányban megyünk ki az oldalára, nem X irányban.
    */
    glTranslatef(0.0f, 0.0f, door->hinge_side * hinge_offset);

    /*
        4. Forgatás a zsanér körül.
    */
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    /*
        5. Vissza az ajtó közepére.
    */
    glTranslatef(0.0f, 0.0f, -door->hinge_side * hinge_offset);

    /*
        6. OBJ középre igazítás + méretezés.
    */
    glTranslatef(0.0f, 0.0f, model_center_fix_z);
    glScalef(door_scale, door_scale, door_scale);

    render_model(&scene->inner_door_model);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
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

static bool check_wall_collision(const Scene* scene, float x, float z, float radius)
{
    int i;

    for (i = 0; i < scene->wall_count; ++i) {
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

    /* 1. Belső ajtók */
    for (i = 0; i < scene->inner_door_count; ++i) {
        dx = player_x - scene->inner_doors[i].x;
        dz = player_z - scene->inner_doors[i].z;

        distance_squared = dx * dx + dz * dz;

        if (distance_squared <= interaction_radius * interaction_radius) {
            InnerDoor* door = &scene->inner_doors[i];

            door->open = !door->open;
            door->auto_close_timer = 0.0f;

            if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = false;
            }

            if (door->open) {
                printf("Inner door opened.\n");
            }
            else {
                printf("Inner door closing.\n");
            }

            return;
        }
    }

    /* 2. Generátorok */
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

static void init_drone(Drone* drone, int index)
{
    drone->x = drone_waypoints[index][0][0];
    drone->y = 1.4f;
    drone->z = drone_waypoints[index][0][1];

    drone->speed = 1.8f;

    drone->detection_radius = 5.0f;
    drone->detection_angle = (float)M_PI / 2.0f;
    drone->direction_angle = 0.0f;

    drone->current_waypoint = 1;
}

static void update_drone(Scene* scene, Drone* drone, int index, double delta_time)
{
    float target_x;
    float target_z;

    float dx;
    float dz;
    float distance;
    float step;

    target_x = drone_waypoints[index][drone->current_waypoint][0];
    target_z = drone_waypoints[index][drone->current_waypoint][1];

    dx = target_x - drone->x;
    dz = target_z - drone->z;

    distance = sqrtf(dx * dx + dz * dz);

    if (distance < 0.1f) {
    if (drone->current_waypoint == 0) {
        drone->current_waypoint = 1;
    }
    else {
        drone->current_waypoint = 0;
    }

    return;
}

    drone->direction_angle = atan2f(dz, dx);

    step = drone->speed * (float)delta_time;

    
    (void)scene;

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


bool is_player_at_exit(const Scene* scene, float player_x, float player_z)
{
    float exit_x = -1.0f;
    float exit_z = 19.0f;
    float exit_radius = 0.5f;

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


void reset_scene(Scene* scene)
{
    int i;

    scene->exit_door_open = false;
    scene->exit_door_offset = 0.0f;

    if (scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = true;
    }

    scene->active_generator_count = 0;

    for (i = 0; i < scene->generator_count; ++i) {
        scene->generators[i].active = false;
    }

    for (i = 0; i < scene->inner_door_count; ++i) {
    scene->inner_doors[i].open = false;
    scene->inner_doors[i].open_offset = 0.0f;
    scene->inner_doors[i].auto_close_timer = 0.0f;

    if (scene->inner_doors[i].collider_index >= 0) {
        scene->colliders[scene->inner_doors[i].collider_index].active = true;
    }
    }

    scene->game_over = false;
    scene->game_won = false;

    init_drone(&scene->drones[0], 0);
    init_drone(&scene->drones[1], 1);
    init_particles(scene);
    init_spark_sources(scene);

    printf("Game restarted.\n");
}
void init_scene(Scene* scene)
{
    scene->floor_size = 20.0f;

    scene->collider_count = 0;
    scene->wall_count = 0;
    scene->crate_count = 0;

    scene->exit_door_open = false;
    scene->exit_door_collider_index = -1;
    scene->exit_door_offset = 0.0f;

    scene->generator_count = 0;
    scene->active_generator_count = 0;
    scene->brightness = 1.0f;
    scene->hard_mode = false;
    scene->game_over = false;
    scene->game_won = false;
    scene->drone_count = MAX_DRONES;
    scene->inner_door_count = 0;
    init_drone(&scene->drones[0], 0);
    init_drone(&scene->drones[1], 1);

    srand((unsigned int)time(NULL));
    init_particles(scene);
    init_spark_sources(scene);
    scene->floor_texture = load_texture("assets/textures/floor.bmp");
    scene->wall_texture = load_texture("assets/textures/wall.bmp");
    scene->door_texture = load_texture("assets/textures/door.bmp");
    scene->ceiling_texture = load_texture("assets/textures/roof.bmp");
    if (!load_model(&scene->door_frame_model, "assets/models/door_frame.obj")) {
    printf("Failed to load door_frame.obj\n");
    }

    if (!load_model(&scene->door_panel_model, "assets/models/door_panel.obj")) {
    printf("Failed to load door_panel.obj\n");
    }
    scene->generator_texture = load_texture("assets/textures/generator.bmp");

    if (!load_model(&scene->generator_model, "assets/models/generator.obj")) {
    printf("Failed to load generator.obj\n");
    }
    scene->inner_door_texture = load_texture("assets/textures/inner_door.bmp");

    if (!load_model(&scene->inner_door_model, "assets/models/inner_door.obj")) {
    printf("Failed to load inner_door.obj\n");
    }

    /* Sci-fi crate
   https://sketchfab.com/3d-models/sci-fi-crate-df5fd0e719cb409698955384dab4a533
    */
    scene->crate_texture = load_texture("assets/textures/crate.bmp");

    if (!load_model(&scene->crate_model, "assets/models/crate.obj")) {
    printf("Failed to load crate.obj\n");
    }

    /* Sci-fi computer desk / console
   https://sketchfab.com/3d-models/sci-fi-computer-desk-console-28c67457f2ef4973a0bbc8b667bb183f
    */
    scene->console_texture = load_texture("assets/textures/console.bmp");

    if (!load_model(&scene->console_model, "assets/models/console.obj")) {
    printf("Failed to load console.obj\n");
    }

    /* Sci-fi desk and chair
   https://sketchfab.com/3d-models/sci-fi-desk-and-chair-677c70fe6e614791ba7ebd96d7e3101c
    */
    scene->desk_chair_texture = load_texture("assets/textures/desk_chair.bmp");

    if (!load_model(&scene->desk_chair_model, "assets/models/desk_chair.obj")) {
    printf("Failed to load desk_chair.obj\n");
    }

    /* Sci-fi plant capsule
   https://sketchfab.com/3d-models/sci-fi-plant-capsule-bd2d05db1bf24143ac2030c57e1eb05e
    */
    scene->plant_capsule_texture = load_texture("assets/textures/plant_capsule.bmp");

    if (!load_model(&scene->plant_capsule_model, "assets/models/plant_capsule.obj")) {
    printf("Failed to load plant_capsule.obj\n");
    }

    /* =========================
   FALAK A VEGLEGES GEOGEBRA RAJZ ALAPJAN
   JAVITVA:
   - inner_door atjarok egységesen kb. 1.5 szelesek
   - x = jatek x
   - GeoGebra y = jatek z
   ========================= */

    /* Kulso falak */
    add_vwall(scene, -20.0f, -20.0f, 20.0f);
    add_hwall(scene, -20.0f, 20.0f, -20.0f);
    add_vwall(scene, 20.0f, -20.0f, 20.0f);

    /* Felso fal kijarati nyilassal */
    add_hwall(scene, 1.0f, 20.0f, 20.0f);
    add_hwall(scene, -20.0f, -2.0f, 20.0f);

/* Bal oldal / bal felso */
    add_hwall(scene, -20.0f, -10.0f, 0.0f);
    add_hwall(scene, -20.0f, -14.5f, 16.0f);
    add_vwall(scene, -13.0f, 16.0f, 20.0f);
    add_hwall(scene, -20.0f, -19.0f, 10.0f);
    add_hwall(scene, -17.5f, -17.0f, 10.0f);
    add_vwall(scene, -17.0f, 10.0f, 11.0f);
    add_hwall(scene, -17.0f, -14.0f, 11.0f);
    add_hwall(scene, -14.0f, -13.0f, 11.0f);
    add_vwall(scene, -13.0f, 11.0f, 13.0f);
    add_vwall(scene, -13.0f, 14.0f, 16.0f);

    /* Felso kozep / kijarat kornyeke */
    add_hwall(scene, 1.0f, 5.0f, 17.0f);
    add_vwall(scene, 5.0f, 10.75f, 17.0f);
    add_vwall(scene, -2.0f, 18.0f, 20.0f);
    add_hwall(scene, -5.0f, -2.0f, 15.0f);
    add_vwall(scene, -5.0f, 13.0f, 15.0f);
    add_vwall(scene, -2.0f, 15.0f, 18.0f);

    /* x = -5 ajto: kozep z = 10.0, nyilas z = 9.25 .. 10.75 */
    add_vwall(scene, -5.0f, 10.75f, 13.0f);

    add_hwall(scene, -11.5f, -5.0f, 12.0f);
    add_vwall(scene, -11.5f, 12.0f, 16.0f);
    add_vwall(scene, -11.5f, 16.0f, 18.5f);
    add_hwall(scene, -11.5f, -9.5f, 18.5f);
    add_vwall(scene, -9.5f, 17.0f, 18.5f);
    add_vwall(scene, -8.0f, 15.5f, 20.0f);
    add_hwall(scene, -10.0f, -8.0f, 15.5f);
    add_vwall(scene, -10.0f, 13.5f, 15.5f);
    add_hwall(scene, -10.0f, -8.0f, 13.5f);

/* Jobb felso */
    add_vwall(scene, 11.0f, 10.0f, 20.0f);
    add_hwall(scene, 11.0f, 18.5f, 10.0f);
    add_vwall(scene, 18.5f, 10.0f, 12.0f);
    add_hwall(scene, 13.0f, 18.5f, 12.0f);
    add_hwall(scene, 13.0f, 20.0f, 15.0f);
    add_vwall(scene, 13.0f, 15.0f, 18.0f);

    /* Also kozep / start */
    add_vwall(scene, -2.0f, -20.0f, -12.0f);
    add_vwall(scene, 2.0f, -20.0f, -10.0f);
    add_hwall(scene, -6.0f, -2.0f, -10.0f);
    add_hwall(scene, 2.0f, 4.0f, -10.0f);
    add_vwall(scene, -6.0f, -10.0f, -9.0f);
    add_hwall(scene, -7.0f, -6.0f, -9.0f);
    add_vwall(scene, -7.0f, -20.0f, -9.0f);

    /* Bal also */
    add_hwall(scene, -10.0f, -7.0f, -9.0f);
    add_vwall(scene, -10.0f, -9.0f, -6.0f);
    add_hwall(scene, -14.0f, -10.0f, -6.0f);

    /* z = -6 ajto: kozep x = -15.5, nyilas x = -16.25 .. -14.75 */
    add_hwall(scene, -20.0f, -16.25f, -6.0f);
    add_hwall(scene, -14.75f, -10.0f, -6.0f);

    /* x = -10 ajto: kozep z = 1.5, nyilas z = 0.75 .. 2.25 */
    add_vwall(scene, -10.0f, -2.0f, 0.75f);

    add_hwall(scene, -20.0f, -16.0f, -14.0f);

    /* x = -16 ajto: kozep z = -13.0, nyilas z = -13.75 .. -12.25 */
    add_vwall(scene, -16.0f, -16.0f, -13.75f);

    add_hwall(scene, -16.0f, -10.0f, -16.0f);
    add_vwall(scene, -10.0f, -16.0f, -14.0f);

    add_hwall(scene, -20.0f, -14.0f, -8.0f);
    add_vwall(scene, -14.0f, -10.0f, -8.0f);
    add_hwall(scene, -16.0f, -14.0f, -10.0f);

    /* x = -16 ajto masik oldala */
    add_vwall(scene, -16.0f, -12.25f, -10.0f);

    add_hwall(scene, -16.0f, -14.0f, -12.0f);

    /* Bal kozep / szikrazo oldal */

    /* x = -10 ajto masik oldala */
    add_vwall(scene, -10.0f, 2.25f, 9.0f);

    add_hwall(scene, -10.0f, -6.0f, 9.0f);
    add_hwall(scene, -6.0f, -2.5f, 9.0f);
    add_vwall(scene, -2.5f, 9.0f, 11.0f);
    add_hwall(scene, -2.5f, 1.0f, 11.0f);
    add_vwall(scene, 1.0f, 9.0f, 11.0f);
    add_hwall(scene, 1.0f, 2.0f, 9.0f);

    add_vwall(scene, -14.0f, 8.0f, 11.0f);
    add_hwall(scene, -15.0f, -14.0f, 8.0f);
    add_vwall(scene, -15.0f, 4.0f, 8.0f);
    add_hwall(scene, -15.0f, -13.0f, 4.0f);
    add_vwall(scene, -13.0f, 2.0f, 4.0f);
    add_vwall(scene, -16.0f, 0.2f, 2.0f);
    add_hwall(scene, -16.0f, -14.5f, 2.0f);
    add_hwall(scene, -20.0f, -18.0f, 5.0f);
    add_vwall(scene, -18.0f, 5.0f, 6.0f);
    add_hwall(scene, -18.0f, -17.0f, 6.0f);
    add_vwall(scene, -17.0f, 5.0f, 6.0f);
    add_hwall(scene, -17.0f, -15.0f, 5.0f);

    /* Kozepso nagy terem */

    /* x = 5 ajto: kozep z = 10.0, nyilas z = 9.25 .. 10.75 */
    add_vwall(scene, 5.0f, -5.0f, 9.25f);

    add_vwall(scene, -6.0f, -5.0f, 9.0f);
    add_hwall(scene, -4.0f, 5.0f, -5.0f);

    /* Jobb kozep / szikrazo oldal */
    add_vwall(scene, 10.0f, 4.0f, 6.0f);
    add_hwall(scene, 10.0f, 14.0f, 4.0f);
    add_vwall(scene, 10.0f, 6.0f, 7.0f);
    add_hwall(scene, 8.0f, 10.0f, 7.0f);
    add_vwall(scene, 8.0f, 7.0f, 9.0f);
    add_hwall(scene, 5.0f, 8.0f, 9.0f);

    /* x = 14 ajto: kozep z = -0.5, nyilas z = -1.25 .. 0.25 */
    add_vwall(scene, 14.0f, 0.25f, 4.0f);
    add_vwall(scene, 14.0f, -5.0f, -1.25f);

    add_hwall(scene, 11.0f, 14.0f, -5.0f);

    /* Jobb also / jobb oldal */
    add_vwall(scene, 7.0f, -20.0f, -10.0f);
    add_hwall(scene, 7.0f, 14.0f, -10.0f);
    add_hwall(scene, 17.0f, 20.0f, -10.0f);
    add_vwall(scene, 14.0f, -18.5f, -10.0f);
    add_hwall(scene, 18.0f, 20.0f, -8.0f);

    /* x = 5 ajto felso oldala */
    add_vwall(scene, 5.0f, 10.75f, 17.0f);

    /* Eddig tartanak a falak */
    scene->wall_count = scene->collider_count;
   
    /* Kijarati ajto collider */
    scene->exit_door_collider_index = scene->collider_count;
    add_collider(scene, -1.0f, 19.7f, 3.0f, 0.4f);

    /* Függőleges falnyílások */
    add_inner_door(scene, -5.0f,   10.0f, 90.0f, 1.0f,  -1.0f);
    add_inner_door(scene, -16.0f, -12.85f, 90.0f, 1.0f,  -1.0f);
    add_inner_door(scene, -10.0f,   1.5f, 90.0f, 1.0f,  -1.0f);
    add_inner_door(scene,  5.0f,   10.0f, 90.0f,  -1.0f, 1.0f);
    add_inner_door(scene, 14.0f,   -0.5f, 90.0f,  -1.0f, 1.0f);
    //add_inner_door(scene, -15.35f, -6.0f, 0.0f, 1.0f,  -1.0f);

    /* Vízszintes falnyílás */
    add_inner_door(scene, -15.5f,  -6.0f, 0.0f, 1.0f,  -1.0f);

    /* Generatorok */
    add_generator(scene, -15.0f, -11.0f);
    add_generator(scene,  15.5f,  -3.5f);
    add_generator(scene, -10.0f,  17.0f);

    /* Generator colliderjei */
    add_collider(scene, -15.0f, -11.0f, 1.0f, 1.0f);
    add_collider(scene,  15.5f,  -3.5f, 1.0f, 1.0f);
    add_collider(scene, -10.0f,  17.0f, 1.0f, 1.0f);

   /* =========================
   CRATE LEOSZTAS - UJ VERZIO
   scale mindig 0.01f
   csak az also crate kap collidert
   ========================= */

/* =========================
   1. Bal also raktar
   ========================= */

/* dupla rakas */
add_crate(scene, -17.5f, 0.7f, -15.5f, 0.0f, 0.01f);
add_collider(scene, -17.5f, -15.5f, 1.0f, 1.0f);
add_crate(scene, -17.5f, 1.5f, -15.5f, 0.0f, 0.01f);

/* szimpla */
add_crate(scene, -15.8f, 0.7f, -15.2f, 0.0f, 0.01f);
add_collider(scene, -15.8f, -15.2f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, -18.2f, 0.7f, -12.8f, 90.0f, 0.01f);
add_collider(scene, -18.2f, -12.8f, 1.0f, 1.0f);
add_crate(scene, -18.2f, 1.5f, -12.8f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, -14.8f, 0.7f, -11.8f, 90.0f, 0.01f);
add_collider(scene, -14.8f, -11.8f, 1.0f, 1.0f);

/* szimpla */
add_crate(scene, -12.8f, 0.7f, -8.2f, 0.0f, 0.01f);
add_collider(scene, -12.8f, -8.2f, 1.0f, 1.0f);


/* =========================
   2. Bal kozep / szikrazo resz
   ========================= */

/* szimpla */
add_crate(scene, -17.2f, 0.7f, 4.8f, 0.0f, 0.01f);
add_collider(scene, -17.2f, 4.8f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, -15.0f, 0.7f, 5.5f, 90.0f, 0.01f);
add_collider(scene, -15.0f, 5.5f, 1.0f, 1.0f);
add_crate(scene, -15.0f, 1.5f, 5.5f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, -13.2f, 0.7f, 7.5f, 0.0f, 0.01f);
add_collider(scene, -13.2f, 7.5f, 1.0f, 1.0f);


/* =========================
   3. Bal felso / labor kornyeke
   ========================= */

/* szimpla */
add_crate(scene, -17.5f, 0.7f, 14.8f, 0.0f, 0.01f);
add_collider(scene, -17.5f, 14.8f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, -15.2f, 0.7f, 16.2f, 90.0f, 0.01f);
add_collider(scene, -15.2f, 16.2f, 1.0f, 1.0f);
add_crate(scene, -15.2f, 1.5f, 16.2f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, -9.5f, 0.7f, 15.0f, 0.0f, 0.01f);
add_collider(scene, -9.5f, 15.0f, 1.0f, 1.0f);

/* szimpla */
add_crate(scene, -4.0f, 0.7f, 16.5f, 90.0f, 0.01f);
add_collider(scene, -4.0f, 16.5f, 1.0f, 1.0f);


/* =========================
   4. Kozepso nagy terem
   ========================= */

/* szimpla */
add_crate(scene, -1.5f, 0.7f, -3.5f, 0.0f, 0.01f);
add_collider(scene, -1.5f, -3.5f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, 1.5f, 0.7f, -2.5f, 90.0f, 0.01f);
add_collider(scene, 1.5f, -2.5f, 1.0f, 1.0f);
add_crate(scene, 1.5f, 1.5f, -2.5f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, 3.8f, 0.7f, 6.5f, 0.0f, 0.01f);
add_collider(scene, 3.8f, 6.5f, 1.0f, 1.0f);


/* =========================
   5. Jobb also raktar
   ========================= */

/* szimpla */
add_crate(scene, 9.5f, 0.7f, -15.2f, 0.0f, 0.01f);
add_collider(scene, 9.5f, -15.2f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, 12.0f, 0.7f, -14.0f, 90.0f, 0.01f);
add_collider(scene, 12.0f, -14.0f, 1.0f, 1.0f);
add_crate(scene, 12.0f, 1.5f, -14.0f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, 15.5f, 0.7f, -11.5f, 0.0f, 0.01f);
add_collider(scene, 15.5f, -11.5f, 1.0f, 1.0f);


/* =========================
   6. Jobb kozep / gepterem
   ========================= */

/* szimpla */
add_crate(scene, 12.2f, 0.7f, 2.5f, 0.0f, 0.01f);
add_collider(scene, 12.2f, 2.5f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, 15.0f, 0.7f, 4.0f, 90.0f, 0.01f);
add_collider(scene, 15.0f, 4.0f, 1.0f, 1.0f);
add_crate(scene, 15.0f, 1.5f, 4.0f, 90.0f, 0.01f);

/* szimpla */
add_crate(scene, 13.5f, 0.7f, 7.5f, 0.0f, 0.01f);
add_collider(scene, 13.5f, 7.5f, 1.0f, 1.0f);


/* =========================
   7. Jobb felso
   ========================= */

/* szimpla */
add_crate(scene, 15.5f, 0.7f, 13.0f, 0.0f, 0.01f);
add_collider(scene, 15.5f, 13.0f, 1.0f, 1.0f);

/* dupla rakas */
add_crate(scene, 13.0f, 0.7f, 16.0f, 90.0f, 0.01f);
add_collider(scene, 13.0f, 16.0f, 1.0f, 1.0f);
add_crate(scene, 13.0f, 1.5f, 16.0f, 90.0f, 0.01f);
}

void update_scene(Scene* scene, double delta_time, float player_x, float player_z)
{
    int i;
    if (scene->exit_door_open) {
    float max_offset = 2.0f;
    float open_speed = 1.0f;

    if (scene->exit_door_offset < max_offset) {
        scene->exit_door_offset += (float)(delta_time * open_speed);
    }

    if (scene->exit_door_offset > max_offset) {
        scene->exit_door_offset = max_offset;
    }

    if (scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = false;
    }
}

    update_particles(scene, delta_time);

    if (scene->game_over || scene->game_won) {
        return;
    }

    for (i = 0; i < scene->drone_count; ++i) {
    update_drone(scene, &scene->drones[i], i, delta_time);

    if (is_player_detected_by_drone(&scene->drones[i], player_x, player_z)) {
        scene->game_over = true;
        printf("GAME OVER: Drone %d detected the player!\n", i + 1);
        return;
    }
    }   

    if (is_player_at_exit(scene, player_x, player_z)) {
        scene->game_won = true;
        printf("MISSION COMPLETE: Player escaped!\n");
        return;
    }

   for (i = 0; i < scene->inner_door_count; ++i) {
    InnerDoor* door = &scene->inner_doors[i];

    if (door->open) {
        if (door->collider_index >= 0) {
            scene->colliders[door->collider_index].active = false;
        }

        if (door->open_offset < 90.0f) {
            door->open_offset += (float)(delta_time * 120.0f);
        }

        if (door->open_offset > 90.0f) {
            door->open_offset = 90.0f;
        }

        door->auto_close_timer += (float)delta_time;

        if (door->auto_close_timer >= 5.0f) {
            door->open = false;
            door->auto_close_timer = 0.0f;
        }
    }
    else {
        if (door->open_offset > 0.0f) {
            door->open_offset -= (float)(delta_time * 120.0f);
        }

        if (door->open_offset <= 0.0f) {
            door->open_offset = 0.0f;

            if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = true;
            }
        }
        else {
            if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = false;
            }
        }
    }
    }
}

void render_scene(const Scene* scene)
{
    int i;

    //draw_ceiling(scene->floor_size, scene->wall_texture);
    draw_floor(scene->floor_size, scene->floor_texture);
    draw_ceiling(scene->floor_size, scene->ceiling_texture);

    /* 0-3: kulso falak, 4-6: belso falak */
    for (i = 0; i < scene->wall_count; ++i) {
    draw_wall(
        scene->colliders[i].x,
        scene->colliders[i].z,
        scene->colliders[i].width,
        scene->colliders[i].depth,
        scene->wall_texture
    );
    }

    draw_exit_door(scene);

    for (i = 0; i < scene->inner_door_count; ++i) {
    draw_inner_door(scene, &scene->inner_doors[i]);
    }

    for (i = 0; i < scene->generator_count; ++i) {
    draw_generator(scene, &scene->generators[i]);
    }

    for (i = 0; i < scene->drone_count; ++i) {
    draw_detection_sector(
        scene->drones[i].x,
        scene->drones[i].z,
        scene->drones[i].detection_radius,
        scene->drones[i].direction_angle,
        scene->drones[i].detection_angle
    );
    draw_drone_path(i);

    draw_drone(&scene->drones[i]);
    }
    draw_particles(scene);

   for (i = 0; i < scene->crate_count; ++i) {
    draw_static_model(
        &scene->crate_model,
        scene->crate_texture,
        scene->crates[i].x,
        scene->crates[i].y,
        scene->crates[i].z,
        scene->crates[i].rotation_y,
        scene->crates[i].scale
    );
    }
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

    if (scene->ceiling_texture != 0) {
    glDeleteTextures(1, &scene->ceiling_texture);
    scene->ceiling_texture = 0;
    }

    destroy_model(&scene->door_frame_model);
    destroy_model(&scene->door_panel_model);
    if (scene->generator_texture != 0) {
    glDeleteTextures(1, &scene->generator_texture);
    scene->generator_texture = 0;
    }

    destroy_model(&scene->generator_model);

    if (scene->inner_door_texture != 0) {
    glDeleteTextures(1, &scene->inner_door_texture);
    scene->inner_door_texture = 0;
    }

    if (scene->crate_texture != 0) {
    glDeleteTextures(1, &scene->crate_texture);
    scene->crate_texture = 0;
}

    destroy_model(&scene->crate_model);

    if (scene->console_texture != 0) {
    glDeleteTextures(1, &scene->console_texture);
    scene->console_texture = 0;
    }

    destroy_model(&scene->console_model);

    if (scene->desk_chair_texture != 0) {
    glDeleteTextures(1, &scene->desk_chair_texture);
    scene->desk_chair_texture = 0;
    }

    destroy_model(&scene->desk_chair_model);

    if (scene->plant_capsule_texture != 0) {
    glDeleteTextures(1, &scene->plant_capsule_texture);
    scene->plant_capsule_texture = 0;
    }

    destroy_model(&scene->plant_capsule_model);

    destroy_model(&scene->inner_door_model);
}