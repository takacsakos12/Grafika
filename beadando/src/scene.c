#include "scene.h"
#include "texture.h"
#include "model.h"

#include <GL/gl.h>
#include <stdio.h>
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
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-size, 0.0f, -size);

    glTexCoord2f(8.0f, 0.0f);
    glVertex3f(size, 0.0f, -size);

    glTexCoord2f(8.0f, 8.0f);
    glVertex3f(size, 0.0f, size);

    glTexCoord2f(0.0f, 8.0f);
    glVertex3f(-size, 0.0f, size);

    glEnd();

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

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

    glColor3f(1.0, 1.0f, 1.0f);
    draw_box(x, z, width, 3.0f, depth);

    glDisable(GL_TEXTURE_2D);
}

static void draw_exit_door(const Scene* scene)
{
    if (scene->exit_door_open) {
        return;
    }

    glDisable(GL_LIGHTING);
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

/* ---------- Scene lifecycle ---------- */

void init_scene(Scene* scene)
{
    scene->floor_size = 20.0f;

    scene->collider_count = 0;

    scene->exit_door_open = false;
    scene->exit_door_collider_index = -1;

    scene->generator_count = 0;
    scene->active_generator_count = 0;

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

void update_scene(Scene* scene, double delta_time)
{
    (void)delta_time;

    if (scene->exit_door_open && scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = false;
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