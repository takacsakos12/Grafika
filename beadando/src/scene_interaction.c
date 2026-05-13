#include "scene_interaction.h"
#include "scene_drone.h"
#include "scene_particles.h"

#include <stdio.h>

void interact_scene(Scene* scene, float player_x, float player_z)
{
    int i;
    float dx;
    float dz;
    float distance_squared;
    float interaction_radius = 2.0f;

    for (i = 0; i < scene->inner_door_count; ++i) {
        dx = player_x - scene->inner_doors[i].x;
        dz = player_z - scene->inner_doors[i].z;
        distance_squared = dx * dx + dz * dz;

        if (distance_squared <= interaction_radius * interaction_radius) {
            InnerDoor* door = &scene->inner_doors[i];

            door->open = !door->open;
            door->auto_close_timer = 0.0f;

            if (door->collider_index >= 0) {
                scene->colliders[door->collider_index].active = false;
            }

            if (door->open) {
                printf("Inner door opened.\n");
            }
            else {
                printf("Inner door closing.\n");
            }

            return;
        }
    }

    for (i = 0; i < scene->generator_count; ++i) {
        if (scene->generators[i].active) {
            continue;
        }

        dx = player_x - scene->generators[i].x;
        dz = player_z - scene->generators[i].z;
        distance_squared = dx * dx + dz * dz;

        if (distance_squared <= interaction_radius * interaction_radius) {
            scene->generators[i].active = true;
            scene->active_generator_count++;

            printf(
                "Generator activated: %d/%d\n",
                scene->active_generator_count,
                scene->generator_count
            );

            if (scene->active_generator_count == scene->generator_count) {
                scene->exit_door_open = true;

                if (scene->exit_door_collider_index >= 0) {
                    scene->colliders[scene->exit_door_collider_index].active = false;
                }

                printf("Exit door opened!\n");
            }

            return;
        }
    }

    printf("No interactable object nearby.\n");
}

void reset_scene(Scene* scene)
{
    int i;

    scene->exit_door_open = false;
    scene->exit_door_offset = 0.0f;

    if (scene->exit_door_collider_index >= 0) {
        scene->colliders[scene->exit_door_collider_index].active = true;
    }

    scene->active_generator_count = 0;

    for (i = 0; i < scene->generator_count; ++i) {
        scene->generators[i].active = false;
    }

    for (i = 0; i < scene->inner_door_count; ++i) {
        scene->inner_doors[i].open = false;
        scene->inner_doors[i].open_offset = 0.0f;
        scene->inner_doors[i].auto_close_timer = 0.0f;

        if (scene->inner_doors[i].collider_index >= 0) {
            scene->colliders[scene->inner_doors[i].collider_index].active = true;
        }
    }

    scene->game_over = false;
    scene->game_won = false;

    init_drone(&scene->drones[0], 0);
    init_drone(&scene->drones[1], 1);
    init_particles(scene);
    init_spark_sources(scene);

    printf("Game restarted.\n");
}

bool is_player_at_exit(const Scene* scene, float player_x, float player_z)
{
    float exit_x = 3.0f;
    float exit_z = 19.0f;
    float exit_radius = 0.5f;
    float dx;
    float dz;
    float distance_squared;

    if (!scene->exit_door_open) {
        return false;
    }

    dx = player_x - exit_x;
    dz = player_z - exit_z;
    distance_squared = dx * dx + dz * dz;

    return distance_squared <= exit_radius * exit_radius;
}
