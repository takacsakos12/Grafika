#ifndef CAMERA_H
#define CAMERA_H

#include "utils.h"
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct Scene Scene;

typedef struct Camera
{
    vec3 position;
    vec3 rotation;
    vec3 speed;
    bool is_preview_visible;
} Camera;

/* Kamera alaphelyzetbe allitasa. */
void init_camera(Camera* camera);

/* Kamera mozgatasa a bemenetek es az utkozesvizsgalat alapjan. */
void update_camera(
    Camera* camera,
    const Scene* scene,
    double delta_time,
    bool move_forward,
    bool move_backward,
    bool move_left,
    bool move_right
);

/* Kamera nezeti matrixanak beallitasa OpenGL-ben. */
void set_view(const Camera* camera);

/* Kamera forgatasa eger mozgas alapjan. */
void rotate_camera(Camera* camera, double horizontal, double vertical);

/* Kamera visszaallitasa a kezdo pozicioba. */
void reset_camera(Camera* camera);

#endif /* CAMERA_H */