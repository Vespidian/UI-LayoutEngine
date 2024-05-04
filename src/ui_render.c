#include <stdbool.h>
#include <SDL2/SDL.h>

#include "textures.h"
#include "vectorlib.h"
#include "renderer/quad.h"

#include "ui.h"

extern Texture texture;

void UIRenderElement(UIElement *element){
    if(element != NULL){
        SDL_Rect r = {
            element->transform.x,
            element->transform.y,
            element->transform.z,
            element->transform.w
        };

        Vector4 color = {
            element->style.color.x,
            element->style.color.y,
            element->style.color.z,
            1
        };

        RenderQuad(texture, NULL, &r, 0, color, 0);
    }
}

void UIRenderSubElements(UIElement *element){
    if(element != NULL){
        // Initially we create an array of all children
        UIElement **children;
        unsigned int num_children = 1;
        children = malloc(sizeof(UIElement) * (num_children + 1));
        children[0] = element;
        children[num_children + 1] = NULL;

	    if(element->num_children != 0){
            for(int i = 0; children[i] != NULL; i++){
                if(children[i]->num_children == 0){
                    continue;
                }

                num_children += children[i]->num_children;
                children = realloc(children, sizeof(UIElement) * (num_children + 1));

                for(int k = 0; k < children[i]->num_children; k++){
                    children[num_children - children[i]->num_children + k] = &children[i]->children[k];
                }
                children[num_children] = NULL;
            }

            
        }else{
            UIRenderElement(element);
        }

        // Loop from leaves to root
        for(int i = (num_children - 1); i >= 0; i--){
            UIRenderElement(children[i]);
        }

        free(children);
        children = NULL;
    }
}