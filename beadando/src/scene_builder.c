#include "scene_builder.h"

#include <math.h>

void add_collider(Scene* scene, float x, float z, float width, float depth)
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

void add_hwall(Scene* scene, float x1, float x2, float z)
{
    float center_x = (x1 + x2) / 2.0f;
    float width = fabsf(x2 - x1);

    add_collider(scene, center_x, z, width, 0.4f);
}

void add_vwall(Scene* scene, float x, float z1, float z2)
{
    float center_z = (z1 + z2) / 2.0f;
    float depth = fabsf(z2 - z1);

    add_collider(scene, x, center_z, 0.4f, depth);
}

void add_crate(
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

void add_plant_capsule(
    Scene* scene,
    float x,
    float y,
    float z,
    float rotation_y,
    float scale
)
{
    if (scene->plant_capsule_count >= MAX_PLANT_CAPSULES) {
        return;
    }

    scene->plant_capsules[scene->plant_capsule_count].x = x;
    scene->plant_capsules[scene->plant_capsule_count].y = y;
    scene->plant_capsules[scene->plant_capsule_count].z = z;
    scene->plant_capsules[scene->plant_capsule_count].rotation_y = rotation_y;
    scene->plant_capsules[scene->plant_capsule_count].scale = scale;
    scene->plant_capsules[scene->plant_capsule_count].collider_index = -1;

    scene->plant_capsule_count++;
}

void add_generator(Scene* scene, float x, float z)
{
    if (scene->generator_count >= MAX_GENERATORS) {
        return;
    }

    scene->generators[scene->generator_count].x = x;
    scene->generators[scene->generator_count].z = z;
    scene->generators[scene->generator_count].active = false;

    scene->generator_count++;
}

void add_inner_door(
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
