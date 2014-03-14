#pragma once
#include "GlbCore.h"
#include "GlbTouch.h"
#include "city.h"
#include "menu_icon.h"

enum ENUM_LAYERS
{
    LAYER_GLOBE = 0,
    LAYER_CITY_ICON_START = 100,
	LAYER_LINES = 400,
    LAYER_SUB_MENU_START = 410,
	LAYER_CITY_DETAIL = 500,
	LAYER_CITY_DETAIL_CLOSE = 510,
	LAYER_CITY_DETAIL_RETURN = 520,
	LAYER_CITY_DETAIL_FORWARD = 530,
	LAYER_CITY_DETAIL_BACKWARD = 540,
	LAYER_MENU_START = 1000,
    LAYER_CIRCLE = 1100,
	LAYER_COPYRIGHT = 1200
};


bool txt2ImgHelper(int mode, char* myString);
bool txt2ImgHelper(char* imgFile, int mode, char* myString);
bool invokeValidatorHelper();
