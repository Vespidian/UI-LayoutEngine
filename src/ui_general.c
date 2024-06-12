#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "vectorlib.h"
#include "debug.h"
#include "ui.h"

UIState UINewState(){
	UIState state;
	state.num_classes = 0;
	state.num_elements = 0;
	return state;
}

UIClass *UINewClass(UIState *state){
	UIClass *c = &state->classes[0];
	if((state != NULL) && (state->num_classes < UI_STATE_MAX_CLASSES)){
		c = &state->classes[state->num_classes];

		c->id = state->num_classes++;
		c->offset = (iVector2){2147483647, 2147483647};

		c->size_min = (iVector2){-1, -1};
		c->size_max = (iVector2){-1, -1};

		c->padding = (iVector4){-1, -1, -1, -1};
		c->border = (iVector4){-1, -1, -1, -1};
		c->margin = (iVector4){-1, -1, -1, -1};
		c->color = (Vector3){-1, -1, -1};

		c->wrap = -1;
		c->wrap_vertical = -1;
		c->wrap_reverse = -1;
		c->origin_p = UI_ORIGIN_UNDEFINED;
		c->origin_c = UI_ORIGIN_UNDEFINED;

		c->culling = -1;
		c->inherit = false;

		/** Mouse Events **/
		c->class_hold = NULL;
		c->class_hover = NULL;

		c->event_func = NULL;
	}else{
		DebugLog(D_ERR, "%s:%s: error: Could not create new class: UI_STATE_MAX_CLASSES reached", __FILE__, __LINE__);
	}

	return c;
}

UIClass UIElementDefaultClass(){
	UIClass c;

	c.id = -1;
	c.offset = (iVector2){0, 0};

	c.size_min = (iVector2){100, 100};
	c.size_max = (iVector2){200, 200};

	c.padding = (iVector4){10, 10, 10, 10};
	c.border = (iVector4){0, 0, 0, 0};
	c.margin = (iVector4){10, 10, 10, 10};
	c.color = (Vector3){1, 1, 1};

	c.wrap = true;
	c.wrap_vertical = false;
	c.wrap_reverse = false;
	c.origin_p = UI_ORIGIN_NORTHWEST;
	c.origin_c = UI_ORIGIN_NORTHWEST;

	c.culling = false;
	c.inherit = false;

	/** Mouse Events **/
	c.class_hold = NULL;
	c.class_hover = NULL;

	c.event_func = NULL;
	
	return c;
}

UIElement *UINewElement(UIState *state){
	UIElement *e = &state->elements[0];

	if((state != NULL) && (state->num_elements < UI_STATE_MAX_ELEMENTS)){
		e = &state->elements[state->num_elements];

		e->id = state->num_elements++;

		e->text = NULL;

		e->num_children = 0;
		e->children = NULL;

		e->num_classes = 0;
		e->classes = NULL;
		
		e->num_tmp_classes = 0;
		e->tmp_classes = NULL;

		e->transform = (iVector4){0, 0, 100, 100};
		e->mouse_events = 0;
		e->style = UIElementDefaultClass();

		e->parent = NULL;
	}else{
		DebugLog(D_ERR, "%s:%s: error: Could not create new class: UI_STATE_MAX_ELEMENTS reached", __FILE__, __LINE__);
	}

	return e;
}



static void UIFreeElement(UIElement *element){
	free(element->children);
	element->children = NULL;
	element->num_children = 0;
	
	free(element->classes);
	element->classes = NULL;
	element->num_classes = 0;

	free(element->tmp_classes);
	element->tmp_classes = NULL;
	element->num_tmp_classes = 0;
}

void UIFreeState(UIState *state){
	if(state != NULL){
		for(int i = 0; i < state->num_elements; i++){
			UIFreeElement(&state->elements[i]);
		}
		state->num_elements = 0;
		state->num_classes = 0;
	}
}



void UIElementAddChild(UIElement *parent, UIElement *child){
	// Now to set up the parent
	child->parent = parent;
	if((parent != NULL) && (child != NULL)){
		UIElement **tmp = realloc(parent->children, sizeof(UIElement *) * (parent->num_children + 1));
		if(tmp != NULL){
			parent->children = tmp;
			parent->children[parent->num_children] = child;
			parent->num_children++;
		}
	}
}

void UIElementAddClass(UIElement *element, UIClass *class){
	if((element != NULL) && (class != NULL)){
		element->num_classes++;
		UIClass **tmp = realloc(element->classes, sizeof(UIClass *) * (element->num_classes + 1));
		if(tmp != NULL){
			element->classes = tmp;

			element->classes[element->num_classes - 1] = class;
		}else{
			DebugLog(D_ERR, "%s:%s: error: Could not allocate space in element's class buffer", __FILE__, __LINE__);
		}
	}
}

void UIElementAddTmpClass(UIElement *element, UIClass *class){
	if((element != NULL) && (class != NULL)){
		element->num_tmp_classes++;
		UIClass **tmp = realloc(element->tmp_classes, sizeof(UIClass *) * (element->num_tmp_classes + 1));
		if(tmp != NULL){
			element->tmp_classes = tmp;

			element->tmp_classes[element->num_tmp_classes - 1] = class;
		}else{
			DebugLog(D_ERR, "%s:%s: error: Could not allocate space in element's tmp class buffer", __FILE__, __LINE__);
		}
	}
}

// UIClass *UIElementFindClass(UIElement *element, int class_id){
// 	UIClass *class = NULL;
// 	if(element != NULL){
// 		for(int i = 0; i < element->num_classes; i++){
// 			if(element->classes[i].id == class_id){
// 				class = &element->classes[i];
// 			}
// 		}
// 	}

// 	return class;
// }

// UIClass *UIFindClass(UIState state, int class_id){
//     UIClass *c = NULL;
//     if(class_id >= 0){
//         c = &state.classes[class_id];
//     }
//     return c;
// }

// UIElement *UIFindElement(UIElement *root, int element_id){
// 	UIElement *element = NULL;
// 	if(root != NULL){
// 		// Initially we create an array of all children
// 		UIElement **children;
// 		unsigned int num_children = 1;
// 		children = malloc(sizeof(UIElement) * (num_children + 1));
// 		children[0] = root;
// 		children[num_children + 1] = NULL;

// 		// if(children[0]->num_children != 0){
// 			for(int i = 0; children[i] != NULL; i++){
// 				if(children[i]->id == element_id){
// 					element = children[i];
// 				}
// 				if(children[i]->num_children == 0){
// 					continue;
// 				}

// 				UIElement **tmp = realloc(children, sizeof(UIElement *) * (num_children + children[i]->num_children + 1));
// 				if(tmp != NULL){
// 					children = tmp;
// 					num_children += children[i]->num_children;

// 					for(int k = 0; k < children[i]->num_children; k++){
// 						children[num_children - children[i]->num_children + k] = &children[i]->children[k];
// 					}
// 				}

// 			}
// 		// }
// 	}

// 	return element;
// }