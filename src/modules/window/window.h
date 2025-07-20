#include <string>

#include <SDL3/SDL.h>

namespace nebula {

namespace window {

class Window final {
public:
    Window();
    ~Window();
    void setWindow(int width = 800, int height = 600);
    void close();
    int getWidth();
    int getHeight();
    void swapBuffers();

private:
    bool createWindow(int width, int height);
    void setGLContextAttributes();
    void setGLBufferAttributes();

    SDL_Window *window;
    SDL_GLContext glContext;

    int width = 800;
    int height = 600;
    std::string title;
};

}
}