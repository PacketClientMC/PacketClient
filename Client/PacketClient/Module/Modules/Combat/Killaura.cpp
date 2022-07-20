#include "Killaura.h"

using namespace std;
Killaura::Killaura() : IModule(0, Category::COMBAT, "Attacks entities") {
	registerEnumSetting("Rotations", &rotations, 0);
	rotations.addEntry("Normal", 0);
	rotations.addEntry("Smooth", 1);
	rotations.addEntry("Silent", 2);
	rotations.addEntry("None", 3);
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Multi", 0);
	mode.addEntry("Switch", 1);
	mode.addEntry("Single", 2);
	registerBoolSetting("Visualize", &visualize, visualize);
	registerBoolSetting("Strafe", &strafe, strafe);
	registerBoolSetting("Click", &click, click);
	registerBoolSetting("Hold", &hold, hold);
	registerFloatSetting("Smoothing", &smoothing, smoothing, 1.f, 80.f);
	registerFloatSetting("Random", &random, random, 0.f, 15.f);
	registerFloatSetting("Range", &range, range, 3.f, 8.f);
	//registerMinMaxSetting("APS", &maxAPS, maxAPS, 1, 20);
	registerIntSetting("MaxAPS", &maxAPS, maxAPS, 1, 20);
	registerIntSetting("MinAPS", &minAPS, minAPS, 1, 20);
}

const char* Killaura::getRawModuleName() {
	return "Killaura";
}

const char* Killaura::getModuleName() { 
	name = string("Killaura ") + string(GRAY) + mode.GetEntry(mode.getSelectedValue()).GetName();
	return name.c_str();
}

static vector<Entity*> targetList;
void findEntity(Entity* currentEntity, bool isRegularEntity) {
	static auto killaura = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() == 80 || currentEntity->getEntityTypeId() == 69) return;

	if (killaura->mobs) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63) return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) return;
		if (currentEntity->getEntityTypeId() == 64) return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity)) return;
		if (currentEntity->getEntityTypeId() == 51 || currentEntity->getEntityTypeId() == 1677999) return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < killaura->range) targetList.push_back(currentEntity);
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Killaura::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListEmpty = true;
	targetList.clear();
}

void Killaura::onTick(GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	Level* pointing = g_Data.getLocalPlayer()->level;
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (clickGUI->isEnabled()) targetListEmpty = true;
	targetListEmpty = targetList.empty();

	targetList.clear();
	g_Data.forEachEntity(findEntity);

	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		if (TimeUtil::hasTimedElapsed((1000 / APS), true)) {
			if (hold && !g_Data.isLeftClickDown())
				return;

			for (auto& i : targetList) {
				player->swing();
				if (mode.getSelectedValue() == 0) gm->attack(i);
				else gm->attack(targetList[0]);
			}

			if (!MoveUtil::isMoving()) {
				MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
			}
		}

		for (auto& i : targetList) {
			delay++;
			if (click && !targetList.empty()) {
				if (delay >= 0) {
					g_Data.leftclickCount++;
					if (pointing->hasEntity()) gm->attack(pointing->getEntity());
				}
			}
		}
	}
	else targetListEmpty = true;
}

void Killaura::onPlayerTick(Player* plr) {
	if (plr == nullptr) return;

	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		APS = randomFloat(maxAPS, minAPS);
		if (maxAPS < minAPS) maxAPS = minAPS;
		if (hold && !g_Data.isLeftClickDown())
			return;

		xRandom = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / random));
		yRandom = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / random));
		if (random >= 0.f) { yRandom += 1.75f; xRandom -= 3.5f; }

		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		for (auto& i : targetList) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			vec2_t pos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*i->getPos());

			if (animYaw > angle.y) animYaw -= ((animYaw - angle.y) / smoothing);
			else if (animYaw < angle.y) animYaw += ((angle.y - animYaw) / smoothing);

			switch (rotations.getSelectedValue()) {
			case 0: // Normal
				plr->yawUnused1 = animYaw + xRandom;
				plr->pitch = angle.x + yRandom;
				plr->bodyYaw = animYaw + xRandom;
				break;
			case 1: // Smooth
				plr->yawUnused1 = animYaw + xRandom;
				plr->pitch = angle.x + yRandom;
				break;
			}

			// Strafe
			if (mode.getSelectedValue() <= 2 && strafe) plr->setRot(angle);
		}
	}
}

void Killaura::onSendPacket(Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (g_Data.canUseMoveKeys()) {
		if (hold && !g_Data.isLeftClickDown())
			return;

		sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		if (packet->isInstanceOf<MovePlayerPacket>() && rotations.getSelectedValue() != 3 && !targetList.empty()) {
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList[0]->eyePos0);
			auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet);

			movePacket->pitch = angle.x + yRandom;
			movePacket->headYaw = animYaw + xRandom;
			movePacket->yaw = animYaw + xRandom;
		}
	}
}

void Killaura::onLevelRender() {
	auto targetStrafe = moduleMgr->getModule<TargetStrafe>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || targetStrafe->isEnabled() || !visualize) return;

	static float n = 0;
	static float anim = 0;
	if (g_Data.canUseMoveKeys() && !targetList.empty()) {
		if (hold && !g_Data.isLeftClickDown())
			return;
		if (mode.getSelectedValue() == 2) sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		if (!targetList.empty()) {
			anim++;
			DrawUtils::setMC_Color(1, 1, 1, 0.9f);

			vec3_t permutations[56];
			for (int i = 0; i < 56; i++) {
				permutations[i] = { sinf((i * 10.f) / (180 / PI)), 0.f, cosf((i * 10.f) / (180 / PI)) };
			}
			vec3_t permutations2[56];
				n++;
			if (n == 360)
				n = 0;
			for (int i = 0; i < 56; i++) {
				permutations2[i] = { sinf((i * 10.f) / (180 / PI)), sinf((i * 10.f) / (180 / PI)) ,cosf((i * 10.f) / (180 / PI)) };
			}

			const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI * 1);

			vec3_t* start = targetList[0]->getPosPrev();
			vec3_t* end = targetList[0]->getPos();

			auto te = DrawUtils::getLerpTime();
			vec3_t pos = start->lerp(end, te);

			auto yPos = pos.y;
			yPos -= 1.6f;
			yPos += coolAnim;

			vector<vec3_t> posList;
			posList.reserve(56);
			for (auto& perm : permutations) {
				vec3_t curPos(pos.x, yPos, pos.z);
				posList.push_back(curPos.add(perm));
			}
			DrawUtils::drawLineStrip3D(posList, 5);
			
			//Draw Sims Esp
			/*vector<vec3_t> posList2;
			posList2.clear();
			// Iterate through phi, theta then convert r,theta,phi to XYZ
			for (double phi = 0.; phi < 2 * PI; phi += PI / 100.) // Azimuth [0, 2PI]
			{
				for (double theta = 0.; theta < PI; theta += PI / 100.) // Elevation [0, PI]
				{
					vec3_t point;
					point.x = 0.2 * cos(phi) * sin(theta);
					point.y = 0.2 * sin(phi) * sin(theta);
					point.z = 0.2 * cos(theta);
					vec3_t curPos = targetList[0]->eyePos0;
					curPos.y += 0.5f;
					posList2.push_back(curPos.add(point));
				}
			}
			DrawUtils::setMC_Color(0.1f,0.9f,0.1f,255);
			DrawUtils::drawLineStrip3D(posList2, 4);*/
		}
	}
}

void Killaura::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	targetListEmpty = true;
	targetList.clear();
}