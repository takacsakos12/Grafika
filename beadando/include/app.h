#ifndef APP_H
#define APP_H

#include "camera.h"
#include "scene.h"

#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>

#define VIEWPORT_RATIO (4.0 / 3.0)
#define VIEWPORT_ASPECT 50.0

typedef struct App
{
    SDL_Window* window;
    SDL_GLContext gl_context;

    bool is_running;
    double uptime;

    bool key_w;
    bool key_a;
    bool key_s;
    bool key_d;

    bool show_help;

    TTF_Font* font;
    GLuint help_texture;
    int help_texture_width;
    int help_texture_height;
    GLuint game_over_texture;
    int game_over_texture_width;
    int game_over_texture_height;
    GLuint win_texture;
    int win_texture_width;
    int win_texture_height;

    Camera camera;
    Scene scene;
} App;
/**
 * Initialize the application.
 */
void init_app(App* app, int width, int height);

/**
 * Initialize the OpenGL context.
 */
void init_opengl();

/**
 * Reshape the window.
 */
void reshape(GLsizei width, GLsizei height);

/**
 * Handle the events of the application.
 */
void handle_app_events(App* app);

/**
 * Update the application.
 */
void update_app(App* app);

/**
 * Render the application.
 */
void render_app(App* app);

/**
 * Destroy the application.
 */
void destroy_app(App* app);
#endif /* APP_H */