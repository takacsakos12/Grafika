#ifndef SCENE_PARTICLES_H
#define SCENE_PARTICLES_H

#include "scene.h"

void init_particles(Scene* scene);
void init_spark_sources(Scene* scene);
void update_particles(Scene* scene, double delta_time);
void draw_particles(const Scene* scene);

#endif
