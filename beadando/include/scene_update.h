#ifndef SCENE_UPDATE_H
#define SCENE_UPDATE_H

#include "scene.h"

void update_exit_door(Scene* scene, double delta_time);
void update_inner_doors(Scene* scene, double delta_time);
void update_drones_and_detection(
    Scene* scene,
    double delta_time,
    float player_x,
    float player_z
);

#endif
