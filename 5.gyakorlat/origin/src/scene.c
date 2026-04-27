#include "scene.h"

#include <GL/gl.h>
#include <math.h>

void init_scene(Scene* scene)
{
    scene->sphere_rotation_angle = 0.0f;
}

void update_scene(Scene* scene)
{
    scene->sphere_rotation_angle += 1.0f;

    if (scene->sphere_rotation_angle > 360.0f) {
        scene->sphere_rotation_angle -= 360.0f;
    }
}

void render_scene(const Scene* scene)
{
    int i, j;
    const float PI = 3.1415926;
    const int sectorCount = 4;
    const int stackCount = 4;
    const float radius = 0.25;

    draw_origin();

    glBegin(GL_QUADS);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 1);
    glVertex3f(0, 0, 1);

    glEnd();

    glBegin(GL_TRIANGLES);

    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.001, 0.0);

    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(1.0, 0.001, 0.0);

    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(0.5, 0.001, 0.8);

    glEnd();

    {
    int i;
    const int n = 20;
    const float radius = 0.15f;
    const float height = 0.4f;
    const float PI = 3.1415926f;

    glPushMatrix();
    glTranslatef(1.4f, 0.5f, 0.2f);

    glColor3f(0.7f, 0.7f, 0.7f);

    /* palast */
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i <= n; i++) {
        float angle = 2.0f * PI * i / n;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);

        glVertex3f(x, y, 0.0f);
        glVertex3f(x, y, height);
    }
    glEnd();

    /* also kor */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= n; i++) {
        float angle = 2.0f * PI * i / n;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    /* felso kor */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, height);
    for (i = 0; i <= n; i++) {
        float angle = 2.0f * PI * i / n;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x, y, height);
    }
    glEnd();

    {
    int i;
    const int n = 20;
    const float radius = 0.15f;
    const float height = 0.4f;
    const float PI = 3.1415926f;

    glPushMatrix();
    glTranslatef(1.8f, 0.5f, 0.0f);

    glColor3f(0.8f, 0.5f, 0.2f);

    /* oldal */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, height);
    for (i = 0; i <= n; i++) {
        float angle = 2.0f * PI * i / n;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    /* alja */
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= n; i++) {
        float angle = 2.0f * PI * i / n;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    glPopMatrix();
}

    glPopMatrix();
}

    glColor3f(0.6, 0.6, 0.6);

    glPushMatrix();
    glTranslatef(0.5, 0.5, 0.5);
    glRotatef(scene->sphere_rotation_angle, 0.0, 0.0, 1.0);

    for (i = 0; i < stackCount; ++i) {
        float stackAngle1 = PI / 2.0 - i * (PI / stackCount);
        float stackAngle2 = PI / 2.0 - (i + 1) * (PI / stackCount);

        float xy1 = radius * cosf(stackAngle1);
        float z1 = radius * sinf(stackAngle1);

        float xy2 = radius * cosf(stackAngle2);
        float z2 = radius * sinf(stackAngle2);

        glBegin(GL_TRIANGLE_STRIP);

        for (j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * (2.0 * PI / sectorCount);

            float x1 = xy1 * cosf(sectorAngle);
            float y1 = xy1 * sinf(sectorAngle);

            float x2 = xy2 * cosf(sectorAngle);
            float y2 = xy2 * sinf(sectorAngle);

            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
        }

        glEnd();
    }

    glPopMatrix();

    int row, col;
    float cell_size = 0.125f;

    for (row = 0; row < 8; row++) {
    for (col = 0; col < 8; col++) {
        float x = col * cell_size;
        float y = row * cell_size;

        if ((row + col) % 2 == 0) {
            glColor3f(0.9f, 0.9f, 0.9f);
        } else {
            glColor3f(0.2f, 0.2f, 0.2f);
        }

        glBegin(GL_QUADS);
        glVertex3f(x, y, 0.0f);
        glVertex3f(x + cell_size, y, 0.0f);
        glVertex3f(x + cell_size, y + cell_size, 0.0f);
        glVertex3f(x, y + cell_size, 0.0f);
        glEnd();
    }
}
}





void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();

    


}
