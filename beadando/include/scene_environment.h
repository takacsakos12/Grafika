#ifndef SCENE_ENVIRONMENT_H
#define SCENE_ENVIRONMENT_H

#include "scene.h"

void set_scene_lighting(const Scene* scene);
void change_scene_brightness(Scene* scene, float amount);
void apply_scene_fog(const Scene* scene);
void toggle_hard_mode(Scene* scene);

#endif
