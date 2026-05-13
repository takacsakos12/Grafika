#include "scene_update.h"
#include "scene_drone.h"

#include <stdio.h>

void update_exit_door(Scene* scene, double delta_time)
{
    float max_offset = 2.0f;
    float open_speed = 1.0f;

    if (!scene->exit_door_open) {
        return;
    }

    if (scene->exit_door_offset < max_offset) {
        scene->exit_door_offset += (float)(delta_time * open_speed);
    }

    if (scene->exit_door_offset > max_offset) {
        scene->exit_door_offset = max_offset;
    }

    if (scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = false;
    }
}

void update_inner_doors(Scene* scene, double delta_time)
{
    int i;

    for (i = 0; i < scene->inner_door_count; ++i) {
        InnerDoor* door = &scene->inner_doors[i];

        if (door->open) {
            if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = false;
            }

            if (door->open_offset < 90.0f) {
                door->open_offset += (float)(delta_time * 120.0f);
            }

            if (door->open_offset > 90.0f) {
                door->open_offset = 90.0f;
            }

            door->auto_close_timer += (float)delta_time;

            if (door->auto_close_timer >= 5.0f) {
                door->open = false;
                door->auto_close_timer = 0.0f;
            }
        }
        else {
            if (door->open_offset > 0.0f) {
                door->open_offset -= (float)(delta_time * 120.0f);
            }

            if (door->open_offset <= 0.0f) {
                door->open_offset = 0.0f;

                if (door->collider_index >= 0) {
                    scene->colliders[door->collider_index].active = true;
                }
            }
            else if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = false;
            }
        }
    }
}

void update_drones_and_detection(
    Scene* scene,
    double delta_time,
    float player_x,
    float player_z
)
{
    int i;

    for (i = 0; i < scene->drone_count; ++i) {
        update_drone(scene, &scene->drones[i], i, delta_time);

        if (is_player_detected_by_drone(scene, &scene->drones[i], player_x, player_z)) {
            scene->game_over = true;
            printf("GAME OVER: Drone %d detected the player!\n", i + 1);
            return;
        }
    }
}
