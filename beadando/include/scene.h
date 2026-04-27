#ifndef SCENE_H
#define SCENE_H

typedef struct Scene
{
    float floor_size;
} Scene;

void init_scene(Scene* scene);
void update_scene(Scene* scene, double delta_time);
void render_scene(const Scene* scene);
void destroy_scene(Scene* scene);

#endif /* SCENE_H */