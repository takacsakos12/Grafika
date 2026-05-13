#ifndef SCENE_COLLISION_H
#define SCENE_COLLISION_H

#include "scene.h"
#include <stdbool.h>

bool check_collision(const Scene* scene, float x, float z, float radius);

bool is_wall_between_drone_and_player(
    const Scene* scene,
    float drone_x,
    float drone_z,
    float player_x,
    float player_z
);

#endif
