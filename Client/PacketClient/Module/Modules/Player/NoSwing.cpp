#include "NoSwing.h"

NoSwing::NoSwing() : IModule(0, Category::PLAYER, "Disable the swing animation") {
}

const char* NoSwing::getModuleName() {
	return ("NoSwing");
}