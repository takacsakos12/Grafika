#ifndef SCENE_INTERACTION_H
#define SCENE_INTERACTION_H

#include "scene.h"
#include <stdbool.h>

void interact_scene(Scene* scene, float player_x, float player_z);
void reset_scene(Scene* scene);
bool is_player_at_exit(const Scene* scene, float player_x, float player_z);

#endif
