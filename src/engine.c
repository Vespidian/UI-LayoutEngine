#include <stdbool.h>
#include <SDL2/SDL.h>

#include "engine.h"

#include "vectorlib.h"
#include "textures.h"
#include "shader.h"
#include "renderer/renderer.h"
#include "renderer/quad.h"
#include "ui_layout.h"

Shader shader;
Texture texture;

SDL_Rect rect = {10, 10, 40, 50};

UIElement root;
UIElement *e1;
UIClass class1;
UIClass class2;
UIClass class3;

void EngineSetup(){
    RendererInit();
    InitQuadRender();

    // Basic Texture
    texture = TextureOpen("../assets/texture.png");
    
    // Basic Shader
    shader = ShaderOpen("../assets/shader.shader");

    class1 = UINewClass();
    class1.color = (Vector3){0.839, 0.478, 0.360};
    // class1.wrap_vertical = true;
    // class1.wrap_reverse = true;
    class1.size_max = (iVector2){800, 800};
    // class1.origin_p = UI_ORIGIN_CENTER;
    // class1.origin_p = UI_ORIGIN_SOUTHWEST;
    // class1.origin_p = UI_ORIGIN_SOUTHWEST;
    // UIElementAddClass(&root, &class1);


    root = UINewElement();
    UIElementAddClass(&root, class1);
    root.transform.x = 10;
    root.transform.y = 100;

    UIElementAddChild(&root, UINewElement());
    UIElementAddChild(&root, UINewElement());
    UIElementAddChild(&root, UINewElement());
    UIElementAddChild(&root, UINewElement());

    UIElementAddChild(&root.children[0], UINewElement());
    UIElementAddChild(&root.children[0], UINewElement());

    UIElementAddChild(&root.children[1], UINewElement());
    UIElementAddChild(&root.children[1], UINewElement());


    class2 = UINewClass();
    class3 = UINewClass();
    class2.color = (Vector3){0.929, 0.443, 0.541};
    class2.size_max = (iVector2){700, 700};

    class3.color = (Vector3){0, 0.1, 0.541};
    class3.padding.x = 30;
    class3.padding.z = 50;
    // class2.origin_c = UI_ORIGIN_SOUTHWEST;
    UIElementAddClass(&root.children[0], class2);

    class2.wrap_vertical = true;
    UIElementAddClass(&root.children[1], class2);
    UIElementAddClass(&root.children[1], class3);

    UIElementUpdateChildren(&root);


    // WHEN COME BACK: 
    // * test out multiple classes on an element
    // * implement inherit class property (apprently this was already done)
    // * think about how we're going to do mouse interaction - mouse interaction mostly done (see TODOs)
    // - work on json UI file structure and parsing
    // - work on UI serialization to json
    // - get borders to render in the shader
    // - start thinking about percent scaling


}

void EngineExit(){
    RendererQuit();

    TextureFree(&texture);
    ShaderFree(&shader);
}

void EngineLoop(){
    
    // RenderQuad(texture, NULL, &rect, 0, (Vector4){1, 1, 0, 1}, 0);
    // RenderQuad(texture, NULL, &rect, 0, (Vector4){1, 1, 0, 1}, 0);
    // UIElement e = UINewElement(NULL);
    // for(int i = 0; i < root.num_children; i++){
    //     UIRenderElement(&root.children[i]);
    // }
    // UIRenderElement(&root);

    UIElementUpdateChildren(&root);


    UI_MOUSE_EVENT event;
    UIElement *element = UIInteractGetEvent(&root, &event);
    if(element != NULL){
        if((event & UI_MOUSE_ENTER) != 0){
            element->style.color = (Vector3){1, 0, 0};
        }

        if((event & UI_MOUSE_HOVER) != 0){
            element->style.color = (Vector3){0, 0, 1};
        }
        
        if((event & UI_MOUSE_HOLD) != 0){
            element->style.color = (Vector3){0, 1, 0};
        }
    }


    UIRenderSubElements(&root);
    // UIRenderSubElements(&root.children[0]);
    // UIRenderSubElements(&e2);

    PushRender();
}

