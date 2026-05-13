#include "scene_collision.h"

static float clamp_float(float value, float min, float max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

static bool circle_intersects_collider(
    float circle_x,
    float circle_z,
    float radius,
    const Collider* collider
)
{
    float min_x, max_x, min_z, max_z;
    float closest_x, closest_z;
    float dx, dz;

    if (!collider->active) {
        return false;
    }

    min_x = collider->x - collider->width / 2.0f;
    max_x = collider->x + collider->width / 2.0f;
    min_z = collider->z - collider->depth / 2.0f;
    max_z = collider->z + collider->depth / 2.0f;

    closest_x = clamp_float(circle_x, min_x, max_x);
    closest_z = clamp_float(circle_z, min_z, max_z);

    dx = circle_x - closest_x;
    dz = circle_z - closest_z;

    return dx * dx + dz * dz < radius * radius;
}

static bool line_intersects_collider(
    float x1,
    float z1,
    float x2,
    float z2,
    const Collider* collider
)
{
    float min_x, max_x, min_z, max_z;
    float dx, dz;
    float t_min = 0.0f;
    float t_max = 1.0f;
    float p, q, r;

    if (!collider->active) {
        return false;
    }

    min_x = collider->x - collider->width / 2.0f - 0.05f;
    max_x = collider->x + collider->width / 2.0f + 0.05f;
    min_z = collider->z - collider->depth / 2.0f - 0.05f;
    max_z = collider->z + collider->depth / 2.0f + 0.05f;

    dx = x2 - x1;
    dz = z2 - z1;

#define CLIP_EDGE(P_VALUE, Q_VALUE) \
    p = (P_VALUE); \
    q = (Q_VALUE); \
    if (p == 0.0f) { \
        if (q < 0.0f) { \
            return false; \
        } \
    } \
    else { \
        r = q / p; \
        if (p < 0.0f) { \
            if (r > t_max) { \
                return false; \
            } \
            if (r > t_min) { \
                t_min = r; \
            } \
        } \
        else { \
            if (r < t_min) { \
                return false; \
            } \
            if (r < t_max) { \
                t_max = r; \
            } \
        } \
    }

    CLIP_EDGE(-dx, x1 - min_x)
    CLIP_EDGE(dx, max_x - x1)
    CLIP_EDGE(-dz, z1 - min_z)
    CLIP_EDGE(dz, max_z - z1)

#undef CLIP_EDGE

    return true;
}

bool is_wall_between_drone_and_player(
    const Scene* scene,
    float drone_x,
    float drone_z,
    float player_x,
    float player_z
)
{
    int i;

    for (i = 0; i < scene->wall_count; ++i) {
        if (line_intersects_collider(
            drone_x,
            drone_z,
            player_x,
            player_z,
            &scene->colliders[i]
        )) {
            return true;
        }
    }

    return false;
}

bool check_collision(const Scene* scene, float x, float z, float radius)
{
    int i;

    for (i = 0; i < scene->collider_count; ++i) {
        if (circle_intersects_collider(x, z, radius, &scene->colliders[i])) {
            return true;
        }
    }

    return false;
}
