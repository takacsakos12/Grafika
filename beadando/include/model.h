#ifndef MODEL_H
#define MODEL_H

#include <GL/gl.h>

#define MAX_MODEL_VERTICES 50000

typedef struct ModelVertex
{
    float x;
    float y;
    float z;

    float u;
    float v;
} ModelVertex;

typedef struct Model
{
    ModelVertex vertices[MAX_MODEL_VERTICES];
    int vertex_count;
} Model;

/*
    Normal OBJ betoltes.
    Ezt hasznald azokhoz a modellekhez, amelyeknel a textura jol jelenik meg.
*/
int load_model(Model* model, const char* filename);

/*
    OBJ betoltes fuggolegesen forditott V textura koordinataval.
    Akkor hasznald, ha a textura fejjel lefele jelenik meg.
*/
int load_model_flip_v(Model* model, const char* filename);

/*
    OBJ betoltes vizszintesen forditott U textura koordinataval.
    Akkor hasznald, ha a textura bal-jobb iranyban tukrozott.
*/
int load_model_flip_u(Model* model, const char* filename);

/*
    OBJ betoltes U es V textura koordinata forditassal.
    Ez kb. 180 fokos textura forditasnak felel meg.
*/
int load_model_flip_uv(Model* model, const char* filename);

void render_model(const Model* model);
void destroy_model(Model* model);

#endif /* MODEL_H */