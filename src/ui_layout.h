#ifndef UI_LAYOUT_H_
#define UI_LAYOUT_H_


typedef enum UI_ORIGIN {
	UI_ORIGIN_UNDEFINED = 0,
	UI_ORIGIN_NORTHWEST,
	UI_ORIGIN_NORTH,
	UI_ORIGIN_NORTHEAST,
	UI_ORIGIN_WEST,
	UI_ORIGIN_CENTER,
	UI_ORIGIN_EAST,
	UI_ORIGIN_SOUTHWEST,
	UI_ORIGIN_SOUTH,
	UI_ORIGIN_SOUTHEAST
}UI_ORIGIN;

typedef enum UI_SCALE_TYPE {
	UI_SCALE_UNDEFINED = 0,
	UI_SCALE_PIXELS,
	UI_SCALE_PERCENT
}UI_SCALE_TYPE;


/** --- SEND TO 'ui_interact.c' --- **/
// typedef enum UI_MOUSE_EVENT{
// 	// Triggered once
// 	UI_MOUSE_CLICK   = 1 << 0,
// 	UI_MOUSE_RELEASE = 1 << 1,
// 	UI_MOUSE_ENTER   = 1 << 2,
// 	UI_MOUSE_LEAVE   = 1 << 3,

// 	// Triggered every frame
// 	UI_MOUSE_HOLD    = 1 << 4,
// 	UI_MOUSE_HOVER   = 1 << 5,
// }UI_MOUSE_EVENT;

/**
 * DEFAULTS: (When elements of a class have these values, 
 * they are ignored and not passed to the element)
 * 
 * UI_SCALE_TYPE scale_type			UI_SCALE_UNDEFINED
 * iVector2 offset					
 * iVector2 size_min				any xy < 0
 * iVector2 size_max				any xy < 0
 * iVector4 padding					any xyzw < 0
 * iVector4 border					any xyzw < 0
 * iVector4 margin					any xyzw < 0
 * Vector3 color					any xyz > 1
 * int8_t wrap						-1
 * int8_t wrap_vertical				-1
 * UI_ORIGIN origin_p				UI_ORIGIN_UNDEFINED
 * UI_ORIGIN origin_c				UI_ORIGIN_UNDEFINED
 * bool  inherit					N/A (not passed to element)
*/

typedef struct UIClass{
	UI_SCALE_TYPE scale_type;

	iVector2 offset;

	iVector2 size_min;
	iVector2 size_max;

	// padding
	iVector4 padding;

	// border
	iVector4 border;

	// margin
	iVector4 margin;

	// color
	Vector3 color;

	// Defines whether or not a child will be placed in the next 
	// row or column when there is no more room in the current
	int8_t wrap;

	// Defines whether to add new children to the next column (false) 
	// or next row (true)
	int8_t wrap_vertical;

	// The point of this element to align with the parent's child origin
	UI_ORIGIN origin_p;

	// Where on this element a child should align its parent origin
	UI_ORIGIN origin_c;

	// Whether or not children can be seen when outside the bounds 
	// of this parent element
	int8_t culling;

	// When 'inherit' is true, the children of this element will be 
	// automatically given this class
	bool inherit;
}UIClass;


typedef struct UIElement{
	struct UIElement *parent;
	struct UIElement **children;
	unsigned int num_children;

	UIClass *classes;
	unsigned int num_classes;

	// Final absolute positioning and scale
	Vector4 transform;

	// 
	uint8_t mouse_events;

	// The conglomerate of all classes in the class array 
	// (used for layout and rendering) (cannot be inherited)
	// 
	// This is a temporary class which is created by iterating 
	// through all classes in the classes array
	UIClass style;

}UIElement;

/**
 * LAYOUT STUFFS
*/

UIElement UINewElement(UIElement *parent);

void UIFreeElement(UIElement *element);

void UIElementAddClass(UIElement *element, UIClass class);


UIClass UINewClass();



/**
 * RENDER STUFFS
*/

void UIRenderElement(UIElement *element);

#endif