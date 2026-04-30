#include "scene.h"

#include <GL/gl.h>

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

static void draw_floor(float size)
{
    glBegin(GL_QUADS);

    glColor3f(0.25f, 0.25f, 0.25f);

    glVertex3f(-size, 0.0f, -size);
    glVertex3f( size, 0.0f, -size);
    glVertex3f( size, 0.0f,  size);
    glVertex3f(-size, 0.0f,  size);

    glEnd();
}

static void draw_box(float x, float z, float width, float height, float depth)
{
    float x1 = x - width / 2.0f;
    float x2 = x + width / 2.0f;
    float y1 = 0.0f;
    float y2 = height;
    float z1 = z - depth / 2.0f;
    float z2 = z + depth / 2.0f;

    glBegin(GL_QUADS);

    /* front */
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);

    /* back */
    glVertex3f(x2, y1, z1);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);

    /* left */
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y1, z2);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y2, z1);

    /* right */
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);

    /* top */
    glVertex3f(x1, y2, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    /* bottom */
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    glEnd();
}

static void draw_wall(float x, float z, float width, float depth)
{
    glColor3f(0.45f, 0.45f, 0.50f);
    draw_box(x, z, width, 3.0f, depth);
}

static void draw_exit_door(const Scene* scene)
{
    if (scene->exit_door_open) {
        return;
    }

    glColor3f(0.45f, 0.20f, 0.10f);
    draw_box(0.0f, 9.7f, 3.0f, 2.5f, 0.4f);
}

static void draw_generator(float x, float z)
{
    glColor3f(0.1f, 0.6f, 0.2f);
    draw_box(x, z, 1.0f, 1.0f, 1.0f);
}

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

static bool circle_intersects_collider(float circle_x, float circle_z, float radius, const Collider* collider)
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

void init_scene(Scene* scene)
{
    scene->floor_size = 20.0f;
    scene->collider_count = 0;

    scene->exit_door_open = false;
    scene->exit_door_collider_index = -1;

    /* Kulso falak */
    add_collider(scene, 0.0f, -20.0f, 40.0f, 0.4f);
    add_collider(scene, 0.0f,  20.0f, 40.0f, 0.4f);
    add_collider(scene, -20.0f, 0.0f, 0.4f, 40.0f);
    add_collider(scene,  20.0f, 0.0f, 0.4f, 40.0f);
    /* Belso falak */
    add_collider(scene, -3.0f, 0.0f, 0.4f, 8.0f);
    add_collider(scene,  4.0f, -2.0f, 0.4f, 6.0f);
    add_collider(scene,  1.0f, 4.0f, 6.0f, 0.4f);

    /* Kijarati ajto collider */
    scene->exit_door_collider_index = scene->collider_count;
    add_collider(scene, 0.0f, 9.7f, 3.0f, 0.4f);

    /* Generatorok colliderjei */
    add_collider(scene, -6.0f, -6.0f, 1.0f, 1.0f);
    add_collider(scene,  6.0f, -6.0f, 1.0f, 1.0f);
    add_collider(scene,  6.0f,  6.0f, 1.0f, 1.0f);
}

void update_scene(Scene* scene, double delta_time)
{
    (void)delta_time;

    /*
        Ez csak teszt.
        Kesobb akkor legyen true, ha mindharom generator aktiv.
    */

    if (scene->exit_door_open && scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = false;
    }
}

void render_scene(const Scene* scene)
{
    int i;

    draw_floor(scene->floor_size);

    /* Falak kirajzolasa: az elso 7 collider most fal */
    for (i = 0; i < 7; ++i) {
        draw_wall(
            scene->colliders[i].x,
            scene->colliders[i].z,
            scene->colliders[i].width,
            scene->colliders[i].depth
        );
    }

    draw_exit_door(scene);

    draw_generator(-6.0f, -6.0f);
    draw_generator(6.0f, -6.0f);
    draw_generator(6.0f, 6.0f);
}

void destroy_scene(Scene* scene)
{
    (void)scene;
}