#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"

#include <stdbool.h>

typedef struct Camera
{
    vec3 position;
    vec3 rotation;
    vec3 speed;
    bool is_preview_visible;
} Camera;

void init_camera(Camera* camera);

void update_camera(
    Camera* camera,
    double delta_time,
    bool move_forward,
    bool move_backward,
    bool move_left,
    bool move_right
);

void set_view(const Camera* camera);
void rotate_camera(Camera* camera, double horizontal, double vertical);

#endif /* CAMERA_H */