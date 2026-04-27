#include "scene.h"

#include <GL/gl.h>

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

static void draw_wall(float x, float z, float width, float height, float depth)
{
    float x1 = x - width / 2.0f;
    float x2 = x + width / 2.0f;
    float y1 = 0.0f;
    float y2 = height;
    float z1 = z - depth / 2.0f;
    float z2 = z + depth / 2.0f;

    glBegin(GL_QUADS);

    glColor3f(0.45f, 0.45f, 0.50f);

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

static void draw_generator(float x, float z)
{
    glPushMatrix();

    glTranslatef(x, 0.5f, z);
    glScalef(0.7f, 1.0f, 0.7f);

    glBegin(GL_QUADS);

    glColor3f(0.1f, 0.6f, 0.2f);

    /* front */
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    /* back */
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    /* left */
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);

    /* right */
    glVertex3f(0.5f, -0.5f,  0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f,  0.5f, -0.5f);
    glVertex3f(0.5f,  0.5f,  0.5f);

    /* top */
    glVertex3f(-0.5f, 0.5f,  0.5f);
    glVertex3f( 0.5f, 0.5f,  0.5f);
    glVertex3f( 0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    /* bottom */
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);

    glEnd();

    glPopMatrix();
}

void init_scene(Scene* scene)
{
    scene->floor_size = 20.0f;
}

void update_scene(Scene* scene, double delta_time)
{
    (void)scene;
    (void)delta_time;
}

void render_scene(const Scene* scene)
{
    draw_floor(scene->floor_size);

    /* kulso falak */
    draw_wall(0.0f, -10.0f, 20.0f, 3.0f, 0.4f);
    draw_wall(0.0f,  10.0f, 20.0f, 3.0f, 0.4f);
    draw_wall(-10.0f, 0.0f, 0.4f, 3.0f, 20.0f);
    draw_wall( 10.0f, 0.0f, 0.4f, 3.0f, 20.0f);

    /* par belso fal */
    draw_wall(-3.0f, 0.0f, 0.4f, 3.0f, 8.0f);
    draw_wall(4.0f, -2.0f, 0.4f, 3.0f, 6.0f);
    draw_wall(1.0f, 4.0f, 6.0f, 3.0f, 0.4f);

    /* harom egyszeru generator */
    draw_generator(-6.0f, -6.0f);
    draw_generator(6.0f, -6.0f);
    draw_generator(6.0f, 6.0f);
}

void destroy_scene(Scene* scene)
{
    (void)scene;
}