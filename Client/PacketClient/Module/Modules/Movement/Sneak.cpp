#include "Sneak.h"

Sneak::Sneak() : IModule(0, Category::MOVEMENT, "Automatically sneaks") {
}

const char* Sneak::getModuleName() {
	return ("Sneak");
}

void Sneak::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = true;
}

void Sneak::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	g_Data.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
}