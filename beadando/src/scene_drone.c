#include "scene_drone.h"
#include "scene_collision.h"
#include "scene_render.h"

#define _USE_MATH_DEFINES
#include <GL/gl.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static const float drone_waypoints[MAX_DRONES][DRONE_WAYPOINT_COUNT][2] = {
    {
        {-3.0f, -7.0f},
        {-8.0f, -7.0f},
        {-8.0f, 7.5f}
    },
    {
        {11.5f, -7.5f},
        {16.0f, -7.5f},
        {16.0f, 8.5f}
    }
};

void init_drone(Drone* drone, int index)
{
    drone->x = drone_waypoints[index][0][0];
    drone->y = 1.4f;
    drone->z = drone_waypoints[index][0][1];

    drone->speed = 1.8f;
    drone->detection_radius = 5.0f;
    drone->detection_angle = (float)M_PI / 2.0f;
    drone->direction_angle = 0.0f;
    drone->current_waypoint = 1;
    drone->waypoint_direction = 1;
}

void update_drone(Scene* scene, Drone* drone, int index, double delta_time)
{
    float target_x = drone_waypoints[index][drone->current_waypoint][0];
    float target_z = drone_waypoints[index][drone->current_waypoint][1];
    float dx = target_x - drone->x;
    float dz = target_z - drone->z;
    float distance = sqrtf(dx * dx + dz * dz);
    float step;

    (void)scene;

    if (distance < 0.1f) {
        drone->current_waypoint += drone->waypoint_direction;

        if (drone->current_waypoint >= DRONE_WAYPOINT_COUNT) {
            drone->current_waypoint = DRONE_WAYPOINT_COUNT - 2;
            drone->waypoint_direction = -1;
        }

        if (drone->current_waypoint < 0) {
            drone->current_waypoint = 1;
            drone->waypoint_direction = 1;
        }

        return;
    }

    drone->direction_angle = atan2f(dz, dx);
    step = drone->speed * (float)delta_time;

    drone->x += dx / distance * step;
    drone->z += dz / distance * step;
}

bool is_player_detected_by_drone(
    const Scene* scene,
    const Drone* drone,
    float player_x,
    float player_z
)
{
    float dx = player_x - drone->x;
    float dz = player_z - drone->z;
    float distance_squared = dx * dx + dz * dz;
    float player_angle;
    float angle_difference;

    if (distance_squared > drone->detection_radius * drone->detection_radius) {
        return false;
    }

    player_angle = atan2f(dz, dx);
    angle_difference = player_angle - drone->direction_angle;

    while (angle_difference > (float)M_PI) {
        angle_difference -= 2.0f * (float)M_PI;
    }

    while (angle_difference < -(float)M_PI) {
        angle_difference += 2.0f * (float)M_PI;
    }

    if (fabsf(angle_difference) > drone->detection_angle / 2.0f) {
        return false;
    }

    if (is_wall_between_drone_and_player(scene, drone->x, drone->z, player_x, player_z)) {
        return false;
    }

    return true;
}

static void draw_detection_sector(float x, float z, float radius, float direction_angle, float sector_angle)
{
    int i;
    int segments = 32;
    float start_angle = direction_angle - sector_angle / 2.0f;
    float end_angle = direction_angle + sector_angle / 2.0f;
    float angle;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 0.0f, 0.0f, 0.22f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, 0.04f, z);

    for (i = 0; i <= segments; ++i) {
        angle = start_angle + (end_angle - start_angle) * ((float)i / (float)segments);
        glVertex3f(x + cosf(angle) * radius, 0.04f, z + sinf(angle) * radius);
    }

    glEnd();

    glDisable(GL_BLEND);

    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINE_STRIP);

    for (i = 0; i <= segments; ++i) {
        angle = start_angle + (end_angle - start_angle) * ((float)i / (float)segments);
        glVertex3f(x + cosf(angle) * radius, 0.05f, z + sinf(angle) * radius);
    }

    glEnd();

    glBegin(GL_LINES);
    glVertex3f(x, 0.05f, z);
    glVertex3f(x + cosf(start_angle) * radius, 0.05f, z + sinf(start_angle) * radius);
    glVertex3f(x, 0.05f, z);
    glVertex3f(x + cosf(end_angle) * radius, 0.05f, z + sinf(end_angle) * radius);
    glEnd();

    glEnable(GL_LIGHTING);
}

static void draw_drone_path(int index)
{
    int i;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 0.8f, 0.0f);

    glBegin(GL_LINES);

    for (i = 0; i < DRONE_WAYPOINT_COUNT - 1; ++i) {
        glVertex3f(drone_waypoints[index][i][0], 0.06f, drone_waypoints[index][i][1]);
        glVertex3f(drone_waypoints[index][i + 1][0], 0.06f, drone_waypoints[index][i + 1][1]);
    }

    glEnd();

    glEnable(GL_LIGHTING);
}

static void draw_drone(const Drone* drone)
{
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(drone->x, drone->y, drone->z);
    glRotatef(-drone->direction_angle * 180.0f / (float)M_PI, 0.0f, 1.0f, 0.0f);

    glColor3f(0.15f, 0.15f, 0.18f);
    draw_box(0.0f, 0.0f, 0.8f, 0.25f, 0.5f);

    glColor3f(1.0f, 0.0f, 0.0f);
    draw_box(0.45f, 0.0f, 0.12f, 0.12f, 0.12f);

    glColor3f(0.25f, 0.25f, 0.28f);
    draw_box(0.0f, -0.45f, 0.25f, 0.08f, 0.9f);
    draw_box(0.0f, 0.45f, 0.25f, 0.08f, 0.9f);

    glPopMatrix();
}

void draw_drone_system(const Scene* scene)
{
    int i;

    for (i = 0; i < scene->drone_count; ++i) {
        draw_detection_sector(
            scene->drones[i].x,
            scene->drones[i].z,
            scene->drones[i].detection_radius,
            scene->drones[i].direction_angle,
            scene->drones[i].detection_angle
        );

        draw_drone_path(i);
        draw_drone(&scene->drones[i]);
    }
}
