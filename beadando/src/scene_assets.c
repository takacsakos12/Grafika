#include "scene_assets.h"
#include "texture.h"
#include "model.h"

#include <GL/gl.h>
#include <stdio.h>

void load_scene_assets(Scene* scene)
{
    scene->floor_texture = load_texture("assets/textures/floor.bmp");
    scene->wall_texture = load_texture("assets/textures/wall.bmp");
    scene->door_texture = load_texture("assets/textures/door.bmp");
    scene->ceiling_texture = load_texture("assets/textures/roof.bmp");

    if (!load_model_flip_v(&scene->door_frame_model, "assets/models/door_frame.obj")) {
        printf("Failed to load door_frame.obj\n");
    }

    if (!load_model_flip_v(&scene->door_panel_model, "assets/models/door_panel.obj")) {
        printf("Failed to load door_panel.obj\n");
    }

    scene->generator_texture = load_texture("assets/textures/generator.bmp");

    if (!load_model_flip_v(&scene->generator_model, "assets/models/generator.obj")) {
        printf("Failed to load generator.obj\n");
    }

    scene->inner_door_texture = load_texture("assets/textures/inner_door.bmp");

    if (!load_model(&scene->inner_door_model, "assets/models/inner_door.obj")) {
        printf("Failed to load inner_door.obj\n");
    }

    scene->crate_texture = load_texture("assets/textures/crate.bmp");

    if (!load_model(&scene->crate_model, "assets/models/crate.obj")) {
        printf("Failed to load crate.obj\n");
    }

    scene->console_texture = load_texture("assets/textures/console.bmp");

    if (!load_model(&scene->console_model, "assets/models/console.obj")) {
        printf("Failed to load console.obj\n");
    }

    scene->desk_chair_texture = load_texture("assets/textures/desk_chair.bmp");

    if (!load_model(&scene->desk_chair_model, "assets/models/desk_chair.obj")) {
        printf("Failed to load desk_chair.obj\n");
    }

    scene->plant_capsule_texture = load_texture("assets/textures/plant_capsule.bmp");

    if (!load_model(&scene->plant_capsule_model, "assets/models/plant_capsule.obj")) {
        printf("Failed to load plant_capsule.obj\n");
    }
}

static void delete_texture(GLuint* texture)
{
    if (*texture != 0) {
        glDeleteTextures(1, texture);
        *texture = 0;
    }
}

void destroy_scene(Scene* scene)
{
    delete_texture(&scene->floor_texture);
    delete_texture(&scene->wall_texture);
    delete_texture(&scene->door_texture);
    delete_texture(&scene->ceiling_texture);
    delete_texture(&scene->generator_texture);
    delete_texture(&scene->inner_door_texture);
    delete_texture(&scene->crate_texture);
    delete_texture(&scene->console_texture);
    delete_texture(&scene->desk_chair_texture);
    delete_texture(&scene->plant_capsule_texture);

    destroy_model(&scene->door_frame_model);
    destroy_model(&scene->door_panel_model);
    destroy_model(&scene->generator_model);
    destroy_model(&scene->inner_door_model);
    destroy_model(&scene->crate_model);
    destroy_model(&scene->console_model);
    destroy_model(&scene->desk_chair_model);
    destroy_model(&scene->plant_capsule_model);
}
