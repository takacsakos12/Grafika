#include "scene_environment.h"

#include <GL/gl.h>
#include <stdio.h>

void set_scene_lighting(const Scene* scene)
{
    float b = scene->brightness;

    GLfloat ambient_light[] = {0.05f * b, 0.05f * b, 0.07f * b, 1.0f};
    GLfloat diffuse_light[] = {1.00f * b, 1.00f * b, 1.00f * b, 1.0f};
    GLfloat specular_light[] = {0.20f * b, 0.20f * b, 0.20f * b, 1.0f};
    GLfloat light_position[] = {0.0f, 8.0f, 0.0f, 1.0f};

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
