#include "app.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>

void init_app(App* app, int width, int height)
{
    app->window = NULL;
    app->gl_context = NULL;
    app->is_running = true;
    app->uptime = 0.0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    app->window = SDL_CreateWindow(
        "Abandoned Research Base",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (app->window == NULL) {
        fprintf(stderr, "Window creation error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    app->gl_context = SDL_GL_CreateContext(app->window);

    if (app->gl_context == NULL) {
        fprintf(stderr, "OpenGL context error: %s\n", SDL_GetError());
        SDL_DestroyWindow(app->window);
        SDL_Quit();
        exit(1);
    }

    SDL_GL_SetSwapInterval(1);

    init_opengl();
    reshape(width, height);

    init_camera(&app->camera);
    init_scene(&app->scene);
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(0.02f, 0.02f, 0.03f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void reshape(GLsizei width, GLsizei height)
{
    int x;
    int y;
    int w;
    int h;
    double ratio;

    if (height == 0) {
        height = 1;
    }

    ratio = (double)width / (double)height;

    if (ratio > VIEWPORT_RATIO) {
        w = (int)(height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)(width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(VIEWPORT_ASPECT, VIEWPORT_RATIO, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handle_app_events(App* app)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            app->is_running = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                app->is_running = false;
                break;

            default:
                break;
            }
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                reshape(event.window.data1, event.window.data2);
            }
            break;

        default:
            break;
        }
    }
}

void update_app(App* app)
{
    double current_time;
    double delta_time;

    current_time = (double)SDL_GetTicks() / 1000.0;
    delta_time = current_time - app->uptime;
    app->uptime = current_time;

    update_scene(&app->scene, delta_time);
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    set_view(&app->camera);

    render_scene(&app->scene);

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    destroy_scene(&app->scene);

    if (app->gl_context != NULL) {
        SDL_GL_DeleteContext(app->gl_context);
        app->gl_context = NULL;
    }

    if (app->window != NULL) {
        SDL_DestroyWindow(app->window);
        app->window = NULL;
    }

    SDL_Quit();
}