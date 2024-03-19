#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <stdbool.h>
#include <cglm/cglm.h>

#include "vectorlib.h"
#include "debug.h"
#include "sdl_gl_init.h"
#include "engine.h"
#include "event.h"

extern SDL_Window *window;
bool running = false;

void Init();
void Quit();

extern SDL_Event e;

unsigned int uniform_buffer;
mat4 view_matrix;

int main(int argc, char *argv[]){
    Init();

    while(running){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        EventListener();

        EngineLoop();
        SDL_GL_SwapWindow(window);
    }

    Quit();

    return 0;
}

void Init(){
    InitDebug();
    InitEvents();
    InitSDL();
    InitGL();

    EngineSetup();
    printf("Hello world!!\n");
}

void Quit(){
    running = false;
    EngineExit();

    
    QuitEvents();
    SDL_Quit();
    QuitDebug();
}
