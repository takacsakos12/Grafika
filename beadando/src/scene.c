#include "scene.h"
#include "scene_assets.h"
#include "scene_builder.h"
#include "scene_map.h"
#include "scene_particles.h"
#include "scene_drone.h"
#include "scene_render.h"
#include "scene_update.h"
#include "scene_interaction.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void init_scene_state(Scene* scene)
{
    scene->floor_size = 20.0f;

    scene->collider_count = 0;
    scene->wall_count = 0;
    scene->crate_count = 0;
    scene->plant_capsule_count = 0;

    scene->exit_door_open = false;
    scene->exit_door_collider_index = -1;
    scene->exit_door_offset = 0.0f;

    scene->generator_count = 0;
    scene->active_generator_count = 0;

    scene->brightness = 1.0f;
    scene->hard_mode = false;
    scene->game_over = false;
    scene->game_won = false;

    scene->drone_count = MAX_DRONES;
    scene->inner_door_count = 0;
}

void init_scene(Scene* scene)
{
    init_scene_state(scene);

    srand((unsigned int)time(NULL));

    init_drone(&scene->drones[0], 0);
    init_drone(&scene->drones[1], 1);
    init_particles(scene);
    init_spark_sources(scene);

    load_scene_assets(scene);

    init_scene_walls(scene);
    scene->wall_count = scene->collider_count;

    scene->exit_door_collider_index = scene->collider_count;
    add_collider(scene, 3.0f, 19.7f, 3.0f, 0.4f);

    init_scene_doors(scene);
    init_scene_plants(scene);
    init_scene_generators(scene);
    init_scene_crates(scene);
}

void update_scene(Scene* scene, double delta_time, float player_x, float player_z)
{
    update_exit_door(scene, delta_time);
    update_particles(scene, delta_time);

    if (scene->game_over || scene->game_won) {
        return;
    }

    update_drones_and_detection(scene, delta_time, player_x, player_z);

    if (scene->game_over) {
        return;
    }

    if (is_player_at_exit(scene, player_x, player_z)) {
        scene->game_won = true;
        printf("MISSION COMPLETE: Player escaped!\n");
        return;
    }

    update_inner_doors(scene, delta_time);
}

void render_scene(const Scene* scene)
{
    render_scene_geometry(scene);
    render_scene_interactables(scene);
    draw_drone_system(scene);
    draw_particles(scene);
    render_scene_static_objects(scene);
}
