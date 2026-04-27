#include "scene.h"

#include <obj/load.h>
#include <obj/draw.h>

void init_scene(Scene* scene)
{
    // load_model(&(scene->cube), "assets/models/cube.obj");
    // scene->texture_id = load_texture("assets/textures/cube.png");
    load_model(&(scene->cube), "assets/models/cube.obj");
    scene->texture_id = load_texture("assets/textures/cube.png");
    
    load_model(&(scene->pyramid), "assets/models/pyramid_fixed.obj");
    
    glBindTexture(GL_TEXTURE_2D, scene->texture_id);

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 0.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
    scene-> rotation_angle = 0.0;
    scene->pyramid_x = 1.5f;
    scene->pyramid_y = 0.0f;
    scene->pyramid_z = 0.0f;
}

vvoid set_lighting(const Scene* scene)
{
    float ambient_light[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    float diffuse_light[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    float specular_light[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float position[]       = { 0.0f, 0.0f, 10.0f, 1.0f };

    switch (scene->light_mode) {
    case 1: // csak ambient
        ambient_light[0] = 1.0f;
        ambient_light[1] = 1.0f;
        ambient_light[2] = 1.0f;
        break;

    case 2: // csak diffuse
        diffuse_light[0] = 1.0f;
        diffuse_light[1] = 1.0f;
        diffuse_light[2] = 1.0f;
        break;

    case 3: // csak specular
        specular_light[0] = 1.0f;
        specular_light[1] = 1.0f;
        specular_light[2] = 1.0f;
        break;

    case 4: // ambient + diffuse
        ambient_light[0] = 0.3f;
        ambient_light[1] = 0.3f;
        ambient_light[2] = 0.3f;

        diffuse_light[0] = 1.0f;
        diffuse_light[1] = 1.0f;
        diffuse_light[2] = 1.0f;
        break;

    case 5: // diffuse + specular
        diffuse_light[0] = 1.0f;
        diffuse_light[1] = 1.0f;
        diffuse_light[2] = 1.0f;

        specular_light[0] = 1.0f;
        specular_light[1] = 1.0f;
        specular_light[2] = 1.0f;
        break;

    case 6: // mindharom
    default:
        ambient_light[0] = 0.2f;
        ambient_light[1] = 0.2f;
        ambient_light[2] = 0.2f;

        diffuse_light[0] = 1.0f;
        diffuse_light[1] = 1.0f;
        diffuse_light[2] = 1.0f;

        specular_light[0] = 1.0f;
        specular_light[1] = 1.0f;
        specular_light[2] = 1.0f;
        break;
    }

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene* scene)
{
    scene->rotation_angle += 1.0f;

    if (scene->rotation_angle > 360.0f) {
        scene->rotation_angle -= 360.0f;
    }
}

void render_scene(const Scene* scene)
{
    Material red_material = scene->material;
    Material green_material = scene->material;

    set_lighting(scene);
    draw_origin();

    red_material.diffuse.red = 1.0f;
    red_material.diffuse.green = 0.0f;
    red_material.diffuse.blue = 0.0f;

    set_material(&red_material);

    glPushMatrix();
    glTranslatef(-1.5f, 0.0f, 0.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_model(&(scene->cube));
    glPopMatrix();


    green_material.diffuse.red = 0.0f;
    green_material.diffuse.green = 1.0f;
    green_material.diffuse.blue = 0.0f;

    set_material(&green_material);

    glPushMatrix();
    glTranslatef(scene->pyramid_x, scene->pyramid_y, scene->pyramid_z);
    glRotatef(scene->rotation_angle, 0.0f, 0.0f, 1.0f);
    glScalef(0.3f, 0.3f, 0.3f);
    draw_model(&(scene->pyramid));
    glPopMatrix();
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
