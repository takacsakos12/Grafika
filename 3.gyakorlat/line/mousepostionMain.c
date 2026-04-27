#include "line.c"

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;
    Point p1 = {10, 20};
    Point p2 = {100, 200};
    Color red = {255, 0, 0};

    Line line = Line_create(p1, p2, red);

    Line_print(&line);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Mouse position",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 960,
        0
    );

    if (!window) {
        printf("Window error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_MOUSEMOTION) {
                printf("Mouse position: x=%d y=%d\n",
                       event.motion.x,
                       event.motion.y);
                fflush(stdout); // azonnali kiírás
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}