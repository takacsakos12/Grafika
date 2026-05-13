#include "scene_particles.h"

#include <GL/gl.h>
#include <stdlib.h>

static float random_float(float min, float max)
{
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

void init_particles(Scene* scene)
{
    int i;

    for (i = 0; i < MAX_PARTICLES; ++i) {
        scene->particles[i].active = false;
        scene->particles[i].life = 0.0f;
        scene->particles[i].max_life = 0.0f;
    }
}

static void emit_spark(Scene* scene, float x, float y, float z)
{
    int i;

    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            scene->particles[i].x = x;
            scene->particles[i].y = y;
            scene->particles[i].z = z;

            scene->particles[i].vx = random_float(-1.5f, 1.5f);
            scene->particles[i].vy = random_float(1.0f, 3.0f);
            scene->particles[i].vz = random_float(-1.5f, 1.5f);

            scene->particles[i].max_life = random_float(0.3f, 0.8f);
            scene->particles[i].life = scene->particles[i].max_life;
            scene->particles[i].active = true;

            return;
        }
    }
}

void init_spark_sources(Scene* scene)
{
    scene->spark_sources[0].x = -15.0f;
    scene->spark_sources[0].y = 1.0f;
    scene->spark_sources[0].z = -11.0f;
    scene->spark_sources[0].timer = 0.0f;
    scene->spark_sources[0].next_emit_time = random_float(0.2f, 1.0f);

    scene->spark_sources[1].x = 10.5f;
    scene->spark_sources[1].y = 1.0f;
    scene->spark_sources[1].z = -16.5f;
    scene->spark_sources[1].timer = 0.0f;
    scene->spark_sources[1].next_emit_time = random_float(0.2f, 1.0f);

    scene->spark_sources[2].x = -6.2f;
    scene->spark_sources[2].y = 1.0f;
    scene->spark_sources[2].z = 18.0f;
    scene->spark_sources[2].timer = 0.0f;
    scene->spark_sources[2].next_emit_time = random_float(0.2f, 1.0f);
}

void update_particles(Scene* scene, double delta_time)
{
    int i;
    int source_index;
    float dt = (float)delta_time;

    for (source_index = 0; source_index < SPARK_SOURCE_COUNT; ++source_index) {
        scene->spark_sources[source_index].timer += dt;

        if (scene->spark_sources[source_index].timer >= scene->spark_sources[source_index].next_emit_time) {
            int burst_count = rand() % 4 + 2;
            int j;

            for (j = 0; j < burst_count; ++j) {
                emit_spark(
                    scene,
                    scene->spark_sources[source_index].x,
                    scene->spark_sources[source_index].y,
                    scene->spark_sources[source_index].z
                );
            }

            scene->spark_sources[source_index].timer = 0.0f;
            scene->spark_sources[source_index].next_emit_time = random_float(0.3f, 1.5f);
        }
    }

    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            continue;
        }

        scene->particles[i].life -= dt;

        if (scene->particles[i].life <= 0.0f) {
            scene->particles[i].active = false;
            continue;
        }

        scene->particles[i].x += scene->particles[i].vx * dt;
        scene->particles[i].y += scene->particles[i].vy * dt;
        scene->particles[i].z += scene->particles[i].vz * dt;
        scene->particles[i].vy -= 5.0f * dt;
    }
}

void draw_particles(const Scene* scene)
{
    int i;
    float alpha;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glPointSize(4.0f);

    glBegin(GL_POINTS);

    for (i = 0; i < MAX_PARTICLES; ++i) {
        if (!scene->particles[i].active) {
            continue;
        }

        alpha = scene->particles[i].life / scene->particles[i].max_life;
        glColor4f(1.0f, 0.65f, 0.05f, alpha);
        glVertex3f(scene->particles[i].x, scene->particles[i].y, scene->particles[i].z);
    }

    glEnd();

    glPointSize(1.0f);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
}
