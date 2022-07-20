#include "FastSwim.h"

FastSwim::FastSwim() : IModule(0, Category::MOVEMENT, "Swim Faster in water") {
	registerFloatSetting("Horizontal Speed", &speedh, speedh, 0.4f, 2.5f);
	registerFloatSetting("Vertical Speed", &speedv, speedv, 0.4f, 2.5f);
}

const char* FastSwim::getModuleName() {
	return "FastSwim";
}

void FastSwim::onLevelRender() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	bool pressed = MoveUtil::keyPressed();
	if (player->isInWater() || player->isInLava() && pressed) {
		float calcYaw = (player->yaw + 90) * (PI / 180);
		float calcPitch = (player->pitch) * -(PI / 180);
		player->velocity.x = cos(calcYaw) * speedh;
		player->velocity.y = sin(calcPitch) * speedv;
		player->velocity.z = sin(calcYaw) * speedh;
	}
}