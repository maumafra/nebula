#include <SDL3/SDL.h>
#include <glad/glad.h>

void cleanup(SDL_Window *window);
void setGLContextAttributes();
void setGLBufferAttributes();

// cmake . -B build
// cmake --build build
// build\Debug\nebula

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
        return 1;
    }

    int width = 800;
    int height = 600;

    setGLContextAttributes();
    setGLBufferAttributes();

    SDL_Window *window = SDL_CreateWindow("nebula", 
        width,
        height,
        SDL_WINDOW_OPENGL
    );
    if (!window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", nullptr);
        cleanup(window);
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing GLAD", nullptr);
        return 1;
    }

    bool run = true;
    while (run) {

        glViewport(0, 0, width, height);

        SDL_Event event{0};
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT: {
                    run = false;
                    break;
                }
            }
        }

        // rendering
        // (state-setting function)
        glClearColor(0.07f, 0.0f, 0.125f, 1.0f);
        // (state-using function) fills the color buffer with the color configured by glClearColor
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // swap buffers
        SDL_GL_SwapWindow(window);
    }
    
    cleanup(window);
    return 0;
}

void setGLContextAttributes() {
    // OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // OpenGL Core Profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
}

void setGLBufferAttributes() {
    // 8 bits to each channel RGBA8888
    // meaning 256 values for each channel (0 to 255)
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // double buffer (back and front)
    // switch with SDL_GL_SwapWindow
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // if the framebuffer content is preserved when minimizing window
    SDL_GL_SetAttribute(SDL_GL_RETAINED_BACKING, 0);

    // z-buffer size (usually 24 bits)
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    // stencil buffer size (8 bits)
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
}

void cleanup(SDL_Window *window) {
    SDL_Log("%s", "Cleanup");
    SDL_DestroyWindow(window);
    SDL_Quit();
}
