#include "scene.h"

#include <GL/gl.h>

void init_scene(Scene* scene)
{
}

void update_scene(Scene* scene)
{
}

void render_scene(const Scene* scene)
{
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
glVertex3f(0, 0.001, 0);
glColor3f(0.0, 1.0, 1.0);
glVertex3f(1, 0.001, 0);
glColor3f(1.0, 0.0, 1.0);
glVertex3f(0.5, 0.001, 0.8);

glEnd();


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
