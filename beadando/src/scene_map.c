#include "scene_map.h"
#include "scene_builder.h"

void init_scene_walls(Scene* scene)
{
    add_vwall(scene, -20.0f, -20.0f, 20.0f);
    add_hwall(scene, -20.0f, 20.0f, -20.0f);
    add_vwall(scene, 20.0f, -20.0f, 20.0f);
    add_hwall(scene, -20.0f, 1.5f, 20.0f);
    add_hwall(scene, 4.5f, 20.0f, 20.0f);
    add_hwall(scene, -20.0f, -10.0f, 0.0f);
    add_hwall(scene, -20.0f, -14.5f, 16.0f);
    add_vwall(scene, -13.0f, 16.0f, 20.0f);
    add_hwall(scene, -20.0f, -19.0f, 10.0f);
    add_hwall(scene, -17.5f, -17.0f, 10.0f);
    add_vwall(scene, -17.0f, 10.0f, 11.0f);
    add_hwall(scene, -17.0f, -14.0f, 11.0f);
    add_hwall(scene, -14.0f, -13.0f, 11.0f);
    add_vwall(scene, -13.0f, 11.0f, 13.0f);
    add_vwall(scene, -13.0f, 14.0f, 16.0f);
    add_hwall(scene, 1.0f, 5.0f, 17.0f);
    add_vwall(scene, 5.0f, 10.75f, 17.0f);
    add_vwall(scene, -2.0f, 18.0f, 20.0f);
    add_hwall(scene, -5.0f, -2.0f, 15.0f);
    add_vwall(scene, -5.0f, 13.0f, 15.0f);
    add_vwall(scene, -2.0f, 15.0f, 18.0f);
    add_vwall(scene, -5.0f, 10.75f, 13.0f);
    add_hwall(scene, -11.5f, -5.0f, 12.0f);
    add_vwall(scene, -11.5f, 12.0f, 16.0f);
    add_vwall(scene, -11.5f, 16.0f, 18.5f);
    add_hwall(scene, -11.5f, -9.5f, 18.5f);
    add_vwall(scene, -9.5f, 17.0f, 18.5f);
    add_vwall(scene, -8.0f, 15.5f, 20.0f);
    add_hwall(scene, -10.0f, -8.0f, 15.5f);
    add_vwall(scene, -10.0f, 13.5f, 15.5f);
    add_hwall(scene, -10.0f, -8.0f, 13.5f);
    add_vwall(scene, 11.0f, 10.0f, 20.0f);
    add_hwall(scene, 11.0f, 18.5f, 10.0f);
    add_vwall(scene, 18.5f, 10.0f, 12.0f);
    add_hwall(scene, 13.0f, 18.5f, 12.0f);
    add_hwall(scene, 13.0f, 20.0f, 15.0f);
    add_vwall(scene, 13.0f, 15.0f, 18.0f);
    add_vwall(scene, -2.0f, -20.0f, -12.0f);
    add_vwall(scene, 2.0f, -20.0f, -10.0f);
    add_hwall(scene, -6.0f, -2.0f, -10.0f);
    add_hwall(scene, 2.0f, 4.0f, -10.0f);
    add_vwall(scene, -6.0f, -10.0f, -9.0f);
    add_hwall(scene, -7.0f, -6.0f, -9.0f);
    add_vwall(scene, -7.0f, -20.0f, -9.0f);
    add_hwall(scene, -10.0f, -7.0f, -9.0f);
    add_vwall(scene, -10.0f, -9.0f, -6.0f);
    add_hwall(scene, -14.0f, -10.0f, -6.0f);
    add_hwall(scene, -20.0f, -16.25f, -6.0f);
    add_hwall(scene, -14.75f, -10.0f, -6.0f);
    add_vwall(scene, -10.0f, -2.0f, 0.75f);
    add_hwall(scene, -20.0f, -16.0f, -14.0f);
    add_vwall(scene, -16.0f, -16.0f, -13.75f);
    add_hwall(scene, -16.0f, -10.0f, -16.0f);
    add_vwall(scene, -10.0f, -16.0f, -14.0f);
    add_hwall(scene, -20.0f, -14.0f, -8.0f);
    add_vwall(scene, -14.0f, -10.0f, -8.0f);
    add_hwall(scene, -16.0f, -14.0f, -10.0f);
    add_vwall(scene, -16.0f, -12.25f, -10.0f);
    add_hwall(scene, -16.0f, -14.0f, -12.0f);
    add_vwall(scene, -10.0f, 2.25f, 9.0f);
    add_hwall(scene, -10.0f, -6.0f, 9.0f);
    add_hwall(scene, -6.0f, -2.5f, 9.0f);
    add_vwall(scene, -2.5f, 9.0f, 11.0f);
    add_hwall(scene, -2.5f, 1.0f, 11.0f);
    add_vwall(scene, 1.0f, 9.0f, 11.0f);
    add_hwall(scene, 1.0f, 2.0f, 9.0f);
    add_vwall(scene, -14.0f, 8.0f, 11.0f);
    add_hwall(scene, -15.0f, -14.0f, 8.0f);
    add_vwall(scene, -15.0f, 4.0f, 8.0f);
    add_hwall(scene, -15.0f, -13.0f, 4.0f);
    add_vwall(scene, -13.0f, 2.0f, 4.0f);
    add_vwall(scene, -16.0f, 0.2f, 2.0f);
    add_hwall(scene, -16.0f, -14.5f, 2.0f);
    add_hwall(scene, -20.0f, -18.0f, 5.0f);
    add_vwall(scene, -18.0f, 5.0f, 6.0f);
    add_hwall(scene, -18.0f, -17.0f, 6.0f);
    add_vwall(scene, -17.0f, 5.0f, 6.0f);
    add_hwall(scene, -17.0f, -15.0f, 5.0f);
    add_vwall(scene, 5.0f, -5.0f, 9.25f);
    add_vwall(scene, -6.0f, -5.0f, 9.0f);
    add_hwall(scene, -4.0f, 5.0f, -5.0f);
    add_vwall(scene, 10.0f, 4.0f, 6.0f);
    add_hwall(scene, 10.0f, 14.0f, 4.0f);
    add_vwall(scene, 10.0f, 6.0f, 7.0f);
    add_hwall(scene, 8.0f, 10.0f, 7.0f);
    add_vwall(scene, 8.0f, 7.0f, 9.0f);
    add_hwall(scene, 5.0f, 8.0f, 9.0f);
    add_vwall(scene, 14.0f, 0.25f, 4.0f);
    add_vwall(scene, 14.0f, -5.0f, -1.25f);
    add_hwall(scene, 11.0f, 14.0f, -5.0f);
    add_vwall(scene, 7.0f, -20.0f, -10.0f);
    add_hwall(scene, 7.0f, 14.0f, -10.0f);
    add_hwall(scene, 17.0f, 20.0f, -10.0f);
    add_vwall(scene, 14.0f, -18.5f, -10.0f);
    add_hwall(scene, 18.0f, 20.0f, -8.0f);
}

void init_scene_doors(Scene* scene)
{
    add_inner_door(scene, -5.0f, 10.0f, 90.0f, 1.0f, -1.0f);
    add_inner_door(scene, -16.0f, -12.85f, 90.0f, 1.0f, -1.0f);
    add_inner_door(scene, -10.0f, 1.5f, 90.0f, 1.0f, -1.0f);
    add_inner_door(scene, 5.0f, 10.0f, 90.0f, -1.0f, 1.0f);
    add_inner_door(scene, 14.0f, -0.5f, 90.0f, -1.0f, 1.0f);
    add_inner_door(scene, -15.5f, -6.0f, 0.0f, 1.0f, -1.0f);
}

void init_scene_plants(Scene* scene)
{
    add_plant_capsule(scene, -17.0f, 0.0f, 0.8f, 0.0f, 0.5f); add_collider(scene, -17.0f, 0.8f, 1.0f, 1.0f);
    add_plant_capsule(scene, 4.0f, 0.0f, 2.5f, 0.0f, 0.5f); add_collider(scene, 4.0f, 2.5f, 1.0f, 1.0f);
    add_plant_capsule(scene, -15.5f, 0.0f, 10.0f, 90.0f, 0.5f); add_collider(scene, -15.5f, 10.0f, 1.0f, 1.0f);
    add_plant_capsule(scene, -12.0f, 0.0f, 5.2f, 180.0f, 0.5f); add_collider(scene, -12.0f, 5.2f, 1.0f, 1.0f);
    add_plant_capsule(scene, -11.6f, 0.0f, 7.8f, 90.0f, 0.5f); add_collider(scene, -11.6f, 7.8f, 1.0f, 1.0f);
    add_plant_capsule(scene, -4.9f, 0.0f, 7.8f, 0.0f, 0.5f); add_collider(scene, -4.9f, 7.8f, 1.0f, 1.0f);
    add_plant_capsule(scene, -2.0f, 0.0f, 4.6f, 180.0f, 0.5f); add_collider(scene, -2.0f, 4.6f, 1.0f, 1.0f);
    add_plant_capsule(scene, 4.0f, 0.0f, 5.0f, 0.0f, 0.5f); add_collider(scene, 4.0f, 5.0f, 1.0f, 1.0f);
    add_plant_capsule(scene, 6.8f, 0.0f, 7.6f, 90.0f, 0.5f); add_collider(scene, 6.8f, 7.6f, 1.0f, 1.0f);
    add_plant_capsule(scene, 2.0f, 0.0f, -2.8f, 180.0f, 0.5f); add_collider(scene, 2.0f, -2.8f, 1.0f, 1.0f);
    add_plant_capsule(scene, 13.0f, 0.0f, 1.2f, 90.0f, 0.5f); add_collider(scene, 13.0f, 1.2f, 1.0f, 1.0f);
    add_plant_capsule(scene, 10.0f, 0.0f, 14.0f, 180.0f, 0.5f); add_collider(scene, 10.0f, 14.0f, 1.0f, 1.0f);
    add_plant_capsule(scene, 14.6f, 0.0f, 17.2f, 90.0f, 0.5f); add_collider(scene, 14.6f, 17.2f, 1.0f, 1.0f);
    add_plant_capsule(scene, 15.5f, 0.0f, 16.6f, 0.0f, 0.5f); add_collider(scene, 15.5f, 16.6f, 1.0f, 1.0f);
    add_plant_capsule(scene, 10.0f, 0.0f, -15.0f, 180.0f, 0.5f); add_collider(scene, 10.0f, -15.0f, 1.0f, 1.0f);
    add_plant_capsule(scene, 18.0f, 0.0f, -14.0f, 90.0f, 0.5f); add_collider(scene, 18.0f, -14.0f, 1.0f, 1.0f);
    add_plant_capsule(scene, -6.0f, 0.0f, 16.5f, 90.0f, 0.5f); add_collider(scene, -6.0f, 16.5f, 1.0f, 1.0f);
    add_plant_capsule(scene, -17.0f, 0.0f, 14.4f, 0.0f, 0.5f); add_collider(scene, -17.0f, 14.4f, 1.0f, 1.0f);
}

void init_scene_generators(Scene* scene)
{
    add_generator(scene, -15.0f, -11.0f); add_collider(scene, -15.0f, -11.0f, 1.2f, 1.2f);
    add_generator(scene, 10.5f, -16.5f); add_collider(scene, 10.5f, -16.5f, 1.2f, 1.2f);
    add_generator(scene, -6.2f, 18.0f); add_collider(scene, -6.2f, 18.0f, 1.2f, 1.2f);
}

void init_scene_crates(Scene* scene)
{
    add_crate(scene, -18.7f, 0.7f, -18.7f, 0.0f, 0.01f); add_collider(scene, -18.7f, -18.7f, 1.8f, 1.8f);
    add_crate(scene, -16.2f, 0.7f, -18.5f, 90.0f, 0.01f); add_collider(scene, -16.2f, -18.5f, 1.8f, 1.8f); add_crate(scene, -16.2f, 1.5f, -18.5f, 90.0f, 0.01f);
    add_crate(scene, -12.2f, 0.7f, -18.7f, 90.0f, 0.01f); add_collider(scene, -12.2f, -18.7f, 1.8f, 1.8f);
    add_crate(scene, -18.7f, 0.7f, -11.5f, 0.0f, 0.01f); add_collider(scene, -18.7f, -11.5f, 1.8f, 1.8f);
    add_crate(scene, -13.0f, 0.7f, -12.2f, 90.0f, 0.01f); add_collider(scene, -13.0f, -12.2f, 1.8f, 1.8f); add_crate(scene, -13.0f, 1.5f, -12.2f, 90.0f, 0.01f);
    add_crate(scene, -11.8f, 0.7f, -15.0f, 0.0f, 0.01f); add_collider(scene, -11.8f, -15.0f, 1.8f, 1.8f);
    add_crate(scene, -18.7f, 0.7f, 7.2f, 0.0f, 0.01f); add_collider(scene, -18.7f, 7.2f, 1.8f, 1.8f);
    add_crate(scene, -11.8f, 0.7f, 2.8f, 90.0f, 0.01f); add_collider(scene, -11.8f, 2.8f, 1.8f, 1.8f);
    add_crate(scene, -8.0f, 0.7f, 6.2f, 0.0f, 0.01f); add_collider(scene, -8.0f, 6.2f, 1.8f, 1.8f);
    add_crate(scene, -3.5f, 0.7f, 7.2f, 90.0f, 0.01f); add_collider(scene, -3.5f, 7.2f, 1.8f, 1.8f);
    add_crate(scene, 3.5f, 0.7f, -13.0f, 0.0f, 0.01f); add_collider(scene, 3.5f, -13.0f, 1.8f, 1.8f);
    add_crate(scene, 4.0f, 0.7f, -7.0f, 90.0f, 0.01f); add_collider(scene, 4.0f, -7.0f, 1.8f, 1.8f);
    add_crate(scene, 8.7f, 0.7f, -18.7f, 0.0f, 0.01f); add_collider(scene, 8.7f, -18.7f, 1.8f, 1.8f);
    add_crate(scene, 12.0f, 0.7f, -13.5f, 90.0f, 0.01f); add_collider(scene, 12.0f, -13.5f, 1.8f, 1.8f);
    add_crate(scene, 15.5f, 0.7f, -15.0f, 90.0f, 0.01f); add_collider(scene, 15.5f, -15.0f, 1.8f, 1.8f); add_crate(scene, 15.5f, 1.5f, -15.0f, 90.0f, 0.01f);
    add_crate(scene, 16.7f, 0.7f, -11.5f, 0.0f, 0.01f); add_collider(scene, 16.7f, -11.5f, 1.8f, 1.8f);
    add_crate(scene, 11.7f, 0.7f, 2.5f, 0.0f, 0.01f); add_collider(scene, 11.7f, 2.5f, 1.8f, 1.8f);
    add_crate(scene, 16.7f, 0.7f, 5.5f, 90.0f, 0.01f); add_collider(scene, 16.7f, 5.5f, 1.8f, 1.8f);
    add_crate(scene, 12.0f, 0.7f, 7.8f, 0.0f, 0.01f); add_collider(scene, 12.0f, 7.8f, 1.8f, 1.8f);
    add_crate(scene, 9.0f, 0.7f, 11.5f, 0.0f, 0.01f); add_collider(scene, 9.0f, 11.5f, 1.8f, 1.8f);
    add_crate(scene, 15.8f, 0.7f, 13.2f, 0.0f, 0.01f); add_collider(scene, 15.8f, 13.2f, 1.8f, 1.8f);
    add_crate(scene, 17.4f, 0.7f, 17.2f, 90.0f, 0.01f); add_collider(scene, 17.4f, 17.2f, 1.8f, 1.8f);
    add_crate(scene, 7.5f, 0.7f, 15.5f, 0.0f, 0.01f); add_collider(scene, 7.5f, 15.5f, 1.8f, 1.8f);
}
