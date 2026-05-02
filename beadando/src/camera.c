#include "camera.h"
#include "scene.h"

#include <GL/gl.h>

#include <math.h>

#define PLAYER_HEIGHT 1.7
#define PLAYER_RADIUS 0.35
#define MOVE_SPEED 4.0
#define MOUSE_SENSITIVITY 0.15

void init_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = PLAYER_HEIGHT;
    camera->position.z = 5.0;

    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 180.0;

    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;

    camera->is_preview_visible = false;
}

void update_camera(
    Camera* camera,
    const Scene* scene,
    double delta_time,
    bool move_forward,
    bool move_backward,
    bool move_left,
    bool move_right
)
{
    double yaw;
    double forward_x;
    double forward_z;
    double right_x;
    double right_z;
    double move_x;
    double move_z;
    double length;

    float next_x;
    float next_z;

    yaw = degree_to_radian(-camera->rotation.z);

    forward_x = sin(yaw);
    forward_z = -cos(yaw);

    right_x = cos(yaw);
    right_z = sin(yaw);

    move_x = 0.0;
    move_z = 0.0;

    if (move_forward) {
        move_x += forward_x;
        move_z += forward_z;
    }

    if (move_backward) {
        move_x -= forward_x;
        move_z -= forward_z;
    }

    if (move_left) {
        move_x -= right_x;
        move_z -= right_z;
    }

    if (move_right) {
        move_x += right_x;
        move_z += right_z;
    }

    length = sqrt(move_x * move_x + move_z * move_z);

    if (length > 0.0) {
        move_x /= length;
        move_z /= length;
    }

    next_x = (float)(camera->position.x + move_x * MOVE_SPEED * delta_time);
    next_z = (float)(camera->position.z + move_z * MOVE_SPEED * delta_time);

    /*
        X es Z irany kulon ellenorzese.
        Igy a jatekos tud csuszni a falak menten,
        nem akad meg teljesen a sarkoknal.
    */

    if (!check_collision(scene, next_x, (float)camera->position.z, PLAYER_RADIUS)) {
        camera->position.x = next_x;
    }

    if (!check_collision(scene, (float)camera->position.x, next_z, PLAYER_RADIUS)) {
        camera->position.z = next_z;
    }

    camera->position.y = PLAYER_HEIGHT;
}

void set_view(const Camera* camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(float)camera->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(-(float)camera->rotation.z, 0.0f, 1.0f, 0.0f);

    glTranslatef(
        -(float)camera->position.x,
        -(float)camera->position.y,
        -(float)camera->position.z
    );
}

void reset_camera(Camera* camera)
{
    camera->position.x = 0.0;
    camera->position.y = PLAYER_HEIGHT;
    camera->position.z = 0.0;

    camera->rotation.x = 0.0;
    camera->rotation.y = 0.0;
    camera->rotation.z = 0.0;

    camera->speed.x = 0.0;
    camera->speed.y = 0.0;
    camera->speed.z = 0.0;
}

void rotate_camera(Camera* camera, double horizontal, double vertical)
{
    camera->rotation.z -= horizontal * MOUSE_SENSITIVITY;
    camera->rotation.x -= vertical * MOUSE_SENSITIVITY;

    if (camera->rotation.x > 89.0) {
        camera->rotation.x = 89.0;
    }

    if (camera->rotation.x < -89.0) {
        camera->rotation.x = -89.0;
    }

    if (camera->rotation.z < 0.0) {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z >= 360.0) {
        camera->rotation.z -= 360.0;
    }
}