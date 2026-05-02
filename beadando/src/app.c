#include "app.h"

#include <GL/gl.h>


#include <stdlib.h>
#include <string.h>

static void create_help_texture(App* app)
{
    const char* help_text =
    "HASZNÁLATI ÚTMUTATÓ\n\n"

    "Cél:\n"
    "Aktiváld a 3 energiagenerátort.\n"
    "Ezután kinyílik a kijárati ajtó.\n\n"

    "Irányítás:\n"
    "Egér - nézelődés\n"
    "W A S D - mozgás\n"
    "E - interakció\n"
    "+ / - - fényerő állítása\n"
    "3 - Hard Mode be/ki\n"
    "F1 - súgó be/ki\n"
    "ESC - kilépés\n\n"
    "R - játék újraindítása\n"

    "Figyelem:\n"
    "Hard Mode-ban köd csökkenti a látótávolságot.\n"
    "Ha a drón észlel, a játék véget ér.";

    SDL_Color text_color = {255, 255, 255, 255};

    SDL_Surface* surface;
    SDL_Surface* formatted_surface;

    surface = TTF_RenderUTF8_Blended_Wrapped(
        app->font,
        help_text,
        text_color,
        700
    );

    if (surface == NULL) {
        fprintf(stderr, "Help text render error: %s\n", TTF_GetError());
        return;
    }

    formatted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);

    if (formatted_surface == NULL) {
        fprintf(stderr, "Surface convert error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    app->help_texture_width = formatted_surface->w;
    app->help_texture_height = formatted_surface->h;

    glGenTextures(1, &app->help_texture);
    glBindTexture(GL_TEXTURE_2D, app->help_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted_surface->w,
        formatted_surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted_surface->pixels
    );

    SDL_FreeSurface(formatted_surface);
    SDL_FreeSurface(surface);
}

static void create_game_over_texture(App* app)
{
    const char* game_over_text =
        "GAME OVER\n\n"
        "A dron eszrevett.\n\n"
        "R - ujrainditas\n"
        "ESC - kilepes";

    SDL_Color text_color = {255, 255, 255, 255};

    SDL_Surface* surface;
    SDL_Surface* formatted_surface;

    surface = TTF_RenderUTF8_Blended_Wrapped(
        app->font,
        game_over_text,
        text_color,
        500
    );

    if (surface == NULL) {
        fprintf(stderr, "Game over text render error: %s\n", TTF_GetError());
        return;
    }

    formatted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);

    if (formatted_surface == NULL) {
        fprintf(stderr, "Game over surface convert error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    app->game_over_texture_width = formatted_surface->w;
    app->game_over_texture_height = formatted_surface->h;

    glGenTextures(1, &app->game_over_texture);
    glBindTexture(GL_TEXTURE_2D, app->game_over_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted_surface->w,
        formatted_surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted_surface->pixels
    );

    SDL_FreeSurface(formatted_surface);
    SDL_FreeSurface(surface);
}

static void create_win_texture(App* app)
{
    const char* win_text =
        "SIKERES KIJUTAS\n\n"
        "Mindharom generator aktiv.\n"
        "A kijarati ajton keresztul elhagytad a bazist.\n\n"
        "R - ujrainditas\n"
        "ESC - kilepes";

    SDL_Color text_color = {255, 255, 255, 255};

    SDL_Surface* surface;
    SDL_Surface* formatted_surface;

    surface = TTF_RenderUTF8_Blended_Wrapped(
        app->font,
        win_text,
        text_color,
        600
    );

    if (surface == NULL) {
        fprintf(stderr, "Win text render error: %s\n", TTF_GetError());
        return;
    }

    formatted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_ABGR8888, 0);

    if (formatted_surface == NULL) {
        fprintf(stderr, "Win surface convert error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    app->win_texture_width = formatted_surface->w;
    app->win_texture_height = formatted_surface->h;

    glGenTextures(1, &app->win_texture);
    glBindTexture(GL_TEXTURE_2D, app->win_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        formatted_surface->w,
        formatted_surface->h,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        formatted_surface->pixels
    );

    SDL_FreeSurface(formatted_surface);
    SDL_FreeSurface(surface);
}

static void draw_win_overlay(const App* app)
{
    int screen_width = 1024;
    int screen_height = 768;

    int padding = 30;

    int texture_width = app->win_texture_width;
    int texture_height = app->win_texture_height;

    int x = (screen_width - texture_width) / 2;
    int y = (screen_height - texture_height) / 2;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, screen_width, screen_height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* teljes képernyős sötétítés */
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.55f);

    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(screen_width, 0);
    glVertex2i(screen_width, screen_height);
    glVertex2i(0, screen_height);
    glEnd();

    /* középső panel */
    glColor4f(0.02f, 0.18f, 0.08f, 0.90f);

    glBegin(GL_QUADS);
    glVertex2i(x - padding, y - padding);
    glVertex2i(x + texture_width + padding, y - padding);
    glVertex2i(x + texture_width + padding, y + texture_height + padding);
    glVertex2i(x - padding, y + texture_height + padding);
    glEnd();

    /* szöveg textúra */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, app->win_texture);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(x, y);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x + texture_width, y);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x + texture_width, y + texture_height);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(x, y + texture_height);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

static void draw_game_over_overlay(const App* app)
{
    int screen_width = 1024;
    int screen_height = 768;

    int padding = 30;

    int texture_width = app->game_over_texture_width;
    int texture_height = app->game_over_texture_height;

    int x = (screen_width - texture_width) / 2;
    int y = (screen_height - texture_height) / 2;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, screen_width, screen_height, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* teljes képernyős sötétítés */
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.65f);

    glBegin(GL_QUADS);
    glVertex2i(0, 0);
    glVertex2i(screen_width, 0);
    glVertex2i(screen_width, screen_height);
    glVertex2i(0, screen_height);
    glEnd();

    /* középső panel */
    glColor4f(0.20f, 0.02f, 0.02f, 0.90f);

    glBegin(GL_QUADS);
    glVertex2i(x - padding, y - padding);
    glVertex2i(x + texture_width + padding, y - padding);
    glVertex2i(x + texture_width + padding, y + texture_height + padding);
    glVertex2i(x - padding, y + texture_height + padding);
    glEnd();

    /* Game Over textúra */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, app->game_over_texture);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(x, y);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x + texture_width, y);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x + texture_width, y + texture_height);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(x, y + texture_height);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

static void draw_help_overlay(const App* app)
{   glDisable(GL_FOG);
    int x = 40;
    int y = 40;
    int padding = 20;

    int w = app->help_texture_width;
    int h = app->help_texture_height;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0, 1024, 768, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* fekete háttérpanel */
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0f, 0.0f, 0.0f, 0.75f);

    glBegin(GL_QUADS);
    glVertex2i(x - padding, y - padding);
    glVertex2i(x + w + padding, y - padding);
    glVertex2i(x + w + padding, y + h + padding);
    glVertex2i(x - padding, y + h + padding);
    glEnd();

    /* help szöveg textúra */
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, app->help_texture);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(x, y);

    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(x + w, y);

    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(x + w, y + h);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(x, y + h);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

void init_app(App* app, int width, int height)
{
    app->window = NULL;
    app->gl_context = NULL;
    app->is_running = true;
    app->uptime = 0.0;
    app->key_w = false;
    app->key_a = false;
    app->key_s = false;
    app->key_d = false;
    app->show_help = false;
    app->font = NULL;
    app->help_texture = 0;
    app->help_texture_width = 0;
    app->help_texture_height = 0;
    app->game_over_texture = 0;
    app->game_over_texture_width = 0;
    app->game_over_texture_height = 0;
    app->win_texture = 0;
    app->win_texture_width = 0;
    app->win_texture_height = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization error: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1) {
    fprintf(stderr, "SDL_ttf initialization error: %s\n", TTF_GetError());
    SDL_Quit();
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
    app->font = TTF_OpenFont("assets/fonts/arial.ttf", 22);

    create_help_texture(app);
    create_game_over_texture(app);
    create_win_texture(app);

    if (app->font == NULL) {
    fprintf(stderr, "Font loading error: %s\n", TTF_GetError());
    destroy_app(app);
    exit(1);
    }

    create_help_texture(app);

    SDL_GL_SetSwapInterval(1);

    init_opengl();
    reshape(width, height);

    init_camera(&app->camera);
    init_scene(&app->scene);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_NORMALIZE);

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

    glFrustum(-0.08, 0.08, -0.06, 0.06, 0.1, 1000.0);

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

            case SDLK_w:
                app->key_w = true;
                break;

            case SDLK_a:
                app->key_a = true;
                break;

            case SDLK_s:
                app->key_s = true;
                break;

            case SDLK_d:
                app->key_d = true;
                break;
            
            case SDLK_F1:
                app->show_help = !app->show_help;
                break;
            
            case SDLK_e:
                interact_scene(
                &app->scene,
                (float)app->camera.position.x,
                (float)app->camera.position.z
                );
                break;
            case SDLK_PLUS:
                case SDLK_KP_PLUS:
                change_scene_brightness(&app->scene, 0.1f);
                break;

            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                change_scene_brightness(&app->scene, -0.1f);
                break;
            case SDLK_3:
                toggle_hard_mode(&app->scene);
                break;
            case SDLK_r:
                reset_scene(&app->scene);
                reset_camera(&app->camera);

                app->key_w = false;
                app->key_a = false;
                app->key_s = false;
                app->key_d = false;

                break;                   

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_w:
                app->key_w = false;
                break;

            case SDLK_a:
                app->key_a = false;
                break;

            case SDLK_s:
                app->key_s = false;
                break;

            case SDLK_d:
                app->key_d = false;
                break;

            default:
                break;
            }
            break;

        case SDL_MOUSEMOTION:
                if (!app->show_help) {
                rotate_camera(&app->camera, event.motion.xrel, event.motion.yrel);
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

    if (!app->scene.game_over && !app->scene.game_won) {
        update_camera(
            &app->camera,
            &app->scene,
            delta_time,
            app->key_w,
            app->key_s,
            app->key_a,
            app->key_d
        );
    }

    update_scene(
        &app->scene,
        delta_time,
        (float)app->camera.position.x,
        (float)app->camera.position.z
    );
}

void render_app(App* app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    set_view(&app->camera);

    set_scene_lighting(&app->scene);
    apply_scene_fog(&app->scene);

    render_scene(&app->scene);

    if (app->show_help) {
        draw_help_overlay(app);
    }

    if (app->scene.game_over) {
        draw_game_over_overlay(app);
    }
    if (app->scene.game_won) {
    draw_win_overlay(app);
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App* app)
{
    destroy_scene(&app->scene);

    if (app->help_texture != 0) {
        glDeleteTextures(1, &app->help_texture);
        app->help_texture = 0;
    }

    if (app->game_over_texture != 0) {
        glDeleteTextures(1, &app->game_over_texture);
        app->game_over_texture = 0;
    }

    if (app->font != NULL) {
        TTF_CloseFont(app->font);
        app->font = NULL;
    }

    if (app->win_texture != 0) {
    glDeleteTextures(1, &app->win_texture);
    app->win_texture = 0;
    }

    TTF_Quit();

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