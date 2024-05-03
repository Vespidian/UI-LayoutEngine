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

typedef enum UI_MOUSE_EVENT{
	// Triggered once
	UI_MOUSE_CLICK   = 1 << 0,
	UI_MOUSE_RELEASE = 1 << 1,
	UI_MOUSE_ENTER   = 1 << 2,
	UI_MOUSE_LEAVE   = 1 << 3,

	// Triggered every frame
	UI_MOUSE_HOLD    = 1 << 4,
	UI_MOUSE_HOVER   = 1 << 5,
}UI_MOUSE_EVENT;

/**
 * IGNORES: (When elements of a class have these values, 
 * they are ignored and not passed to the element)
 * (Only the subelement with the specified value is ignored)
 * 
 * UI_SCALE_TYPE scale_type			UI_SCALE_UNDEFINED
 * iVector2 offset					any xy = 2147483647
 * iVector2 size_min				any xy = -1
 * iVector2 size_max				any xy = -1
 * iVector4 padding					any xyzw = -1
 * iVector4 border					any xyzw = -1
 * iVector4 margin					any xyzw = -1
 * Vector3 color					any xyz = -1
 * int8_t wrap						-1
 * int8_t wrap_vertical				-1
 * int8_t wrap_reverse				-1
 * UI_ORIGIN origin_p				UI_ORIGIN_UNDEFINED
 * UI_ORIGIN origin_c				UI_ORIGIN_UNDEFINED
 * int8_t culling					-1
 * bool  inherit					N/A (not passed to element)
 * 
 * 
 * DEFAULTS: (The element.style class's default values)
 * 
 * UI_SCALE_TYPE scale_type			UI_SCALE_PIXELS
 * iVector2 offset					(0, 0)
 * iVector2 size_min				100					(TBD)
 * iVector2 size_max				-1					(TBD)
 * iVector4 padding					10
 * iVector4 border					0
 * iVector4 margin					10
 * Vector3 color					(1, 1, 1)
 * int8_t wrap						true
 * int8_t wrap_vertical				false
 * int8_t wrap_reverse				false
 * UI_ORIGIN origin_p				UI_ORIGIN_NORTHWEST
 * UI_ORIGIN origin_c				UI_ORIGIN_NORTHWEST
 * bool  inherit					false
 * 
*/

typedef struct UIClass{
	int id;

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

	// Defines whether to add children in a forwards (left to right 
	// and top to bottom) or reverse order
	int8_t wrap_reverse;

	// The point of this element to align with the parent's child origin
	UI_ORIGIN origin_c;

	// Where on this element a child should align its parent origin
	UI_ORIGIN origin_p;

	// Whether or not children can be seen when outside the bounds 
	// of this parent element
	int8_t culling;

	// When 'inherit' is true, the children of this element will be 
	// automatically given this class
	bool inherit;
}UIClass;


typedef struct UIElement{
	struct UIElement *parent;
	struct UIElement *children;
	unsigned int num_children;

	UIClass *classes;
	unsigned int num_classes;

	// Array of classes which gets emptied when they are applied 
	// to the elmeent
	UIClass *tmp_classes;
	unsigned int num_tmp_classes;

	// Final absolute positioning and scale
	iVector4 transform;

	UI_MOUSE_EVENT mouse_events;

	// The conglomerate of all classes in the class array 
	// (used for layout and rendering) (cannot be inherited)
	// 
	// This is a temporary class which is created from inherited 
	// parent classes, and classes in the classes array
	UIClass style;


}UIElement;

/**
 * LAYOUT STUFFS
*/

UIElement UINewElement();
UIElement *UIElementAddChild(UIElement *parent, UIElement child);

void UIFreeElement(UIElement *element);

void UIElementAddClass(UIElement *element, UIClass class);

void UIElementUpdateChildren(UIElement *element);


UIClass UINewClass();

UIClass *UIElementFindClass(UIElement *element, int class_id);


/**
 * RENDER STUFFS
*/

void UIRenderElement(UIElement *element);
void UIRenderSubElements(UIElement *element);


/**
 * INTERACT STUFFS
*/

UIElement *UIInteractGetEvent(UIElement *element, UI_MOUSE_EVENT *event_type);

#endif