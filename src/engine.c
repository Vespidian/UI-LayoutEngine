#include <stdbool.h>
#include <SDL2/SDL.h>

#include "engine.h"

#include "textures.h"
#include "shader.h"
#include "renderer/renderer.h"
#include "renderer/quad.h"
#include "ui_layout.h"

Shader shader;
Texture texture;

SDL_Rect rect = {10, 10, 40, 50};

void EngineSetup(){
    RendererInit();
    InitQuadRender();

    // Basic Texture
    texture = TextureOpen("../assets/texture.png");
    
    // Basic Shader
    shader = ShaderOpen("../assets/shader.shader");

}

void EngineExit(){
    RendererQuit();

    TextureFree(&texture);
    ShaderFree(&shader);
}

void EngineLoop(){
    
    // RenderQuad(texture, NULL, &rect, 0, (Vector4){1, 1, 0, 1}, 0);
    RenderQuad(texture, NULL, &rect, 0, (Vector4){1, 1, 0, 1}, 0);
    UIElement e = UINewElement(NULL);
    UIRenderElement(&e);
    PushRender();
}

