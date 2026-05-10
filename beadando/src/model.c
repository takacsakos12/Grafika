#include "model.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_POSITIONS 50000
#define MAX_TEXCOORDS 50000
#define MAX_FACE_VERTICES 8

typedef struct Position
{
    float x;
    float y;
    float z;
} Position;

typedef struct TexCoord
{
    float u;
    float v;
} TexCoord;

static Position positions[MAX_POSITIONS];
static TexCoord texcoords[MAX_TEXCOORDS];

static int position_count = 0;
static int texcoord_count = 0;

static void parse_face_vertex(const char* token, int* position_index, int* texcoord_index)
{
    char buffer[128];
    char* first_slash;
    char* second_slash;

    strcpy(buffer, token);

    *position_index = 0;
    *texcoord_index = 0;

    first_slash = strchr(buffer, '/');

    if (first_slash == NULL) {
        *position_index = atoi(buffer);
        return;
    }

    *first_slash = '\0';
    *position_index = atoi(buffer);

    second_slash = strchr(first_slash + 1, '/');

    if (second_slash != NULL) {
        *second_slash = '\0';
    }

    if (*(first_slash + 1) != '\0') {
        *texcoord_index = atoi(first_slash + 1);
    }
}

static void add_model_vertex(Model* model, int position_index, int texcoord_index)
{
    ModelVertex* vertex;
    Position position;
    TexCoord texcoord;

    if (model->vertex_count >= MAX_MODEL_VERTICES) {
        return;
    }

    if (position_index <= 0 || position_index > position_count) {
        return;
    }

    position = positions[position_index - 1];

    texcoord.u = 0.0f;
    texcoord.v = 0.0f;

    if (texcoord_index > 0 && texcoord_index <= texcoord_count) {
        texcoord = texcoords[texcoord_index - 1];
    }

    vertex = &model->vertices[model->vertex_count];

    vertex->x = position.x;
    vertex->y = position.y;
    vertex->z = position.z;

    /*
        Itt mar nem forditunk U/V koordinatat.
        A flipet a load_model_internal() intezi betolteskor.
    */
    vertex->u = texcoord.u;
    vertex->v = texcoord.v;

    model->vertex_count++;
}

static void add_triangle(
    Model* model,
    int p1, int t1,
    int p2, int t2,
    int p3, int t3
)
{
    add_model_vertex(model, p1, t1);
    add_model_vertex(model, p2, t2);
    add_model_vertex(model, p3, t3);
}

static int load_model_internal(
    Model* model,
    const char* filename,
    int flip_u,
    int flip_v
)
{
    FILE* file;
    char line[512];

    model->vertex_count = 0;
    position_count = 0;
    texcoord_count = 0;

    file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "Model loading error: %s\n", filename);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "v ", 2) == 0) {
            if (position_count < MAX_POSITIONS) {
                sscanf(
                    line,
                    "v %f %f %f",
                    &positions[position_count].x,
                    &positions[position_count].y,
                    &positions[position_count].z
                );

                position_count++;
            }
        }
        else if (strncmp(line, "vt ", 3) == 0) {
            if (texcoord_count < MAX_TEXCOORDS) {
                sscanf(
                    line,
                    "vt %f %f",
                    &texcoords[texcoord_count].u,
                    &texcoords[texcoord_count].v
                );

                /*
                    Ha a texturak rosszul jelennek meg,
                    akkor modellenkent lehet U/V flipet kerni.
                */
                if (flip_u) {
                    texcoords[texcoord_count].u = 1.0f - texcoords[texcoord_count].u;
                }

                if (flip_v) {
                    texcoords[texcoord_count].v = 1.0f - texcoords[texcoord_count].v;
                }

                texcoord_count++;
            }
        }
        else if (strncmp(line, "f ", 2) == 0) {
            char* token;
            char* tokens[MAX_FACE_VERTICES];
            int token_count = 0;

            int p[MAX_FACE_VERTICES];
            int t[MAX_FACE_VERTICES];

            token = strtok(line + 2, " \t\r\n");

            while (token != NULL && token_count < MAX_FACE_VERTICES) {
                tokens[token_count] = token;
                token_count++;

                token = strtok(NULL, " \t\r\n");
            }

            if (token_count >= 3) {
                int i;

                for (i = 0; i < token_count; ++i) {
                    parse_face_vertex(tokens[i], &p[i], &t[i]);
                }

                /*
                    Ha a face haromszognel tobb pontbol all,
                    akkor triangle fan modon haromszogekre bontjuk.
                */
                for (i = 1; i < token_count - 1; ++i) {
                    add_triangle(
                        model,
                        p[0], t[0],
                        p[i], t[i],
                        p[i + 1], t[i + 1]
                    );
                }
            }
        }
    }

    fclose(file);

    printf("Model loaded: %s, vertices: %d\n", filename, model->vertex_count);

    return 1;
}

int load_model(Model* model, const char* filename)
{
    return load_model_internal(model, filename, 0, 0);
}

void render_model(const Model* model)
{
    int i;

    glBegin(GL_TRIANGLES);

    for (i = 0; i < model->vertex_count; ++i) {
        glTexCoord2f(
            model->vertices[i].u,
            model->vertices[i].v
        );

        glVertex3f(
            model->vertices[i].x,
            model->vertices[i].y,
            model->vertices[i].z
        );
    }

    glEnd();
}

void destroy_model(Model* model)
{
    model->vertex_count = 0;
}