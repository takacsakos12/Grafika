#ifndef SCENE_BUILDER_H
#define SCENE_BUILDER_H

#include "scene.h"

void add_collider(Scene* scene, float x, float z, float width, float depth);

void add_hwall(Scene* scene, float x1, float x2, float z);
void add_vwall(Scene* scene, float x, float z1, float z2);

void add_crate(
    Scene* scene,
    float x,
    float y,
    float z,
    float rotation_y,
    float scale
);

void add_plant_capsule(
    Scene* scene,
    float x,
    float y,
    float z,
    float rotation_y,
    float scale
);

void add_generator(Scene* scene, float x, float z);

void add_inner_door(
    Scene* scene,
    float x,
    float z,
    float rotation_y,
    float hinge_side,
    float open_direction
);

#endif
