#include "Criticals.h"
#include "../../ModuleManager.h"

using namespace std;
Criticals::Criticals() : IModule(0, Category::COMBAT, "Makes every hit a critical") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Position", 1);
	mode.addEntry("Jump", 2);
	registerBoolSetting("Hurttime", &hurttime, hurttime);
}

const char* Criticals::getRawModuleName() {
	return "Criticals";
}

const char* Criticals::getModuleName() {
	if (mode.getSelectedValue() == 0)  name = string("Criticals ") + string(GRAY) + string("Vanilla");
	if (mode.getSelectedValue() == 1)  name = string("Criticals ") + string(GRAY) + string("Position");
	if (mode.getSelectedValue() == 2)  name = string("Criticals ") + string(GRAY) + string("Jump");
	if (mode.getSelectedValue() == 3)  name = string("Criticals ") + string(GRAY) + string("Hive");
	return name.c_str();
}

static vector<Entity*> targetList;
void findEntity_Criticals(Entity* currentEntity, bool isRegularEntity) {
	static auto criticals = moduleMgr->getModule<Criticals>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (currentEntity->getEntityTypeId() == 80)  // Arrows
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	if (currentEntity->getEntityTypeId() == 1677999)  // Villager
		return;

	if (currentEntity->getEntityTypeId() == 51)  // NPC
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 8)
		targetList.push_back(currentEntity);
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Criticals::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetList.clear();
	g_Data.forEachEntity(findEntity_Criticals);
	tick++;

	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		Level* pointing = g_Data.getLocalPlayer()->level;
		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		for (auto& i : targetList) {
			if (hurttime && targetList[0]->damageTime < 1)
				return;

			// Jump
			if (mode.getSelectedValue() == 2 && player->onGround) player->jumpFromGround();
		}
	}
}

void Criticals::onSendPacket(Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (packet->isInstanceOf<MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr) {
		MovePlayerPacket* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);
		if (g_Data.canUseMoveKeys() && !targetList.empty()) {
			for (auto& i : targetList) {
				if (hurttime && targetList[0]->damageTime < 1)
					return;

				// Vanilla
				if (mode.getSelectedValue() == 0) movePacket->onGround = false;

				// Position
				if (mode.getSelectedValue() == 1) {
				}
			}
		}
	}
}
