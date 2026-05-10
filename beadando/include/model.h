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

int load_model(Model* model, const char* filename);
int load_model_flip_v(Model* model, const char* filename);
void render_model(const Model* model);
void destroy_model(Model* model);

#endif