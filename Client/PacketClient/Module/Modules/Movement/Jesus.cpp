#include "Jesus.h"

uintptr_t ViewBobbingJesusChristMan = FindSignature("0F B6 80 DB 01 00 00");

using namespace std;
Jesus::Jesus() : IModule(0, Category::MOVEMENT, "Allows you to walk on Liquids") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Solid", 0);
	mode.addEntry("Bounce", 1);
	registerFloatSetting("Height", &height, height, 0.1, 3);
}

const char* Jesus::getRawModuleName() {
	return "Jesus";
}

const char* Jesus::getModuleName() {
	name = string("Jesus ") + string(GRAY) + mode.GetEntry(mode.getSelectedValue()).GetName();
	return name.c_str();
}

void Jesus::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (player->isSneaking()) return;

	if (player->hasEnteredWater()) {
		player->velocity.y = height;
		player->onGround = true;
		wasInWater = true;
	}
	else if (player->isInWater() || player->isInLava()) {
		player->velocity.y = height;
		player->onGround = true;
		wasInWater = true;
	}
	else if (wasInWater) {
		wasInWater = false;
		player->velocity.x *= 1.2f;
		player->velocity.x *= 1.2f;
	}

	if (mode.selected == 1) return;
	{
		if (player->isSneaking()) return;
		if (player->isOnFire()) return;

		vec3_t pos = *player->getPos();
		pos.y -= 1.62f;

		pos.z = player->aabb.upper.z;
		pos.x = player->aabb.upper.x;

		Block* block = player->region->getBlock(vec3_ti(pos));
		BlockLegacy* blockLegacy = (block->blockLegacy);

		if (blockLegacy->material->isLiquid && player->velocity.y <= 0) {
			vec3_t pos = *player->getPos();
			pos.y -= 0.62f;
			pos.y = ceilf(pos.y);
			pos.y += 0.61f;
			player->setPos(pos);
			player->onGround = true;
			player->velocity.y = 0.f;
		}

		pos.x = player->aabb.lower.x;
		pos.z = player->aabb.lower.z;

		block = player->region->getBlock(vec3_ti(pos));
		blockLegacy = (block->blockLegacy);

		if (blockLegacy->material->isLiquid && player->velocity.y <= 0) {
			vec3_t pos = *player->getPos();
			pos.y -= 0.62f;
			pos.y = ceilf(pos.y);
			pos.y += 0.61f;
			player->setPos(pos);
			player->onGround = true;
			player->velocity.y = 0.f;
		}

		pos.x = player->aabb.upper.x;
		block = player->region->getBlock(vec3_ti(pos));
		blockLegacy = (block->blockLegacy);

		if (blockLegacy->material->isLiquid && player->velocity.y <= 0) {
			vec3_t pos = *player->getPos();
			pos.y -= 0.62f;
			pos.y = ceilf(pos.y);
			pos.y += 0.61f;
			player->setPos(pos);
			player->velocity.y = 0.f;
		}

		pos.x = player->aabb.lower.x;
		pos.z = player->aabb.upper.z;

		block = player->region->getBlock(vec3_ti(pos));
		blockLegacy = (block->blockLegacy);

		if (blockLegacy->material->isLiquid && player->velocity.y <= 0) {
			vec3_t pos = *player->getPos();
			pos.y -= 0.62f;
			pos.y = ceilf(pos.y);
			pos.y += 0.61f;
			player->setPos(pos);
			player->onGround = true;
			player->velocity.y = 0.f;
		}

		if (player->isInWater()) {
			player->velocity.y = 0.1f;
			player->onGround = true;
		}
	}
}
void Jesus::onMove(MoveInputHandler* input) {
	if (viewBobbingEffective) Utils::patchBytes((unsigned char*)ViewBobbingJesusChristMan, (unsigned char*)"\xB8\x01\x00\x00\x00\x90\x90", 7);
	else Utils::patchBytes((unsigned char*)ViewBobbingJesusChristMan, (unsigned char*)"\x0F\xB6\x80\xDB\x01\x00\x00", 7);
}
void Jesus::onDisable() {
	Utils::patchBytes((unsigned char*)ViewBobbingJesusChristMan, (unsigned char*)"\x0F\xB6\x80\xDB\x01\x00\x00", 7);
	viewBobbingEffective = false;
	velocityEffective = false;
}