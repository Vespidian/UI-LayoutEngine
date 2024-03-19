#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "vectorlib.h"
#include "debug.h"
#include "ui_layout.h"

UIElement UINewElement(UIElement *parent){
    UIElement e;
    e.parent = parent;
    e.children = NULL;
    e.num_classes = 0;
    e.classes = NULL;
    e.transform = (Vector4){0, 0, 100, 100};
    e.mouse_events = 0;
    e.style = UINewClass();

    return e;
}

void UIFreeElement(UIElement *element){
    // Free all children
    UIElement **children;
    unsigned int num_children = 1;
    children = malloc(sizeof(UIElement) * (num_children + 1));
    children[0] = element;
    children[num_children + 1] = NULL;

    for(int i = 0; children[i] != NULL; i++){
        if(children[i]->num_children == 0){
            continue;
        }

        num_children += children[i]->num_children;
        children = realloc(children, sizeof(UIElement) * (num_children + 1));

        for(int k = 0; k < children[i]->num_children; k++){
            children[num_children - children[i]->num_children + k] = children[i]->children[k];
        }
    }

    // Now we go through and free each individual element
    for(int i = num_children - 1; i >= 0; i++){
        free(children[i]->classes);
        children[i]->classes = NULL;
        children[i]->num_classes = 0;

        if(children[i]->num_children != 0){
            free(children[i]->children);
            children[i]->children = NULL;
            children[i]->num_children = 0;
        }
    }

    free(children);
    children = NULL;
}

void UIElementAddClass(UIElement *element, UIClass class){
    if(element != NULL){
        element->num_classes++;
        UIClass *tmp = realloc(element->classes, sizeof(UIClass) * (element->num_classes + 1));
        if(tmp != NULL){
            element->classes = tmp;

            element->classes[element->num_classes - 1] = class;
        }else{
            DebugLog(D_ERR, "%s:%s: error: Could not allocate space in elemet's class buffer", __FILE__, __LINE__);
        }
    }
}

UIClass UINewClass(){
    UIClass c;

    c.offset = (iVector2){0, 0};

    c.size_min = (iVector2){50, 50};
    c.size_max = (iVector2){200, 200};

    c.padding = (iVector4){10, 10, 10, 10};
    c.border = (iVector4){0, 0, 0, 0};
    c.margin = (iVector4){10, 10, 10, 10};
    c.color = (Vector3){1, 1, 1};

    c.wrap_vertical = false;
    c.wrap = true;
    c.origin_p = UI_ORIGIN_NORTHWEST;
    c.origin_c = UI_ORIGIN_NORTHWEST;

    c.culling = false;
    c.inherit = false;
    
    return c;
}



/**
 * --- LAYOUT ---
*/

void UIElementApplyClass(UIElement *element, UIClass *class){
    // Individually apply each bit of the class if it has been defined
}

void UIElementUpdate(){
    // Look at parent for inherited classes
    // Look at children for size and any stretching
    // Look at element's own classes for other modification
}

void UIElementUpdateChildren(UIElement *element){
    // 1. iteratively creating an array of all nodes (elements)
    // 2. Iterate through the array backwards, updating each element based on 
    // the size of its immediate children and any classes inherited from 
    // the parent element

}

void UIElementUpdateSiblings(UIElement *element){
    // Update all siblings of 'element' as well as all siblings of all parents
    
    // Here, we start at a leaf of the tree. 
    // 1. Create a 'parent' variable, to store the parent of the current 
    // children we are looping through
    // 3. Loop through all immediate children of the 'parent' variable
    // 4. Update each child's size and classes based on its immediate children 
    // and any classes inherited from the parent element
    // 5. Set the 'parent' variable to its own parent
    // 6. Repeat steps 3 to 5 until the root parent

}