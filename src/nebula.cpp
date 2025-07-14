#include <SDL3/SDL.h>

void cleanup(SDL_Window *window);

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }

    int width = 800;
    int height = 600;

    SDL_Window *window = SDL_CreateWindow("nebula", width, height, 0);

    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", nullptr);
        cleanup(window);
        return 1;
    }

    bool run = true;
    while (run) {
        SDL_Event event{0};
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    run = false;
                    break;
                }
            }
        }
    }
    
    cleanup(window);
    return 0;
}

void cleanup(SDL_Window *window) {
    SDL_Log("%s", "Cleanup");
    SDL_DestroyWindow(window);
    SDL_Quit();
}