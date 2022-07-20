#pragma once

#include <vector>

#include "../../Utils/DrawUtils.h"
#include "../Module/ModuleManager.h"
#include "../../Utils/Json.hpp"
#include "../../Utils/ColorUtil.h"

class HudEditor {
public:
	static void onMouseClickUpdate(int key, bool isDown);
	static void onKeyUpdate(int key, bool isDown);
	static void onLoadSettings(void* conf);
	static void onSaveSettings(void* conf);
	static void render();
	static void init();
};