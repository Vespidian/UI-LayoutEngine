#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>


#include "vectorlib.h"
#include "event.h"
#include "ui.h"


// Returns a pointer to the element that was interacted with
// also sets the event_type variable to the events currently happening
void UIInteractGetEvent(UIState *state){
	UIElement *mouse_element = NULL;

	if(state != NULL){
		UIElement *element = &state->elements[0];
		UI_MOUSE_EVENT event_type = 0;

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
					children[num_children - children[i]->num_children + k] = children[i]->children[k];
				}
				children[num_children] = NULL;
			}   
		}

		// Loop from leaves to root
		// Find the first child-most element that contains the mouse
		for(int i = (num_children - 1); i >= 0; i--){
			if(
				(mouse_pos.x > children[i]->transform.x) && 
				(mouse_pos.x < (children[i]->transform.x + children[i]->transform.z)) &&
				(mouse_pos.y > children[i]->transform.y) && 
				(mouse_pos.y < (children[i]->transform.y + children[i]->transform.w)) &&
				mouse_element == NULL
			){
				mouse_element = children[i];
			}else{
				children[i]->mouse_events &= ~UI_MOUSE_HOVER;
			}

		}

		// Only test events if the mouse is hovering over an element
		if(mouse_element != NULL){
			// Mouse entered
			if((mouse_element->mouse_events & UI_MOUSE_HOVER) == 0){
				printf("entered\n");
				event_type |= UI_MOUSE_ENTER;
			}

			// Mouse hovered
			event_type |= UI_MOUSE_HOVER;
			mouse_element->mouse_events |= UI_MOUSE_HOVER;


			// Mouse held
			if(mouse_held && (((mouse_element->mouse_events & UI_MOUSE_CLICK) != 0) || ((mouse_element->mouse_events & UI_MOUSE_HOLD) != 0))){
				event_type |= UI_MOUSE_HOLD;
				mouse_element->mouse_events |= UI_MOUSE_HOLD;
				mouse_element->mouse_events &= ~(UI_MOUSE_CLICK);
			}

			// Mouse leave
			// TODO: add mouse leave functionality

			// Mouse released
			if(mouse_released){
				event_type |= UI_MOUSE_RELEASE;
				mouse_element->mouse_events &= ~(UI_MOUSE_HOLD);
			}

			// Mouse clicked
			if(mouse_clicked){
				event_type |= UI_MOUSE_CLICK;
				mouse_element->mouse_events |= UI_MOUSE_CLICK;
			}

			// Run event functions and set event classes
			for(int i = 0; i < mouse_element->num_classes; i++){

				if((event_type & UI_MOUSE_HOLD) != 0){
					if(mouse_element->classes[i]->class_hold != NULL){
						// UIElementAddTmpClass(mouse_element, mouse_element->classes[i]->class_hold);
						UIElementAddTmpClass(mouse_element, UIFindClass(state, mouse_element->classes[i]->class_hold));
					}
				}
				
				if((event_type & UI_MOUSE_HOVER) != 0){
					if(mouse_element->classes[i]->class_hover != NULL){
						// UIElementAddTmpClass(mouse_element, mouse_element->classes[i]->class_hover);
						UIElementAddTmpClass(mouse_element, UIFindClass(state, mouse_element->classes[i]->class_hover));
					}
				}

				if(mouse_element->classes[i]->event_func != NULL){
					mouse_element->classes[i]->event_func(mouse_element, event_type);
				}

			}
		}
	}
}

void UIClassSetEventFunc(UIClass *class, UIMouseEventFunc_c event_func){
	if(class != NULL){
		class->event_func = event_func;
	}
}

void UIClassSetEventClass_hold(UIClass *class, UIClass *event_class){
	if(class != NULL){
		// class->class_hold = event_class;
		free(class->class_hold);
		class->class_hold = malloc(sizeof(char) * strlen(event_class->name));
	}
}

void UIClassSetEventClass_hover(UIClass *class, UIClass *event_class){
	if(class != NULL){
		// class->class_hover = event_class;
		free(class->class_hover);
		class->class_hover = malloc(sizeof(char) * strlen(event_class->name));
	}
}