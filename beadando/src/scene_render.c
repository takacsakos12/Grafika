#include "scene_render.h"
#include "model.h"

#include <GL/gl.h>

void draw_box(float x, float z, float width, float height, float depth)
{
    float x1 = x - width / 2.0f;
    float x2 = x + width / 2.0f;
    float y1 = 0.0f;
    float y2 = height;
    float z1 = z - depth / 2.0f;
    float z2 = z + depth / 2.0f;

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z2);
    glTexCoord2f(width, 0.0f); glVertex3f(x2, y1, z2);
    glTexCoord2f(width, height); glVertex3f(x2, y2, z2);
    glTexCoord2f(0.0f, height); glVertex3f(x1, y2, z2);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x2, y1, z1);
    glTexCoord2f(width, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(width, height); glVertex3f(x1, y2, z1);
    glTexCoord2f(0.0f, height); glVertex3f(x2, y2, z1);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(depth, 0.0f); glVertex3f(x1, y1, z2);
    glTexCoord2f(depth, height); glVertex3f(x1, y2, z2);
    glTexCoord2f(0.0f, height); glVertex3f(x1, y2, z1);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x2, y1, z2);
    glTexCoord2f(depth, 0.0f); glVertex3f(x2, y1, z1);
    glTexCoord2f(depth, height); glVertex3f(x2, y2, z1);
    glTexCoord2f(0.0f, height); glVertex3f(x2, y2, z2);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y2, z2);
    glTexCoord2f(width, 0.0f); glVertex3f(x2, y2, z2);
    glTexCoord2f(width, depth); glVertex3f(x2, y2, z1);
    glTexCoord2f(0.0f, depth); glVertex3f(x1, y2, z1);

    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1, y1, z1);
    glTexCoord2f(width, 0.0f); glVertex3f(x2, y1, z1);
    glTexCoord2f(width, depth); glVertex3f(x2, y1, z2);
    glTexCoord2f(0.0f, depth); glVertex3f(x1, y1, z2);

    glEnd();
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

static void draw_floor(float size, GLuint texture)
{
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0f, -size);
    glTexCoord2f(8.0f, 0.0f); glVertex3f(size, 0.0f, -size);
    glTexCoord2f(8.0f, 8.0f); glVertex3f(size, 0.0f, size);
    glTexCoord2f(0.0f, 8.0f); glVertex3f(-size, 0.0f, size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_wall(float x, float z, float width, float depth, GLuint texture)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
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
    glColor3f(0.75f, 0.75f, 0.75f);

    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, y, size);
    glTexCoord2f(8.0f, 0.0f); glVertex3f(size, y, size);
    glTexCoord2f(8.0f, 8.0f); glVertex3f(size, y, -size);
    glTexCoord2f(0.0f, 8.0f); glVertex3f(-size, y, -size);
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
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(door_x, door_y, door_z);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(door_scale, door_scale, door_scale);
    render_model(&scene->door_frame_model);
    glPopMatrix();

    if (scene->exit_door_offset < max_visible_offset) {
        glPushMatrix();
        glTranslatef(door_x + scene->exit_door_offset, door_y, door_z);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glScalef(door_scale, door_scale, door_scale);
        render_model(&scene->door_panel_model);
        glPopMatrix();
    }

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

    if (generator->active) {
        glColor3f(0.5f, 1.0f, 1.0f);
    }
    else {
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    glPushMatrix();
    glTranslatef(generator->x, generator_y, generator->z);
    glScalef(generator_scale, generator_scale, generator_scale);
    render_model(&scene->generator_model);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

static void draw_inner_door(const Scene* scene, const InnerDoor* door)
{
    float door_y = 0.0f;
    float door_scale = 1.5f;
    float hinge_offset = 0.74f;
    float model_center_fix_z = -0.10f;
    float base_rotation = door->rotation_y + 90.0f;
    float angle = door->open_offset * door->open_direction;

    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, scene->inner_door_texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(door->x, door_y, door->z);
    glRotatef(base_rotation, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, door->hinge_side * hinge_offset);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -door->hinge_side * hinge_offset);
    glTranslatef(0.0f, 0.0f, model_center_fix_z);
    glScalef(door_scale, door_scale, door_scale);
    render_model(&scene->inner_door_model);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
}

void render_scene_geometry(const Scene* scene)
{
    int i;

    draw_floor(scene->floor_size, scene->floor_texture);
    draw_ceiling(scene->floor_size, scene->ceiling_texture);

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
}

void render_scene_interactables(const Scene* scene)
{
    int i;

    for (i = 0; i < scene->inner_door_count; ++i) {
        draw_inner_door(scene, &scene->inner_doors[i]);
    }

    for (i = 0; i < scene->generator_count; ++i) {
        draw_generator(scene, &scene->generators[i]);
    }
}

void render_scene_static_objects(const Scene* scene)
{
    int i;

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

    for (i = 0; i < scene->plant_capsule_count; ++i) {
        draw_static_model(
            &scene->plant_capsule_model,
            scene->plant_capsule_texture,
            scene->plant_capsules[i].x,
            scene->plant_capsules[i].y,
            scene->plant_capsules[i].z,
            scene->plant_capsules[i].rotation_y,
            scene->plant_capsules[i].scale
        );
    }
}
