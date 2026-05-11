#ifndef MODEL_H
#define MODEL_H

#include <GL/gl.h>

#define MAX_MODEL_VERTICES 50000

/* Egyszeru OBJ modell csucspontja: pozicio es textura-koordinata. */
typedef struct ModelVertex
{
    float x;
    float y;
    float z;

    float u;
    float v;
} ModelVertex;

/* Betoltott modell adatai: haromszogekre bontott csucspontlista. */
typedef struct Model
{
    ModelVertex vertices[MAX_MODEL_VERTICES];
    int vertex_count;
} Model;

/* Betolt egy OBJ modellt alap textura-koordinatakkal. */
int load_model(Model* model, const char* filename);

/* Betolt egy OBJ modellt ugy, hogy a V textura-koordinatat megforditja. */
int load_model_flip_v(Model* model, const char* filename);

/* Kirajzolja a betoltott modellt OpenGL haromszogekkent. */
void render_model(const Model* model);

/* Kiuriti a modell adatait. */
void destroy_model(Model* model);

#endif