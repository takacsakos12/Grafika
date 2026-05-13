#ifndef SCENE_DRONE_H
#define SCENE_DRONE_H

#include "scene.h"
#include <stdbool.h>

void init_drone(Drone* drone, int index);
void update_drone(Scene* scene, Drone* drone, int index, double delta_time);

bool is_player_detected_by_drone(
    const Scene* scene,
    const Drone* drone,
    float player_x,
    float player_z
);

void draw_drone_system(const Scene* scene);

#endif
