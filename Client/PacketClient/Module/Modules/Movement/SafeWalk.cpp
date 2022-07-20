#include "SafeWalk.h"

SafeWalk::SafeWalk() : IModule(0, Category::MOVEMENT, "Prevents you from walking off blocks") {
}

const char* SafeWalk::getModuleName() {
	return "SafeWalk";
}