#include <stdbool.h>
#include <SDL2/SDL.h>

#include "textures.h"
#include "vectorlib.h"
#include "renderer/quad.h"

#include "ui_layout.h"

extern Texture texture;

void UIRenderElement(UIElement *element){
    if(element != NULL){
        SDL_Rect r = {
            element->transform.x,
            element->transform.y,
            element->transform.z,
            element->transform.w
        };

        RenderQuad(texture, NULL, &r, 0, (Vector4){1, 0, 1, 1}, 0);
    }
}