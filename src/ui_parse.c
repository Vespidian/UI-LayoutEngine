#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

// #include <jsmn.h>

#include "vectorlib.h"
#include "json_read.h"
#include "debug.h"
#include "ui.h"

static UIState *ui_state_ptr = NULL;
static UIClass *class_ptr = NULL;
static UIElement *element_ptr = NULL;

static int strntol(char *string, unsigned int length){
	int result = 0;
	int decimal_place = 1;
	for(int i = 0; i < length; i++){
		if(string[length - 1 - i] >= '0' && string[length - 1 - i] <= '9'){
			result += (string[length - 1 - i] - '0') * decimal_place;
			decimal_place *= 10;
		}
	}
	if(string[0] == '-'){
		result *= -1;
	}

	return result;
}
static Vector3 HexToColor(char *hex){
	if(hex[0] == '#'){
		hex = hex + 1;
	}
	short v[3] = {0};
	char o1, o2;
	o1 = o2 = 0;
	for(int i = 0; i < 3; i++){
		if(hex[i * 2] <= '9'){
			o1 = '0';
		}else if(hex[i * 2] <= 'F'){
			o1 = 'A' - 10;
		}else if(hex[i * 2] <= 'f'){
			o1 = 'a' - 10;
		}
		if(hex[i * 2 + 1] <= '9'){
			o2 = '0';
		}else if(hex[i * 2 + 1] <= 'F'){
			o2 = 'A' - 10;
		}else if(hex[i * 2 + 1] <= 'f'){
			o2 = 'a' - 10;
		}
		v[i] = ((hex[i * 2] - o1) << 4) + (hex[i * 2 + 1] - o2);
	}
	return  (Vector3){v[0] / 255.0, v[1] / 255.0, v[2] / 255.0};
}

static char *class_attributes[] = {
	"color",
	"padding",
	"margin",
	"max-width",

	NULL
};
static void tfunc2_classes_attributes(JSONState *json, unsigned int token){
	// Make switch case here with every class attribute
	switch(JSONTokenHash(json, token, class_attributes)){
		case 0: // color
			char *color_str = NULL;
			JSONTokenToString(json, token + 1, &color_str);
			Vector3 color = HexToColor(color_str);
			free(color_str);
			color_str = NULL;
			class_ptr->color = color;
			break;
		case 1: // padding
			break;
		case 2: // margin
			int val = JSONTokenValue(json, token + 1)._int;
			class_ptr->margin.x = val;
			class_ptr->margin.y = val;
			class_ptr->margin.z = val;
			class_ptr->margin.w = val;
			break;
		case 3: // max-width
			// JSONTokenLength(json, token + 1) - 2
			class_ptr->size_max.x = strntol(json->json_string + json->tokens[token + 1].start, JSONTokenLength(json, token + 1)/*add a -2 or -1 here if adding support for 'px' or '%'*/);
			break;
		default: break;
	}
}

static void tfunc1_classes(JSONState *json, unsigned int token){
	JSONToken json_token = JSONTokenValue(json, token);
	if(json_token.type == JSON_STRING){
		class_ptr = UINewClass(ui_state_ptr);
		JSONTokenToString(json, token, &class_ptr->name);

		JSONSetTokenFunc(json, NULL, tfunc2_classes_attributes);
		JSONParse(json);
	}
}

static void tfunc1_new_elements(JSONState *json, unsigned int token);
static char *element_attributes[] = {
	"name",
	"class",
	"text",
	"children",

	NULL
};
static void tfunc2_element_data(JSONState *json, unsigned int token){
	switch(JSONTokenHash(json, token, element_attributes)){
		case 0: // name
			JSONTokenToString(json, token + 1, &element_ptr->name);

			break;
		case 1: // class
			char *name = NULL;
			JSONTokenToString(json, token + 1, &name);
			UIElementAddClass(element_ptr, UIFindClass(ui_state_ptr, name));
			free(name);
			name = NULL;

			break;
		case 2: // text
			JSONTokenToString(json, token + 1, &element_ptr->text);

			break;
		case 3: // children
			JSONSetTokenFunc(json, NULL, tfunc1_new_elements);
			JSONParse(json);

			break;
		default: break;
	}
}

static void tfunc1_new_elements(JSONState *json, unsigned int token){
	// JSONToken json_token = JSONTokenValue(json, token);
	
	
	UIElement *element = UINewElement(ui_state_ptr);

	UIElementAddChild(element_ptr, element);

	element_ptr = element;
	// JSONTokenToString(json, token, &element_ptr->name);
	JSONSetTokenFunc(json, NULL, tfunc2_element_data);
	JSONParse(json);

	element_ptr = element->parent;

}

static char *root_dict[] = {
	"classes",
	"elements",
	NULL
};
static void tfunc0_default(JSONState *json, unsigned int token){
	switch(JSONTokenHash(json, token, root_dict)){
		case 0: // classes
			JSONSetTokenFunc(json, NULL, tfunc1_classes);
			JSONParse(json);

			break;
		case 1: // elements
			JSONSetTokenFunc(json, NULL, tfunc1_new_elements);
			JSONParse(json);

			break;
		default: break;
	}
}
/* ---  ^ DEEPER NESTED AS WE GO UP ^  --- */
/* --- JSON ROOT - PARSING STARTS HERE --- */


UIState UIParse(char *path){
	UIState state = UINewState();
	ui_state_ptr = &state;

	if(path != NULL){
		element_ptr = UINewElement(&state);
		

		JSONState json = JSONOpen(path);
		JSONSetTokenFunc(&json, NULL, tfunc0_default);
		JSONParse(&json);
		JSONFree(&json);


	}else{
		DebugLog(D_WARN, "UI path string is null");
	}

	ui_state_ptr = NULL;
	element_ptr = NULL;
	class_ptr = NULL;

	return state;
}