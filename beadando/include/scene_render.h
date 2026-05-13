#ifndef SCENE_RENDER_H
#define SCENE_RENDER_H

#include "scene.h"

void draw_box(float x, float z, float width, float height, float depth);

void render_scene_geometry(const Scene* scene);
void render_scene_interactables(const Scene* scene);
void render_scene_static_objects(const Scene* scene);

#endif
