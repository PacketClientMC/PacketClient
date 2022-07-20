#include "Hooks.h"

#include <algorithm>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include "../SDK/Tag.h"
#include "../include/image/stb_image.h"



Hooks g_Hooks;
bool overrideStyledReturn = false;
TextHolder styledReturnText;

bool isHUDHidden = false;
bool isTicked = false;

void blockRotate(glm::mat4& matrix, float upper) {
	float floatY = -1.30F;
	matrix = glm::translate<float>(matrix, glm::vec3(-0.24F, upper, -0.20F));
	matrix = glm::rotate<float>(matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, -floatY, glm::vec3(4.0F, 0.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void slideBlockRotate(glm::mat4& matrix, float upper) {
	matrix = glm::translate<float>(matrix, glm::vec3(-0.5F, upper, 0.0F));
	matrix = glm::rotate<float>(matrix, 30.0F, glm::vec3(0.0F, 1.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, -80.0F, glm::vec3(1.0F, 0.0F, 0.0F));
	matrix = glm::rotate<float>(matrix, 60.0F, glm::vec3(0.0F, 1.0F, 0.0F));
}

void Hooks::Init() {
	logF("Setting up Hooks...");

	// VTables
	{
		// LocalPlayer::vtable
		{
			if (g_Data.getLocalPlayer() != nullptr) {
				uintptr_t** localPlayerVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getLocalPlayer());
				if (localPlayerVtable == 0x0) logF("g_Data.getLocalPlayer() not working!!!");
				else {
					g_Hooks.rotationHook = std::make_unique<FuncHook>(localPlayerVtable[27], Hooks::rotation);
					g_Hooks.swingHook = std::make_unique<FuncHook>(localPlayerVtable[217], Hooks::swing);
					g_Hooks.setPosHook = std::make_unique<FuncHook>(localPlayerVtable[19], Hooks::setPos);
					g_Hooks.baseTickHook = std::make_unique<FuncHook>(localPlayerVtable[50], Hooks::baseTick);
					g_Hooks.isImmobileHook = std::make_unique<FuncHook>(localPlayerVtable[91], Hooks::isImmobile);
					g_Hooks.tickWorldHook = std::make_unique<FuncHook>(localPlayerVtable[367], Hooks::tickWorld);
				}
			}
		}

		// GameMode::vtable
		{
			if (g_Data.getGameMode() != nullptr) {
				uintptr_t** gmVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getGameMode());
				if (gmVtable == 0x0) logF("Gamemode not working!!!");
				else {
					g_Hooks.startDestroyBlockHook = std::make_unique<FuncHook>(gmVtable[1], Hooks::startDestroyBlock);
					g_Hooks.getPickRangeHook = std::make_unique<FuncHook>(gmVtable[10], Hooks::getPickRange);
					g_Hooks.attackHook = std::make_unique<FuncHook>(gmVtable[14], Hooks::attack);
				}
			}
		}

		// MoveInputHandler::vtable
		{
			if (g_Data.getClientInstance() != nullptr && g_Data.getClientInstance()->getMoveTurnInput() != nullptr) {
				uintptr_t** moveInputVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getClientInstance()->getMoveTurnInput());
				if (moveInputVtable == 0x0) logF("g_Data.getClientInstance()->getMoveTurnInput() not working!!!");
				else {
					g_Hooks.moveInputHandler_tickHook = std::make_unique<FuncHook>(moveInputVtable[1], Hooks::moveInputHandler_tick);
				}
			}
		}

		// PackAccessStrategy vtables for isTrusted
		{
			uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
			int offset = *reinterpret_cast<int*>(sigOffset + 3);
			uintptr_t** directoryPackVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);

			{ g_Hooks.directoryPackAccessStrategy_isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable[6], Hooks::directoryPackAccessStrategy_isTrusted); }

			uintptr_t sigOffset2 = FindSignature("48 8D 05 ?? ?? ?? ?? 48 89 01 4C 8D B1 ?? ?? ?? ?? 49 8B 46 08"); // Was 48 8D 05 ?? ?? ?? ?? 48 89 03 49 8D 57 in 1.18.2
			int offset2 = *reinterpret_cast<int*>(sigOffset2 + 3);
			uintptr_t** directoryPackVtable2 = reinterpret_cast<uintptr_t**>(sigOffset2 + offset2 + 7);

			{ g_Hooks.zipPackAccessStrategy_isTrustedHook = std::make_unique<FuncHook>(directoryPackVtable2[6], Hooks::returnTrue); }
			g_Hooks.skinRepository_checkSignatureFileInPack = std::make_unique<FuncHook>(FindSignature("48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B 79"), Hooks::returnTrue);
		}
	}

	// Signatures
	{
		void* _sendChatMessage = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ?? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 4C 8B EA 4C 8B F9 48 8B 49"));
		g_Hooks.sendChatMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::sendChatMessage);

		void* _renderCtx = reinterpret_cast<void*>(FindSignature("48 8B ? 48 89 ? ? 55 56 57 41 ? 41 ? 41 ? 41 ? 48 8D ? ? ? ? ? 48 81 EC ? ? ? ? 0F 29 ? ? 0F 29 ? ? 48 8B ? ? ? ? ? 48 33 ? 48 89 ? ? ? ? ? 4C 8B ? 48 89 ? ? ? 4C 8B"));
		g_Hooks.renderTextHook = std::make_unique<FuncHook>(_renderCtx, Hooks::renderText);
		g_Hooks.renderTextHook->enableHook();

		void* setupRender = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 33 D2 41 B8"));
		g_Hooks.setupAndRenderHook = std::make_unique<FuncHook>(setupRender, Hooks::setupAndRender);

		void* render = reinterpret_cast<void*>(FindSignature("48 89 5C 24 18 57 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B DA 48 8B F9 B9 10 ? ? ?"));
		g_Hooks.renderHook = std::make_unique<FuncHook>(render, Hooks::render);

		void* fogColorHook = reinterpret_cast<void*>(FindSignature("41 0F 10 08 48 8B C2 0F"));
		g_Hooks.getFogColorHook = std::make_unique<FuncHook>(fogColorHook, Hooks::getFogColor);

		void* timeOfDay = reinterpret_cast<void*>(FindSignature("44 8B C2 B8 ? ? ? ? F7 EA"));
		g_Hooks.getTimeOfDayHook = std::make_unique<FuncHook>(timeOfDay, Hooks::getTimeOfDay);

        void* fullbright = reinterpret_cast<void*>(FindSignature("48 83 EC ? 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 74 ? 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 74 ? 48 8B 42 ? 48 8B 88 ? ? ? ? 48 85 C9 74 ? E8 ? ? ? ? 48 83 C4 ? C3 F3 0F 10 42 ? 48 83 C4 ? C3 41 B8 ? ? ? ? FF 50 ? 48 8B 10 48 85 D2 75 ? E8 ? ? ? ? CC E8 ? ? ? ? CC CC CC CC CC CC CC CC CC CC 40 53 48 83 EC ? 48 8B 01"));
		g_Hooks.getGammaHook = std::make_unique<FuncHook>(fullbright, Hooks::getGamma);

        void* chatLogHook = reinterpret_cast<void*>(FindSignature("40 55 57 41 56 48 83 EC ? 48 8B 81")); //40 55 57 41 56 48 83 EC ? 48 8B 81
		g_Hooks.chatLogHook = std::make_unique<FuncHook>(chatLogHook, Hooks::chatLog);

		void* keyMouseFunc = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8B EC 48 81 EC ? ? ? ? 0F 29 74 24 ? 0F 29 7C 24 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 49 8B F0"));
		g_Hooks.HIDController_keyMouseHook = std::make_unique<FuncHook>(keyMouseFunc, Hooks::HIDController_keyMouse);
		// dead
		void* chestTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 56 57 41 56 48 83 EC ? 48 83 79"));
		g_Hooks.chestBlockActor_tickHook = std::make_unique<FuncHook>(chestTick, Hooks::chestBlockActor_tick);

		void* getRenderLayer = reinterpret_cast<void*>(FindSignature("8B 81 ? ? ? ? C3 CC CC CC CC CC CC CC CC CC F3 0F 10 81"));
		g_Hooks.getRenderLayerHook = std::make_unique<FuncHook>(getRenderLayer, Hooks::getRenderLayer);

		void* getLightEmission = reinterpret_cast<void*>(FindSignature("0F B6 81 ? ? ? ? 88 02 48 8B C2"));
		g_Hooks.getLightEmissionHook = std::make_unique<FuncHook>(getLightEmission, Hooks::getLightEmission);

		void* autoComplete = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 41 8B D9"));
		g_Hooks.autoCompleteHook = std::make_unique<FuncHook>(autoComplete, Hooks::autoComplete);

		uintptr_t** packetSenderVtable = reinterpret_cast<uintptr_t**>(*(uintptr_t*)g_Data.getClientInstance()->loopbackPacketSender);
		g_Hooks.loopbackPacketSender_sendToServerHook = std::make_unique<FuncHook>(packetSenderVtable[2], Hooks::loopbackPacketSender_sendToServer);

		void* tick_entityList = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F9 0F 57 C0 0F 11 44 24 ? F3 0F 7F 44 24 ? 48 8B 05 ? ? ? ? 48 85 C0 74 ? F0 FF 40 ? 0F 10 05 ? ? ? ? 0F 11 44 24 ? 48 8D 54 24 ? 48 8D 4C 24 ? E8 ? ? ? ? 90 48 8B CF"));
		g_Hooks.multiLevelPlayer_tickHook = std::make_unique<FuncHook>(tick_entityList, Hooks::multiLevelPlayer_tick);

		void* renderLevel = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 49 8B F8 48 8B DA 48 8B F1 0F 57 C0"));
		g_Hooks.renderLevelHook = std::make_unique<FuncHook>(renderLevel, Hooks::renderLevel);

		void* cameraRotation = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B F9 48 8B F2 48 8B 49"));
		g_Hooks.cameraRotationHook = std::make_unique<FuncHook>(cameraRotation, Hooks::cameraRotation);

		void* playerCallBackHook = reinterpret_cast<void*>(FindSignature("F3 0F 10 81 ? ? ? ? 41 0f 2F 00"));
		g_Hooks.playerCallBack_Hook = std::make_unique<FuncHook>(playerCallBackHook, Hooks::playerCallBack);
		
		void* clickHook = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 57 41 54 41 55 41 56 41 57 48 83 EC ? 44 0F B7 BC 24 ? ? ? ? 48 8B D9"));
		g_Hooks.clickFuncHook = std::make_unique<FuncHook>(clickHook, Hooks::clickFunc);

		void* chestScreenControllerTick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B F9 E8 ? ? ? ? 48 8B 17"));
		g_Hooks.chestScreenController_tickHook = std::make_unique<FuncHook>(chestScreenControllerTick, Hooks::chestScreenController_tick);

		void* RakNetInstance__tick = reinterpret_cast<void*>(FindSignature("48 89 5C 24 10 48 89 74 24 18 55 57 41 54 41 56 41 57 48 8D AC 24 20 FD FF FF 48 81 EC E0 03 00 00"));
		g_Hooks.rakNetInstance_tickHook = std::make_unique<FuncHook>(RakNetInstance__tick, Hooks::rakNetInstance_tick);

		void* ConnectionRequest__create = reinterpret_cast<void*>(FindSignature("40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 8B ?? ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 49 8B D9 48 89 55 E8"));
		g_Hooks.connectionRequestCreateHook = std::make_unique<FuncHook>(ConnectionRequest__create, Hooks::connectionRequestCreate);

		void* _getSkinPack = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B EA 48 8B F1"));
		g_Hooks.skinRepository_loadSkinPackHook = std::make_unique<FuncHook>(_getSkinPack, Hooks::skinRepository_loadSkinPack);

		void* lerpFunc = reinterpret_cast<void*>(FindSignature("8B 02 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? 8B 42 ? 89 81 ? ? ? ? C3 CC CC CC CC CC 48 89 5C 24"));
		g_Hooks.lerpMotionHook = std::make_unique<FuncHook>(lerpFunc, Hooks::lerpMotion);

		void* forceThirdPerson = reinterpret_cast<void*>(Utils::getBase() + 0xCA3D40);
		g_Hooks.forceThirdPersonHook = std::make_unique<FuncHook>(forceThirdPerson, Hooks::forceThirdPerson);

		void* destroySpeed = reinterpret_cast<void*>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B FA 0F 29 74 24 ? 48 8B 91"));
		g_Hooks.getDestroySpeedHook = std::make_unique<FuncHook>(destroySpeed, Hooks::getDestroySpeed);

		static constexpr auto counterStart = __COUNTER__ + 1;
#define lambda_counter (__COUNTER__ - counterStart)

		void* levelRendererBobView = reinterpret_cast<void*>(FindSignature("48 8B C4 48 89 58 20 57 48 ?? ?? ?? ?? 00 00 0F ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ?? ?? 0F ?? ?? ??"));

		static auto bobViewHookF = [](__int64 _this, glm::mat4& matrix, float lerpT) {
			static auto origFunc = g_Hooks.lambdaHooks.at(lambda_counter)->GetFastcall<void, __int64, glm::mat4&, float>();
			static auto animations = moduleMgr->getModule<Animations>();
			static auto killaura = moduleMgr->getModule<Killaura>();
			auto p = g_Data.getLocalPlayer();
			float degrees = fmodf(p->getPosPrev()->lerp(p->getPos(), lerpT).x, 5) - 2.5f;
			auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();
			degrees *= 180 / 2.5f;

			glm::mat4 View = matrix;
			matrix = View;

			// Blocking Animation
			auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
			if (animations->isEnabled()) {
				// Custom Settings
				if (animations->translate)
					matrix = glm::translate<float>(matrix, glm::vec3(animations->xPos, animations->yPos, animations->zPos)); // X Y Z

				if (animations->scale)
					matrix = glm::scale<float>(matrix, glm::vec3(animations->xScale, animations->yScale, animations->zScale)); // SCALE

				if (animations->rotate)
					matrix = glm::rotate<float>(matrix, degrees, glm::vec3(animations->xRotate, animations->yRotate, animations->zRotate)); // idk unused

				if (animations->shouldBlock && g_Data.canUseMoveKeys() && g_Data.isInGame() && !clickGUI->isEnabled()) {
					// 1.7
					if (animations->mode.getSelectedValue() == 1) {
						lerpT = 0.f;
						matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
						matrix = glm::translate<float>(matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
						blockRotate(matrix, 0.25f);
					}

					// Slide
					if (animations->mode.getSelectedValue() == 2) {
						matrix = glm::translate<float>(matrix, glm::vec3(0.42222223281, 0.0, -0.16666666269302368));
						matrix = glm::translate<float>(matrix, glm::vec3(-0.15f, 0.15f, -0.2f));
						slideBlockRotate(matrix, 0.2f);
					}

					// Old
					if (animations->mode.getSelectedValue() == 3) {
						matrix = glm::translate<float>(matrix, glm::vec3(5.54, 0.85, -2.00));
						matrix = glm::scale<float>(matrix, glm::vec3(2, 2, 2));
						float degrees = 13;
						degrees *= 180 / 4;

						auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();

						glm::mat4 View = matrix;

						matrix = View;
						matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(2.58, -4.40, -3.50));
						matrix = glm::translate<float>(matrix, glm::vec3(1.08, -0.02, -0.02));
						matrix = glm::scale<float>(matrix, glm::vec3(4, 4, 4));
						matrix = glm::translate<float>(matrix, glm::vec3(0.5, 0.4, 0.4));
					}

					// Spin
					if (animations->mode.getSelectedValue() == 4) {
						auto player = g_Data.getLocalPlayer();
						float degrees = fmodf(player->getPosPrev()->lerp(player->getPos(), lerpT).x, 5) - 2.5f;
						degrees *= 180 / 2.5f;

						auto pos = g_Data.getClientInstance()->levelRenderer->getOrigin();

						glm::mat4 View = matrix;

						matrix = View;
						matrix = glm::rotate<float>(matrix, glm::radians<float>(degrees), glm::vec3(0, 0, 1));
					}

				}
			}
			return origFunc(_this, matrix, lerpT);
		};

		std::shared_ptr<FuncHook> bobViewHook = std::make_shared<FuncHook>(levelRendererBobView, (decltype(&bobViewHookF.operator()))bobViewHookF);

		g_Hooks.lambdaHooks.push_back(bobViewHook);

#undef lambda_counter

		logF("Hooks initialized");
	}
}

void Hooks::Restore() {
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
}

void Hooks::Enable() {
	logF("Hooks enabled");
	MH_EnableHook(MH_ALL_HOOKS);
}

#pragma region Tick Hooks

void Hooks::multiLevelPlayer_tick(EntityList* _this) {
	static auto oTick = g_Hooks.multiLevelPlayer_tickHook->GetFastcall<void, EntityList*>();
	GameMode* gm = g_Data.getGameMode();
	if (gm != nullptr) moduleMgr->onTick(gm);
	oTick(_this);
	GameData::EntityList_tick(_this);
}

void* Hooks::tickWorld(Player* _this, __int64 unk) {
	static auto oTick = g_Hooks.tickWorldHook->GetFastcall<void*, Player*, __int64>();
	auto o = oTick(_this, unk);

	if (_this == g_Data.getLocalPlayer()) {
		GameMode* gm = *reinterpret_cast<GameMode**>(reinterpret_cast<__int64>(_this) + 0x1EF4);
		//GameData::updateGameData(gm);
		moduleMgr->onWorldTick(gm);
	}
	return o;
}

float Hooks::playerCallBack(Player* lp, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.playerCallBack_Hook->GetFastcall<bool, Player*, __int64, __int64>();
	if (lp == g_Data.getLocalPlayer()) moduleMgr->onPlayerTick(lp);
	if (g_Data.getLocalPlayer() != nullptr && lp == g_Data.getLocalPlayer()) {
		if (!g_Data.getLocalPlayer() || !g_Data.getLocalPlayer()->level || !*(&g_Data.getLocalPlayer()->region + 1))
			g_Hooks.entityList.clear();

		std::vector<EntityListPointerHolder> validEntities;
		for (const auto& ent : g_Hooks.entityList) {
			auto entity = ent.ent;
			MEMORY_BASIC_INFORMATION info;
			VirtualQuery(ent.ent, &info, sizeof(MEMORY_BASIC_INFORMATION));
			if (info.State & MEM_FREE) continue;
			if (info.State & MEM_RESERVE) continue;

			if (entity != nullptr && (__int64)entity != 0xFFFFFFFFFFFFFCD7 && ent.ent != nullptr && entity->isAlive() && *(__int64*)ent.ent != 0xFFFFFFFFFFFFFCD7 && *(__int64*)ent.ent > 0x6FF000000000 && *(__int64*)ent.ent < 0x800000000000 && *((int64_t*)ent.ent + 1) < 0x6FF000000000 && *(__int64*)ent.ent <= Utils::getBase() + 0x10000000)
				validEntities.push_back(ent);
		}
		g_Hooks.entityList.clear();
		g_Hooks.entityList = validEntities;
	}
	return oTick(lp, a2, a3);
}

void Hooks::baseTick(Entity* ent) {
	static auto oFunc = g_Hooks.baseTickHook->GetFastcall<void, Entity*>();
	LocalPlayer* player = g_Data.getLocalPlayer();
	if (!player || !player->getPointingStruct()) return oFunc(ent);

	static int tickCountThen = 0;
	int tickCountNow = *(int*)((__int64)player->getPointingStruct() + 0x708);

	if (tickCountNow != tickCountThen) {
		g_Hooks.entityList.clear();
		tickCountThen = tickCountNow;
	}

	if (ent->isClientSide()) {
		EntityListPointerHolder e;
		e.addedTick = tickCountNow;
		e.ent = ent;

		bool found = false;
		for (const auto& entity : g_Hooks.entityList) {
			if (entity.ent == ent && entity.addedTick == tickCountNow) {
				found = true;
				break;
			}
		}

		if (!found) g_Hooks.entityList.push_back(e);
	}
	return oFunc(ent);
}

void prepServers() {
	if (g_Data.getClientInstance() && g_Data.getClientInstance()->minecraftGame->getServerEntries() && *reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50)) {
		auto serverEntries = g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x48;

		struct ThirdPartyServer {
			TextHolder serverName;
			TextHolder uuid;
			TextHolder masterPlayerAccount;
			TextHolder serverName2;
			TextHolder lobbyDescription;
			TextHolder domain;
			TextHolder pathToServerIcon;
			TextHolder serverAddress;
			int coolBoye;

			ThirdPartyServer() {
				memset(this, 0, sizeof(ThirdPartyServer));
			}
		};

		struct BeansEntry {
			BeansEntry* nextEntry;
			BeansEntry* prevEntry;
			TextHolder masterPlayer;
			float unk;
			char filler[0x3c];
			TextHolder masterPlayer2;
			TextHolder serverName;
			char filler2[8];
			std::shared_ptr<ThirdPartyServer>* start;  // array
			std::shared_ptr<ThirdPartyServer>* end;    // end of array
		};

		auto listEnd = *reinterpret_cast<BeansEntry**>(serverEntries);

		auto current = listEnd;
		int count = 0;
		while (listEnd != current->nextEntry) {
			current = current->nextEntry;
			count++;
		}
		if (count > 5)  // we already added a server
			goto end;

		// make new one
		BeansEntry* epic = new BeansEntry();
		epic->nextEntry = listEnd;
		epic->prevEntry = current;
		epic->masterPlayer.setText("");
		epic->unk = current->unk;
		memcpy(epic->filler, current->filler, sizeof(BeansEntry::filler));
		epic->masterPlayer2.setText("");
		epic->serverName.setText("Epic");
		memcpy(epic->filler2, current->filler2, sizeof(BeansEntry::filler2));

		auto cT = current->start[0].get();

		std::shared_ptr<ThirdPartyServer>* start = new std::shared_ptr<ThirdPartyServer>[1];

		{
			ThirdPartyServer* t = new ThirdPartyServer();

			t->coolBoye = cT->coolBoye;
			t->uuid.setText("");
			t->masterPlayerAccount = cT->masterPlayerAccount;
			t->lobbyDescription = cT->lobbyDescription;
			t->pathToServerIcon.setText("");
			t->serverName.setText("Packet Server");
			t->serverName2.setText("Packet Server");  // This is the one actually displayed
			t->domain.setText(".packetnbeta.club");
			t->serverAddress.setText("mc.packetbeta.wtf");
			start[0] = std::shared_ptr<ThirdPartyServer>(t);
		}

		epic->start = start;
		epic->end = &start[1];

		current->nextEntry = epic;

		// increase count
		*reinterpret_cast<__int64*>(g_Data.getClientInstance()->minecraftGame->getServerEntries() + 0x50) += 1;
	end:;
	}
}

#pragma endregion

#pragma region Main Hooks

void Hooks::loopbackPacketSender_sendToServer(LoopbackPacketSender* a, Packet* packet) {
	static auto oFunc = g_Hooks.loopbackPacketSender_sendToServerHook->GetFastcall<void, LoopbackPacketSender*, Packet*>();

	static auto packetMultiplier = moduleMgr->getModule<PacketMultiplier>();
	static auto noPacket = moduleMgr->getModule<NoPacket>();
	static auto disabler = moduleMgr->getModule<Disabler>();
	static auto freecam = moduleMgr->getModule<Freecam>();
	static auto blinkMod = moduleMgr->getModule<Blink>();
	static auto freeTP = moduleMgr->getModule<FreeTP>();

	if (noPacket->isEnabled() && g_Data.isInGame())
		return;

	if (blinkMod->isEnabled() || freeTP->isEnabled() || (freecam->isEnabled() && freecam->cancelPackets)) {
		if (packet->isInstanceOf<MovePlayerPacket>() || packet->isInstanceOf<PlayerAuthInputPacket>()) {
			if (blinkMod->isEnabled()) {
				if (packet->isInstanceOf<MovePlayerPacket>()) {
					MovePlayerPacket* movePacket = reinterpret_cast<MovePlayerPacket*>(packet); movePacket->onGround = true;
					blinkMod->getMovePlayerPacketHolder()->push_back(new MovePlayerPacket(*movePacket));
				}
				else blinkMod->getPlayerAuthInputPacketHolder()->push_back(new PlayerAuthInputPacket(*reinterpret_cast<PlayerAuthInputPacket*>(packet)));
				return;
			}
		}
	}
	else if (!blinkMod->isEnabled()) {
		if (blinkMod->getMovePlayerPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getMovePlayerPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getMovePlayerPacketHolder()->clear();
			return;
		}
		if (blinkMod->getPlayerAuthInputPacketHolder()->size() > 0) {
			for (auto it : *blinkMod->getPlayerAuthInputPacketHolder()) {
				oFunc(a, (it));
				delete it;
				it = nullptr;
			}
			blinkMod->getPlayerAuthInputPacketHolder()->clear();
			return;
		}
	}

	if (disabler->isEnabled()) {
		if (disabler->mode.getSelectedValue() == 1 || disabler->mode.getSelectedValue() == 2) {
			if (packet->isInstanceOf<NetworkLatencyPacket>()) {
				return;
			}

			if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
				PlayerAuthInputPacket* authInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
				if (disabler->mode.getSelectedValue() == 2) {
					if (disabler->tick % (disabler->ticks * disabler->multiplier) == 0) {
						authInputPacket->pos.x = -0.911;
						authInputPacket->pos.z = -0.911;
					}
					if (disabler->tick % disabler->ticks != 0) {
						return;
					}
				}
			}
		}
	}

	moduleMgr->onSendPacket(packet);

	oFunc(a, packet);
}

__int64 Hooks::renderText(__int64 a1, MinecraftUIRenderContext* renderCtx) {
	static auto oText = g_Hooks.renderTextHook->GetFastcall<__int64, __int64, MinecraftUIRenderContext*>();
	GuiData* dat = g_Data.getClientInstance()->getGuiData();

	DrawUtils::setCtx(renderCtx, dat);

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos(); {
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

		mousePos = mousePos.div(windowSizeReal);
		mousePos = mousePos.mul(windowSize);
	}

	static bool leftClickDown;
	leftClickDown = g_Data.isLeftClickDown();
	std::string screenName(g_Hooks.currentScreenName);

	//DrawUtils::drawText(vec2_t(18, 18), &screenName, MC_Color(255, 255, 255), 1, 1);

	if (strcmp(screenName.c_str(), "nether_loading_progress_screen - nether") == 0) {
		static auto chestStealer = moduleMgr->getModule<ChestStealer>();
		static auto invManager = moduleMgr->getModule<InvManager>();
		static auto killaura = moduleMgr->getModule<Killaura>();
		static auto phase = moduleMgr->getModule<Phase>();

		if (chestStealer->isEnabled() && chestStealer->autoDisable) chestStealer->setEnabled(false);
		if (phase->isEnabled() && phase->mode.getSelectedValue() == 1) phase->worldChanged = true;
		if (invManager->isEnabled() && invManager->autoDisable) invManager->setEnabled(false);
		if (killaura->isEnabled() && killaura->autoDisable) killaura->setEnabled(false);
	}

	if (strcmp(screenName.c_str(), "inventory_screen") == 0 || strcmp(screenName.c_str(), "small_chest_screen") == 0 || strcmp(screenName.c_str(), "large_chest_screen") == 0 || strcmp(screenName.c_str(), "ender_chest_screen") == 0 || strcmp(screenName.c_str(), "shulker_box_screen") == 0) {
		static auto killauraMod = moduleMgr->getModule<Killaura>();
		static auto aidMod = moduleMgr->getModule<ChestStealer>();
		static auto invMod = moduleMgr->getModule<InvManager>();
		std::string len = "disable cheststealer  ";
		std::string killaura = "    Disable Killaura";
		std::string invManager = "  Disable InvManager";
		std::string chestStealer = " Disable ChestStealer";
		float buttonLen = DrawUtils::getTextWidth(&len, 1) + 10;
		vec4_t rectPos = vec4_t(6, 50, buttonLen, 62);
		vec2_t textPos = vec2_t(rectPos.x + 3, rectPos.y + 2);
		vec4_t rectPos2 = vec4_t(6, 30, buttonLen, 42);
		vec2_t textPos2 = vec2_t(rectPos2.x + 3, rectPos2.y + 2);
		vec4_t rectPos3 = vec4_t(6, 10, buttonLen, 22);
		vec2_t textPos3 = vec2_t(rectPos3.x + 2, rectPos3.y + 2);

		if (rectPos.contains(&mousePos) && leftClickDown) killauraMod->setEnabled(false);
		if (rectPos2.contains(&mousePos) && leftClickDown) invMod->setEnabled(false);
		if (rectPos3.contains(&mousePos) && leftClickDown) aidMod->setEnabled(false);
		if (rectPos.contains(&mousePos)) DrawUtils::fillRoundRectangle(rectPos, MC_Color(128, 128, 128, 80), false);
		else DrawUtils::fillRoundRectangle(rectPos, MC_Color(0, 0, 0, 80), false);
		if (rectPos2.contains(&mousePos)) DrawUtils::fillRoundRectangle(rectPos2, MC_Color(128, 128, 128, 80), false);
		else DrawUtils::fillRoundRectangle(rectPos2, MC_Color(0, 0, 0, 80), false);
		if (rectPos3.contains(&mousePos)) DrawUtils::fillRoundRectangle(rectPos3, MC_Color(128, 128, 128, 80), false);
		else DrawUtils::fillRoundRectangle(rectPos3, MC_Color(0, 0, 0, 80), false);

		//Pls TEst button stuffs 
		// ok i will once i dont crash
		//vec4_t rectPos1 = vec4_t(6, 50, buttonLen, 62);
		//if (HImGui.Button("Disable Killaura", vec2_t(rectPos1.x + 3, rectPos1.y + 2), true)) { if (killauraMod->isEnabled()) killauraMod->setEnabled(false); }

		DrawUtils::drawText(textPos, &killaura, MC_Color(255, 255, 255), 1, 1);
		DrawUtils::drawText(textPos2, &invManager, MC_Color(255, 255, 255), 1, 1);
		DrawUtils::drawText(textPos3, &chestStealer, MC_Color(255, 255, 255), 1, 1);
	}

	if (isHUDHidden || GameData::shouldHide() || !g_Hooks.shouldRender || !moduleMgr->isInitialized()) {
		return oText(a1, renderCtx);
	}

	static auto configManager = moduleMgr->getModule<ConfigManagerMod>();
	static auto notificationsMod = moduleMgr->getModule<Notifications>();
	static auto clickGUIMod = moduleMgr->getModule<ClickGUIMod>();

	// HudEditor stuff
	static auto interfaceMod = moduleMgr->getModule<Interface>();
	static auto playerList = moduleMgr->getModule<PlayerList>();
	static auto arraylist = moduleMgr->getModule<ArrayList>();
	static auto watermark = moduleMgr->getModule<Watermark>();

	HImGui.startFrame();

	g_Data.frameCount++;

	auto wid = g_Data.getClientInstance()->getGuiData()->windowSize;

	// Call PreRender() functions
	moduleMgr->onPreRender(renderCtx);
	DrawUtils::flush();

	__int64 retval = oText(a1, renderCtx);

	bool shouldPostRender = true;
	bool shouldRenderTabGui = true;

	{
		// Main Menu
		std::string screenName(g_Hooks.currentScreenName);
		if (strcmp(screenName.c_str(), "start_screen") == 0) {
			auto white = std::string(WHITE);
			auto gray = (GRAY);
			auto bold = std::string(BOLD);

			std::string add = gray + std::string("[+] ") + std::string(RESET);
			std::string remove = gray + std::string("[-] ") + std::string(RESET);
			std::string fix = gray + std::string("[*] ") + std::string(RESET);

			std::string changeLog = (gray + bold + std::string("Changelogs") + ": \n" + RESET +
				gray + bold + std::string("V2.1") + ": \n" + RESET +
				add + "Added Separation slider for colors in Interface \n" +
				fix + "Scaffold Extend now works on Hive mode \n" +
				add + "Added Smoothing slider to Killaura \n" +
				fix + "Fixed Nametags bug \n" +
				fix + "Fixed 30+ bugs \n" +
				"\n" +
				gray + bold + interfaceMod->versionStr + ": \n" + RESET +
				fix + "Fixed Sigma Toggle Sound \n" +
				fix + "Updated to 1.19! \n" +
				fix + "Fixed NoHurtCam \n" +
				add + "More (lol sike)!"
				);

			DrawUtils::drawText(vec2_t(5, 5), &changeLog, MC_Color(255, 255, 255), 1.f, 1, true);

		}
		else {
			shouldRenderTabGui = interfaceMod->tabGUI && interfaceMod->isEnabled();

			// ClickGUI
			if (clickGUIMod->isEnabled()) {
				if (clickGUIMod->hasOpenedGUI) {
					ClickGui::render();
					shouldPostRender = true;
					shouldRenderTabGui = false;
				}
				else HudEditor::render();
			}

			// ConfigManager
			if (configManager->isEnabled())
				ConfigManagerMenu::render();

			if (shouldRenderTabGui) TabGui::render();
		}
	}

	if (shouldPostRender) moduleMgr->onPostRender(renderCtx);
	HImGui.endFrame();
	DrawUtils::flush();

	// Draw Notifications
	NotificationManager notification;
	if (notificationsMod->isEnabled() && g_Hooks.shouldRender) {
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t pos;

		auto notifications = notification.getList();
		float yOffset = windowSize.y;

		if (arraylist->invert) yOffset = 60;
		if (arraylist->invert && !interfaceMod->release) yOffset = 30;
		if (!arraylist->invert && !interfaceMod->release) yOffset = windowSize.y - 30;

		for (auto& notification : notifications) {
			notification->fade();
			if (notification->fadeTarget == 1 && notification->duration <= 0 && notification->duration > -1) notification->fadeTarget = 0;
			else if (notification->duration > 0 && notification->fadeVal > 0.9f) notification->duration -= 1.f / 60;

			float textSize = 1.f;
			float textPadding = 1.f * textSize;
			float textHeight = 22.0f * textSize;

			std::string message = notification->message + " (" + std::to_string((int)notification->duration) + std::string(".") + std::to_string((int)(notification->duration * 10) - ((int)notification->duration * 10)) + ")";
			std::string title = std::string(BOLD) + notification->title + std::string(RESET);
			std::string textStr = " " + title + "\n" + message;

			float textWidth = DrawUtils::getTextWidth(&message, textSize);
			float xOffset = windowSize.x - textWidth;

			vec4_t rectPos = vec4_t(xOffset - 6.f, yOffset, windowSize.x + (textPadding * 2.f), yOffset + textPadding * 2.f + textHeight);
			vec2_t textPos = vec2_t(rectPos.x + 4.f, rectPos.y + 2.f);
			if (notification->duration > 1.f) {
				DrawUtils::fillRoundRectangle(rectPos, MC_Color(0, 0, 0, notificationsMod->opacity), true);
				DrawUtils::drawText(textPos, &textStr, MC_Color(255, 255, 255), textSize, 1.f, true);
			}

			if (notification->isOpen) {
				if (arraylist->invert) yOffset += 36; else yOffset -= 36;
			}
		}
	}
	DrawUtils::flush();

	return retval;
}

__int64 Hooks::render(UIScene* uiscene, __int64 screencontext) {
	static auto oRender = g_Hooks.renderHook->GetFastcall<__int64, UIScene*, __int64>();
	static auto chestStealer = moduleMgr->getModule<ChestStealer>();
	static auto clickGUIMod = moduleMgr->getModule<ClickGUIMod>();
	static auto invManager = moduleMgr->getModule<InvManager>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();

	g_Hooks.shouldRender = false;

	TextHolder alloc{};
	uiscene->getScreenName(&alloc);

	if (alloc.getTextLength() < 100) {
		strcpy_s(g_Hooks.currentScreenName, alloc.getText());
	}

	if (!g_Hooks.shouldRender) g_Hooks.shouldRender = (strcmp(alloc.getText(), "start_screen") == 0 || strcmp(alloc.getText(), "hud_screen") == 0);
	if (clickGUIMod->isEnabled() && strcmp(alloc.getText(), "pause_screen") == 0 || strcmp(alloc.getText(), "respawn_screen") == 0 || strcmp(alloc.getText(), "disconnect_screen") == 0) {
		clickGUIMod->setEnabled(false);
		g_Data.getClientInstance()->grabMouse();
	}
	alloc.alignedTextLength = 0;

	return oRender(uiscene, screencontext);
}

void Hooks::chatLog(__int64 a1, TextHolder* msg, uint32_t a2) {
	static auto oFunc = g_Hooks.chatLogHook->GetFastcall<void, __int64, TextHolder*, uint32_t>();
	auto message = *reinterpret_cast<TextHolder*>(reinterpret_cast<__int64>(msg) + 0x8);

	static auto deathEffects = moduleMgr->getModule<DeathEffects>();

	static auto sessionInfo = moduleMgr->getModule<SessionInfo>();
	static auto killsults = moduleMgr->getModule<Killsults>();
	static auto autoHive = moduleMgr->getModule<AutoHive>();
	static auto autoGG = moduleMgr->getModule<AutoGG>();
	static auto player = g_Data.getLocalPlayer();

	if (message.getText() != nullptr && player != nullptr) {
		std::string textStr = message.getText();

#ifdef _DEBUG
		if (moduleMgr->getModule<TestModule>()->isEnabled() && moduleMgr->getModule<TestModule>()->copyChatMessages) Utils::setClipboardText(std::string(message.getText()));
#endif

		std::string name = player->getNameTag()->getText();
		name = Utils::sanitize(name);

		// SessionInfo
		if (sessionInfo->isEnabled()) { // wtf bro LOL
			if (textStr.find("Game OVER!") != std::string::npos) sessionInfo->games += 1;
			if (textStr.find("You were killed by") != std::string::npos) sessionInfo->dead += 1;
			if (textStr.find(name) != std::string::npos && textStr.find("killed") != std::string::npos) sessionInfo->kills += 1;
			if (textStr.find(name) != std::string::npos && textStr.find("did an oopsie") != std::string::npos) sessionInfo->dead += 1;
			if (textStr.find(name) != std::string::npos && textStr.find("slain") != std::string::npos) sessionInfo->kills += 1;
		}

		// AutoGG
		if (autoGG->isEnabled()) {
			switch (autoGG->server.getSelectedValue()) {
			case 0: // Hive
				if (textStr.find("Game OVER!") != std::string::npos) autoGG->send = true;
				break;
			}
		}

		if (autoHive->isEnabled() && autoHive->autoBridgeWin) {
			if (textStr.find("scored") != std::string::npos) autoHive->doLerp = true;
			if (textStr.find("OVER!") != std::string::npos) {
				autoHive->setEnabled(false);
				moduleMgr->getModule<Blink>()->setEnabled(false);
				autoHive->doLerp = false;
			}
		}

		// KillInsults
		if (killsults->isEnabled()) {
			if (textStr.find(name) != std::string::npos && textStr.find("killed") != std::string::npos || textStr.find(name) != std::string::npos && textStr.find("slain") != std::string::npos)
				killsults->killed = true;
		}

		// DeathEffects
		if (deathEffects->isEnabled()) {
			if (textStr.find(name) != std::string::npos && textStr.find("killed") != std::string::npos || textStr.find(name) != std::string::npos && textStr.find("slain") != std::string::npos)
				deathEffects->killed = true;
		}
	}
	return oFunc(a1, msg, a2);
}

float Hooks::getDestroySpeed(Player* _this, Block& block) {
	static auto oFunc = g_Hooks.getDestroySpeedHook->GetFastcall<float, Player*, Block&>();
	static auto speedMine = moduleMgr->getModule<SpeedMine>();
	if (speedMine->isEnabled() && !speedMine->instant)
		return speedMine->speed;
	return oFunc(_this, block);
}

float Hooks::getTimeOfDay(__int64 _this, int a2, float a3) {
	static auto oGetTimeOfDay = g_Hooks.getTimeOfDayHook->GetFastcall<float, __int64, int, float>();

	static auto ambience = moduleMgr->getModule<Ambience>();
	if (ambience->isEnabled()) return ambience->time;

	return oGetTimeOfDay(_this, a2, a3);
}

void Hooks::cameraRotation(__int64 _this, void* a2) {
	static auto oFunc = g_Hooks.cameraRotationHook->GetFastcall<void, __int64, void*>();
	if (!moduleMgr->getModule<CameraMod>()->isEnabled()) return oFunc(_this, a2);
};

void Hooks::rotation(Entity* _this, vec2_t& angle) {
	static auto rotationHook = g_Hooks.rotationHook->GetFastcall<void, Entity*, vec2_t&>();
	static auto freelook = moduleMgr->getModule<Freelook>();

	if (freelook->isEnabled() && g_Data.getLocalPlayer() == _this) {
		angle = { freelook->oldPos.x, freelook->oldPos.y };
	}
	rotationHook(_this, angle);
}

int Hooks::forceThirdPerson(__int64 perspective) {
	static auto func = g_Hooks.forceThirdPersonHook->GetFastcall<int, __int64>();
	if (perspective == 1) g_Hooks.isThirdPerson = true;

	if (moduleMgr->getModule<Freecam>()->isEnabled()) return 1;
	else return func(perspective);
}

void Hooks::lerpMotion(Entity* _this, vec3_t motVec) {
	static auto oLerp = g_Hooks.lerpMotionHook->GetFastcall<void, Entity*, vec3_t>();
	static auto velocity = moduleMgr->getModule<Velocity>();

	if (g_Data.getLocalPlayer() != nullptr && g_Data.getLocalPlayer() != _this) return oLerp(_this, motVec);
	if (velocity->isEnabled()) {
		static void* networkSender = nullptr;
		if (!networkSender)
			networkSender = reinterpret_cast<void*>(9 + FindSignature("48 8B CB FF ?? ?? ?? ?? 00 C6 47 ?? 01 48 8B 5C 24"));

		if (networkSender == _ReturnAddress()) motVec = _this->velocity.lerp(motVec, velocity->x, velocity->y, velocity->x);
	}

	oLerp(_this, motVec);
}

void Hooks::setPos(Entity* ent, vec3_t& pos) {
	auto func = g_Hooks.setPosHook->GetFastcall<void, Entity*, vec3_t&>();
	static auto noLagBack = moduleMgr->getModule<NoLagBack>();

	if (noLagBack->isEnabled() && g_Data.canUseMoveKeys() && g_Data.isInGame() && ent == g_Data.getLocalPlayer()) return;
	func(ent, pos);
}

float Hooks::getGamma(uintptr_t* a1) {
	static auto nametsags = moduleMgr->getModule<NameTags>();
	static auto ambience = moduleMgr->getModule<Ambience>();
	static auto noRender = moduleMgr->getModule<NoRender>();
	static auto camera = moduleMgr->getModule<CameraMod>();
	static auto xray = moduleMgr->getModule<Xray>();
	uintptr_t** list = (uintptr_t**)a1;

	char obtainedSettings = 0;
	bool hadIt = false;
	for (uint16_t i = 3; i < 450; i++) {
		if (list[i] == nullptr) continue;
		uintptr_t* info = *(uintptr_t**)((uintptr_t)list[i] + 8);
		if (info == nullptr) continue;

		TextHolder* translateName = (TextHolder*)((uintptr_t)info + 0x1D8);
		TextHolder* settingname = (TextHolder*)((uintptr_t)info + 0x188);

		if (!strcmp(translateName->getText(), "options.smoothlighting")) {
			if (hadIt) continue;
			bool* smoothlightning = (bool*)((uintptr_t)list[i] + 16);
			xray->smoothLightningSetting = smoothlightning;

			obtainedSettings++; hadIt = true;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderWeather")) {
			bool* disableWeatherParticles = (bool*)((uintptr_t)list[i] + 16);
			if (ambience->isEnabled() && ambience->weather.getSelectedValue() == 1)
				*disableWeatherParticles = true; else *disableWeatherParticles = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderEntities")) {
			bool* disableRenderEntities = (bool*)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->entities)
				*disableRenderEntities = true; else *disableRenderEntities = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderBlockEntities")) {
			bool* disableRenderBlockEntities = (bool*)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->blockEntities)
				*disableRenderBlockEntities = true; else *disableRenderBlockEntities = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.dev_disableRenderParticles")) {
			bool* disableRenderParticles = (bool*)((uintptr_t)list[i] + 16);
			if (noRender->isEnabled() && noRender->particles)
				*disableRenderParticles = true; else *disableRenderParticles = false;
			obtainedSettings++;
		}
		else if (!strcmp(translateName->getText(), "options.hidehud")) {
			bool* hideHud = (bool*)((uintptr_t)list[i] + 16);

			isHUDHidden = *hideHud;

			obtainedSettings++;
		}

		if (obtainedSettings == 6) break;
	}

	if (xray->isEnabled())
		return 25.f;

	if (camera->isEnabled() && camera->fullbright)
		return camera->intensity;

	static auto ofunc = g_Hooks.getGammaHook->GetFastcall<float, uintptr_t*>();
	return ofunc(a1);
}

void Hooks::swing(Entity* _this) {
	static auto oFunc = g_Hooks.swingHook->GetFastcall<void, Entity*>();
	static auto invManager = moduleMgr->getModule<InvManager>();
	static auto noSwingMod = moduleMgr->getModule<NoSwing>();
	static auto killaura = moduleMgr->getModule<Killaura>();
	static auto scaffold = moduleMgr->getModule<Scaffold>();
	static auto breaker = moduleMgr->getModule<Breaker>();
	static auto regen = moduleMgr->getModule<Regen>();

	bool targetListEmpty = killaura->targetListEmpty;
	bool leftClicking = g_Data.isLeftClickDown();

	if (!(noSwingMod->isEnabled() ||
		// Scaffold
		scaffold->isEnabled() && !scaffold->swing && !leftClicking && targetListEmpty ||
		// Breaker
		breaker->isEnabled() && !breaker->swing && breaker->destroy || breaker->eat && !leftClicking && targetListEmpty ||
		// Regen
		regen->isEnabled() && !regen->swing && regen->destroy && !leftClicking && targetListEmpty ||
		// InvManager
		invManager->isEnabled() && !invManager->swing && !invManager->dropSlots.empty() && !leftClicking && targetListEmpty))
		return oFunc(_this);
}

bool Hooks::isImmobile(Entity* ent) {
	auto func = g_Hooks.isImmobileHook->GetFastcall<bool, Entity*>();
	static auto antiImmobile = moduleMgr->getModule<AntiImmobile>();

	if (ent == g_Data.getLocalPlayer()) {
		if (antiImmobile->isEnabled()) return false;
		g_Hooks.isPlayerImmobile = func(ent);
	}

	return func(ent);
}

#pragma endregion

#pragma region Other Hooks
__int64 Hooks::connectionRequestCreate(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId) {
	static auto oFunc = g_Hooks.connectionRequestCreateHook->GetFastcall<__int64, __int64, __int64, void*, TextHolder*, TextHolder*, __int64, TextHolder*, SkinData*, __int64, CoolSkinData*, TextHolder*, int, int, int, TextHolder*, bool, TextHolder*, __int64, TextHolder*, TextHolder*, bool, TextHolder*, TextHolder*, TextHolder*>();

	auto geoOverride = g_Data.getCustomGeoOverride();
	logF("Connection Request: InputMode: %i UiProfile: %i GuiScale: %i", inputMode, uiProfile, guiScale);
	auto fakeName = g_Data.getFakeName();

	__int64 res = oFunc(_this, privateKeyManager, a3, selfSignedId, serverAddress, clientRandomId, skinId, skinData, capeData, coolSkinStuff, deviceId, inputMode, uiProfile, guiScale, languageCode, sendEduModeParams, tenantId, unused, platformUserId, fakeName != nullptr ? fakeName : thirdPartyName, fakeName != nullptr ? true : thirdPartyNameOnly, platformOnlineId, platformOfflineId, capeId);
	return res;
}

void Hooks::clickFunc(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
	static auto oFunc = g_Hooks.clickFuncHook->GetFastcall<void, __int64, char, char, __int16, __int16, __int16, __int16, char>();
	static auto configMenu = moduleMgr->getModule<ConfigManagerMod>();
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	/*
	MouseButtons
	0 = mouse move
	1 = left click
	2 = right click
	3 = middle click
	4 = scroll (isDown: 120 (SCROLL UP) and -120 (SCROLL DOWN))
	*/

	ClickGui::onMouseClickUpdate((int)mouseButton, isDown);
	HImGui.onMouseClickUpdate((int)mouseButton, isDown);

	if (isDown) {
		if (mouseButton == 1) g_Data.leftclickCount++;
		else if (mouseButton == 2) g_Data.rightclickCount++;
	}

	if (clickGUI->isEnabled() || configMenu->isEnabled()) {
		if (mouseButton == 4) ClickGui::onWheelScroll(isDown > 0);
		if (mouseButton != 0) return;
	}
	if (ImGui::GetCurrentContext() != nullptr) {
		switch (mouseButton) {
		case 1:
			ImGui::GetIO().MouseDown[0] = isDown;
			break;
		case 2:
			ImGui::GetIO().MouseDown[1] = isDown;
			break;
		case 3:
			ImGui::GetIO().MouseDown[2] = isDown;
			break;
		case 4:
			ImGui::GetIO().MouseWheel = isDown < 0 ? -0.5f : 0.5f; //For scrolling
			break;
		default:
			break;
		}

		if (!ImGui::GetIO().WantCaptureMouse)
			return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);

	}
	else return oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
}

void Hooks::startDestroyBlock(GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5) {
	static auto oFunc = g_Hooks.startDestroyBlockHook->GetFastcall<void, GameMode*, vec3_ti*, uint8_t, void*, void*>();

	static auto nukerModule = moduleMgr->getModule<Nuker>();
	static auto speedMine = moduleMgr->getModule<SpeedMine>();

	if (nukerModule->isEnabled()) {
		vec3_ti tempPos;

		int range = nukerModule->getNukerRadius();
		const bool isVeinMiner = nukerModule->isVeinMiner();
		const bool isAutoMode = nukerModule->isAutoMode();

		BlockSource* region = g_Data.getLocalPlayer()->region;
		auto selectedBlockId = ((region->getBlock(*a2)->blockLegacy))->blockId;
		uint8_t selectedBlockData = region->getBlock(*a2)->data;

		if (!isAutoMode) {
			for (int x = -range; x < range; x++) {
				for (int y = -range; y < range; y++) {
					for (int z = -range; z < range; z++) {
						tempPos.x = a2->x + x;
						tempPos.y = a2->y + y;
						tempPos.z = a2->z + z;
						if (tempPos.y > 0) {
							Block* blok = region->getBlock(tempPos);
							uint8_t data = blok->data;
							auto id = blok->blockLegacy->blockId;
							if (blok->blockLegacy->material->isSolid == true && (!isVeinMiner || (id == selectedBlockId && data == selectedBlockData)))
								_this->destroyBlock(&tempPos, face);
						}
					}
				}
			}
		}
		return;
	}
	if (speedMine->isEnabled() && speedMine->instant) {
		_this->destroyBlock(a2, face);
		return;
	}

	oFunc(_this, a2, face, a4, a5);
}

__int64 Hooks::skinRepository_loadSkinPack(__int64 _this, __int64 pack, __int64 a3) {
	static auto func = g_Hooks.skinRepository_loadSkinPackHook->GetFastcall<__int64, __int64, __int64, __int64>();

	*(int*)((*(__int64*)pack) + 888i64) = 2;
	return func(_this, pack, a3);
}

void Hooks::renderNameTags(__int64 a1, __int64 a2, TextHolder* a3, __int64 a4) {
	static auto func = g_Hooks.renderNameTagsHook->GetFastcall<void, __int64, __int64, TextHolder*, __int64>();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (nameTagsMod->isEnabled() && nameTagsMod->nameTags.size() > 0) {
		std::string text = Utils::sanitize(a3->getText());
		size_t found = text.find('\n');

		if (found != std::string::npos) text = text.substr(0, found);
		if (nameTagsMod->nameTags.find(text) != nameTagsMod->nameTags.end())
			return;
	}

	return func(a1, a2, a3, a4);
}

float* Hooks::getFogColor(__int64 _this, float* color, __int64 a3, float a4) {
	static auto oGetFogMC_Color = g_Hooks.getFogColorHook->GetFastcall<float*, __int64, float*, __int64, float>();
	static auto customSky = moduleMgr->getModule<CustomSky>();
	static float rcolors[4];

	if (customSky->isEnabled()) {
		if (customSky->rainbow) {
			if (rcolors[3] < 1) {
				rcolors[0] = 1;
				rcolors[1] = 0.2f;
				rcolors[2] = 0.2f;
				rcolors[3] = 1;
			}
			Utils::MC_ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
			rcolors[0] += 0.001f;
			if (rcolors[0] >= 1) rcolors[0] = 0;
			Utils::MC_ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
			return rcolors;
		}
		else {
			color[0] = customSky->red;
			color[1] = customSky->green;
			color[2] = customSky->blue;
			return color;
		}
	}
	return oGetFogMC_Color(_this, color, a3, a4);
}

GamerTextHolder* Hooks::toStyledString(__int64 strIn, GamerTextHolder* strOut) {
	static auto func = g_Hooks.toStyledStringHook->GetFastcall<GamerTextHolder*, __int64, GamerTextHolder*>();

	static auto conRequest = reinterpret_cast<__int64>(g_Hooks.connectionRequestCreateHook->funcPtr);
	if (reinterpret_cast<__int64>(_ReturnAddress()) > conRequest && reinterpret_cast<__int64>(_ReturnAddress()) < conRequest + 10000 && overrideStyledReturn) {
		strOut->copyFrom(&styledReturnText);
		return strOut;
	}

	return func(strIn, strOut);
}

void Hooks::rakNetInstance_tick(RakNetInstance* _this, __int64 a2, __int64 a3) {
	static auto oTick = g_Hooks.rakNetInstance_tickHook->GetFastcall<void, RakNetInstance*, __int64, __int64>();
	GameData::setRakNetInstance(_this);
	oTick(_this, a2, a3);
}

void Hooks::autoComplete(__int64 a1, __int64 a2, TextHolder* text, int a4) {
	static auto oAutoComplete = g_Hooks.autoCompleteHook->GetFastcall<void, __int64, __int64, TextHolder*, int>();
	char* tx = text->getText();
	if (tx != nullptr && text->getTextLength() >= 1 && tx[0] == '.') {
		std::string search = tx + 1;
		transform(search.begin(), search.end(), search.begin(), ::tolower);

		struct LilPlump {
			std::string cmdAlias;
			IMCCommand* command = 0;
			bool shouldReplace = true;

			bool operator<(const LilPlump& o) const {
				return cmdAlias < o.cmdAlias;
			}
		};

		std::set<LilPlump> searchResults;

		std::vector<IMCCommand*>* commandList = cmdMgr->getCommandList();
		for (auto it = commandList->begin(); it != commandList->end(); ++it) {
			IMCCommand* c = *it;
			auto* aliasList = c->getAliasList();
			for (auto it = aliasList->begin(); it != aliasList->end(); ++it) {
				std::string cmd = *it;
				LilPlump plump;

				for (size_t i = 0; i < search.size(); i++) {
					char car = search.at(i);
					if (car == ' ' && i == cmd.size()) {
						plump.shouldReplace = false;
						break;
					}
					else if (i >= cmd.size()) goto outerContinue;

					if (car != cmd.at(i)) goto outerContinue;
				}
				{
					cmd.insert(0, 1, '.');

					plump.cmdAlias = cmd;
					plump.command = c;
					searchResults.emplace(plump);
				}

			outerContinue:
				continue;
			}
		}

		if (!searchResults.empty()) {
			LilPlump firstResult = *searchResults.begin();

			size_t maxReplaceLength = firstResult.cmdAlias.size();
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin()++; it != searchResults.end(); it++) {
					auto alias = it->cmdAlias;
					maxReplaceLength = std::min(maxReplaceLength, alias.size());

					for (int i = 0; i < maxReplaceLength; i++) {
						if (alias[i] != firstResult.cmdAlias[i]) {
							maxReplaceLength = i;
							break;
						}
					}
				}
			}
			else
				maxReplaceLength = firstResult.cmdAlias.size();

			g_Data.getGuiData()->displayClientMessageF("==========");
			if (searchResults.size() > 1) {
				for (auto it = searchResults.begin(); it != searchResults.end(); ++it) {
					LilPlump plump = *it;
					g_Data.getGuiData()->displayClientMessageF("%s%s - %s%s", plump.cmdAlias.c_str(), GRAY, ITALIC, plump.command->getDescription());
				}
			}
			if (firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1)[0] == 0) g_Data.getGuiData()->displayClientMessageF("%s%s %s- %s", WHITE, firstResult.cmdAlias.c_str(), GRAY, firstResult.command->getDescription());
			else g_Data.getGuiData()->displayClientMessageF("%s%s %s %s- %s", WHITE, firstResult.cmdAlias.c_str(), firstResult.command->getUsage(firstResult.cmdAlias.c_str() + 1 /*exclude prefix*/), GRAY, firstResult.command->getDescription());

			if (firstResult.shouldReplace) {
				if (search.size() == firstResult.cmdAlias.size() - 1 && searchResults.size() == 1) {
					maxReplaceLength++;
					firstResult.cmdAlias.append(" ");
				}
				text->setText(firstResult.cmdAlias.substr(0, maxReplaceLength));  // Set text
				using syncShit = void(__fastcall*)(TextHolder*, TextHolder*);
				static syncShit sync = reinterpret_cast<syncShit>(Utils::FindSignature("40 53 48 83 EC ? 48 8B DA 48 8D 4C 24 ? E8 ? ? ? ? 90 48 8B 40 ? 48 8B 08 48 8B 01 48 8B D3 FF 90 ? ? ? ? 90 48 8D 4C 24 ? E8 ? ? ? ? 80 7C 24 ? ? 74 ? 48 8B 4C 24 ? E8 ? ? ? ? 48 83 C4"));
				sync(text, text);
			}
		}
		return;
	}
	oAutoComplete(a1, a2, text, a4);
}

void Hooks::HIDController_keyMouse(HIDController* _this, void* a2, void* a3) {
	static auto oFunc = g_Hooks.HIDController_keyMouseHook->GetFastcall<void, HIDController*, void*, void*>();
	GameData::setHIDController(_this);
	isTicked = true;
	oFunc(_this, a2, a3);
	return;
}

__int64 Hooks::moveInputHandler_tick(MoveInputHandler* a1, Entity* a2) {
	static auto oTick = g_Hooks.moveInputHandler_tickHook->GetFastcall<__int64, MoveInputHandler*, Entity*>();

	auto ret = oTick(a1, a2);
	moduleMgr->onMove(a1);

	return 0;
}

__int64 Hooks::chestScreenController_tick(ChestScreenController* a1) {
	static auto oFunc = g_Hooks.chestScreenController_tickHook->GetFastcall<__int64, ChestScreenController*>();

	static auto chestStealerMod = moduleMgr->getModule<ChestStealer>();
	if (chestStealerMod->isEnabled()) chestStealerMod->chestScreenController_tick(a1);

	return oFunc(a1);
}

__int64 Hooks::setupAndRender(UIScene* uiscene, __int64 screencontext) {
	static auto oSetup = g_Hooks.setupAndRenderHook->GetFastcall<__int64, UIScene*, __int64>();

	g_Hooks.shouldRender = false;

	return oSetup(uiscene, screencontext);
}

__int64 Hooks::prepFeaturedServersFirstTime(__int64 a1, __int64 a2) {
	static auto func = g_Hooks.prepFeaturedServersFirstTimeHook->GetFastcall<__int64, __int64, __int64>();
	auto ret = func(a1, a2);

	prepServers();

	return ret;
}

__int64 Hooks::renderLevel(__int64 _this, __int64 a2, __int64 a3) {
	static auto oFunc = g_Hooks.renderLevelHook->GetFastcall<__int64, __int64, __int64, __int64>();
	static auto xray = moduleMgr->getModule<Xray>();

	using reloadShit_t = void(__fastcall*)(__int64);
	static reloadShit_t reloadChunk = nullptr;

	if (!reloadChunk) {
		reloadChunk = reinterpret_cast<reloadShit_t>(FindSignature("48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 44 24 ?? 48 8B F9 48 8D A9"));
	}

	static bool lastState = false;
	if (lastState != xray->isEnabled()) {
		lastState = xray->isEnabled();
		unsigned long long* v5;
		unsigned long long* i;

		v5 = *(unsigned long long**)(_this + 32);
		for (i = (unsigned long long*) * v5; i != v5; i = (unsigned long long*) * i) {
			reloadChunk(i[3]);
		}
	}

	auto ret = oFunc(_this, a2, a3);

	DrawUtils::setGameRenderContext(a2);
	moduleMgr->onLevelRender();
	DrawUtils::setGameRenderContext(0);

	return ret;
}

bool Hooks::directoryPackAccessStrategy_isTrusted(__int64 _this) {
	static auto func = g_Hooks.directoryPackAccessStrategy_isTrustedHook->GetFastcall<bool, __int64>();

	static uintptr_t** directoryPackAccessStrategyVtable = 0;

	if (!directoryPackAccessStrategyVtable) {
		uintptr_t sigOffset = FindSignature("48 8D 05 ?? ?? ?? ?? 49 89 06 49 8D 76 50");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		directoryPackAccessStrategyVtable = reinterpret_cast<uintptr_t**>(sigOffset + offset + 7);
	}

	if (*reinterpret_cast<uintptr_t*>(_this) == (uintptr_t)directoryPackAccessStrategyVtable)
		return true;

	return func(_this);
}

float Hooks::getPickRange(GameMode* _this, __int64 a2, char a3) {
	static auto oFunc = g_Hooks.getPickRangeHook->GetFastcall<float, GameMode*, __int64, char>();
	static auto blockReachMod = moduleMgr->getModule<BlockReach>();
	static auto clickTP = moduleMgr->getModule<ClickTP>();

	if (g_Data.getLocalPlayer() != nullptr) {
		if (blockReachMod->isEnabled()) return blockReachMod->distance;
		if (clickTP->isEnabled()) return 255;
	}

	return oFunc(_this, a2, a3);
}

__int8* Hooks::getLightEmission(BlockLegacy* a1, __int8* a2) {
	static auto oFunc = g_Hooks.getLightEmissionHook->GetFastcall<__int8*, BlockLegacy*, __int8*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		*a2 = 15;
		return a2;
	}
	return oFunc(a1, a2);
}

void Hooks::chestBlockActor_tick(ChestBlockActor* _this, void* a) {
	static auto oTick = g_Hooks.chestBlockActor_tickHook->GetFastcall<void, ChestBlockActor*, void*>();
	oTick(_this, a);
	static auto* chestESP = moduleMgr->getModule<ChestESP>();
	static auto* phase = moduleMgr->getModule<Phase>();

	if (_this != nullptr && phase->isEnabled() && phase->mode.getSelectedValue() == 1) GameData::addChestToList(_this);
	if (_this != nullptr && chestESP->isEnabled()) GameData::addChestToList(_this);
}

void Hooks::sendChatMessage(void* _this, TextHolder* text) {
	static auto oSendMessage = g_Hooks.sendChatMessageHook->GetFastcall<void, void*, TextHolder*>();

	if (text->getTextLength() > 0) {
		char* message = text->getText();

		if (*message == cmdMgr->prefix) {
			cmdMgr->execute(message);
			return;
		}
		else if (*message == '.') {
			static bool helpedUser = false;
			if (!helpedUser) {
				helpedUser = true;
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sYour prefix is: \"%s%c%s\"", GRAY, YELLOW, cmdMgr->prefix, RED);
				g_Data.getClientInstance()->getGuiData()->displayClientMessageF("%sEnter \"%s%cprefix .%s\" to reset your prefix", GRAY, YELLOW, cmdMgr->prefix, RED);
			}
		}
	}
	oSendMessage(_this, text);
}

__int64 Hooks::attack(GameMode* _this, Entity* ent) {
	if (!ent) return 0;
	auto func = g_Hooks.attackHook->GetFastcall<__int64, GameMode*, Entity*>();
	moduleMgr->onAttack(ent);
	return func(_this, ent);
}

__int64 Hooks::prepFeaturedServers(__int64 a1) {
	static auto func = g_Hooks.prepFeaturedServersHook->GetFastcall<__int64, __int64>();
	auto ret = func(a1);

	if (g_Data.getClientInstance() == nullptr) return ret;
	prepServers();

	return ret;
}

int Hooks::getRenderLayer(BlockLegacy* a1) {
	static auto oFunc = g_Hooks.getRenderLayerHook->GetFastcall<int, BlockLegacy*>();

	static auto xrayMod = moduleMgr->getModule<Xray>();
	if (xrayMod->isEnabled()) {
		char* text = a1->name.getText();
		if (strstr(text, "ore") == NULL)
			if (strcmp(text, "lava") != NULL)
				if (strcmp(text, "water") != NULL)
					if (strcmp(text, "portal") != NULL)
						if (strcmp(text, "amethyst_block") != NULL)
							if (strcmp(text, "ancient_debris") != NULL)
								if (strcmp(text, "command_block") != NULL)
									if (strcmp(text, "repeating_command_block") != NULL)
										if (strcmp(text, "chain_command_block") != NULL)
											if (strcmp(text, "structure_block") != NULL)
												if (strcmp(text, "deny") != NULL)
													if (strcmp(text, "allow") != NULL)
														if (strcmp(text, "bedrock") != NULL)
															if (strcmp(text, "gravel") != NULL)
																if (strcmp(text, "border_block") != NULL)
																	return 10;
	}
	return oFunc(a1);
}

bool Hooks::returnTrue(__int64 _this) {
	return true;
}

#pragma endregion

#pragma region ImGui
//Image Shit
// Simple helper function to load an image into a DX12 texture with common settings
// Returns true on success, with the SRV CPU handle having an SRV for the newly-created texture placed in it (srv_cpu_handle must be a handle in a valid descriptor heap)
#define STB_IMAGE_IMPLEMENTATION
bool LoadTextureFromFile(const char* filename, ID3D12Device* d3d_device, D3D12_CPU_DESCRIPTOR_HANDLE srv_cpu_handle, ID3D12Resource** out_tex_resource, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture resource
	D3D12_HEAP_PROPERTIES props;
	memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
	props.Type = D3D12_HEAP_TYPE_DEFAULT;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	D3D12_RESOURCE_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment = 0;
	desc.Width = image_width;
	desc.Height = image_height;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ID3D12Resource* pTexture = NULL;
	d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

	// Create a temporary upload resource to move the data in
	UINT uploadPitch = (image_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
	UINT uploadSize = image_height * uploadPitch;
	desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	desc.Alignment = 0;
	desc.Width = uploadSize;
	desc.Height = 1;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	desc.Flags = D3D12_RESOURCE_FLAG_NONE;

	props.Type = D3D12_HEAP_TYPE_UPLOAD;
	props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

	ID3D12Resource* uploadBuffer = NULL;
	HRESULT hr = d3d_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
	IM_ASSERT(SUCCEEDED(hr));

	// Write pixels into the upload resource
	void* mapped = NULL;
	D3D12_RANGE range = { 0, uploadSize };
	hr = uploadBuffer->Map(0, &range, &mapped);
	IM_ASSERT(SUCCEEDED(hr));
	for (int y = 0; y < image_height; y++)
		memcpy((void*)((uintptr_t)mapped + y * uploadPitch), image_data + y * image_width * 4, image_width * 4);
	uploadBuffer->Unmap(0, &range);

	// Copy the upload resource content into the real resource
	D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
	srcLocation.pResource = uploadBuffer;
	srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
	srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srcLocation.PlacedFootprint.Footprint.Width = image_width;
	srcLocation.PlacedFootprint.Footprint.Height = image_height;
	srcLocation.PlacedFootprint.Footprint.Depth = 1;
	srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

	D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
	dstLocation.pResource = pTexture;
	dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
	dstLocation.SubresourceIndex = 0;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = pTexture;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	// Create a temporary command queue to do the copy with
	ID3D12Fence* fence = NULL;
	hr = d3d_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	IM_ASSERT(SUCCEEDED(hr));

	HANDLE event = CreateEvent(0, 0, 0, 0);
	IM_ASSERT(event != NULL);

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 1;

	ID3D12CommandQueue* cmdQueue = NULL;
	hr = d3d_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12CommandAllocator* cmdAlloc = NULL;
	hr = d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
	IM_ASSERT(SUCCEEDED(hr));

	ID3D12GraphicsCommandList* cmdList = NULL;
	hr = d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
	IM_ASSERT(SUCCEEDED(hr));

	cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
	cmdList->ResourceBarrier(1, &barrier);

	hr = cmdList->Close();
	IM_ASSERT(SUCCEEDED(hr));

	// Execute the copy
	cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
	hr = cmdQueue->Signal(fence, 1);
	IM_ASSERT(SUCCEEDED(hr));

	// Wait for everything to complete
	fence->SetEventOnCompletion(1, event);
	WaitForSingleObject(event, INFINITE);

	// Tear down our temporary command queue and release the upload resource
	cmdList->Release();
	cmdAlloc->Release();
	cmdQueue->Release();
	CloseHandle(event);
	fence->Release();
	uploadBuffer->Release();

	// Create a shader resource view for the texture
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	d3d_device->CreateShaderResourceView(pTexture, &srvDesc, srv_cpu_handle);

	// Return results
	*out_tex_resource = pTexture;
	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

//ImGui Shit
typedef HRESULT(__thiscall* PresentD3D12)(IDXGISwapChain3*, UINT, UINT);
PresentD3D12 oPresentD3D12;
ID3D11Device* d3d11Device = nullptr;
ID3D12Device* d3d12Device = nullptr;
enum ID3D_Device_Type {
	INVALID_DEVICE_TYPE,
	D3D11,
	D3D12
};
struct FrameContext {
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12Resource* main_render_target_resource = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
};
uint64_t buffersCounts = -1;
FrameContext* frameContext = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapImGuiRender = nullptr;
ID3D12DescriptorHeap* d3d12DescriptorHeapBackBuffers = nullptr;
ID3D12GraphicsCommandList* d3d12CommandList = nullptr;
ID3D12CommandQueue* d3d12CommandQueue = nullptr;
bool initContext = false;
ID3D12CommandAllocator* Allocator = nullptr;


HRESULT hookPresentD3D12(IDXGISwapChain3* ppSwapChain, UINT syncInterval, UINT flags) {
	auto deviceType = ID3D_Device_Type::INVALID_DEVICE_TYPE;
	auto window = (HWND)FindWindowA(nullptr, (LPCSTR)"Minecraft");
	if (window == NULL) {
		logF("Failed to get Window HWND by name!");
		goto out;
	};
	if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
		deviceType = ID3D_Device_Type::D3D11;
	}
	else if (SUCCEEDED(ppSwapChain->GetDevice(IID_PPV_ARGS(&d3d12Device)))) {
		deviceType = ID3D_Device_Type::D3D12;
	};
	if (deviceType == ID3D_Device_Type::INVALID_DEVICE_TYPE) {
		logF("Failed to get device!");
		goto out;
	};
	if (deviceType == ID3D_Device_Type::D3D11) {
		if (!initContext) {
			ImGui::CreateContext();
			initContext = true;
		};
		ID3D11DeviceContext* ppContext = nullptr;
		d3d11Device->GetImmediateContext(&ppContext);
		ID3D11Texture2D* pBackBuffer;
		ppSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		ID3D11RenderTargetView* mainRenderTargetView;
		d3d11Device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
		pBackBuffer->Release();
		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX11_Init(d3d11Device, ppContext);
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (moduleMgr != nullptr)
			moduleMgr->onImGuiRender();

		ImGui::Render();
		ppContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		mainRenderTargetView->Release();
		d3d11Device->Release();
	}
	else if (deviceType == ID3D_Device_Type::D3D12) {
		if (!initContext)
			ImGui::CreateContext();
		DXGI_SWAP_CHAIN_DESC sdesc;
		ppSwapChain->GetDesc(&sdesc);
		sdesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		sdesc.OutputWindow = window;
		sdesc.Windowed = ((GetWindowLongPtr(window, GWL_STYLE) & WS_POPUP) != 0) ? false : true;
		buffersCounts = sdesc.BufferCount;
		frameContext = new FrameContext[buffersCounts];
		D3D12_DESCRIPTOR_HEAP_DESC descriptorImGuiRender = {};
		descriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptorImGuiRender.NumDescriptors = buffersCounts;
		descriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		if (d3d12DescriptorHeapImGuiRender == nullptr)
			if (FAILED(d3d12Device->CreateDescriptorHeap(&descriptorImGuiRender, IID_PPV_ARGS(&d3d12DescriptorHeapImGuiRender))))
				goto out;

		// We need to pass a D3D12_CPU_DESCRIPTOR_HANDLE in ImTextureID, so make sure it will fit
		static_assert(sizeof(ImTextureID) >= sizeof(D3D12_CPU_DESCRIPTOR_HANDLE), "D3D12_CPU_DESCRIPTOR_HANDLE is too large to fit in an ImTextureID");

		// We presume here that we have our D3D device pointer in g_pd3dDevice

		int my_image_width = 256;
		int my_image_height = 256;
		ID3D12Resource* my_texture = NULL;

		// Get CPU/GPU handles for the shader resource view
		// Normally your engine will have some sort of allocator for these - here we assume that there's an SRV descriptor heap in
		// g_pd3dSrvDescHeap with at least two descriptors allocated, and descriptor 1 is unused
		UINT handle_increment = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		int descriptor_index = 1; // The descriptor table index to use (not normally a hard-coded constant, but in this case we'll assume we have slot 1 reserved for us)
		D3D12_CPU_DESCRIPTOR_HANDLE my_texture_srv_cpu_handle = d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart();
		my_texture_srv_cpu_handle.ptr += (handle_increment * descriptor_index);
		D3D12_GPU_DESCRIPTOR_HANDLE my_texture_srv_gpu_handle = d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart();
		my_texture_srv_gpu_handle.ptr += (handle_increment * descriptor_index);

		if (d3d12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator)) != S_OK)
			return false;
		for (size_t i = 0; i < buffersCounts; i++) {
			frameContext[i].commandAllocator = Allocator;
		};
		if (d3d12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&d3d12CommandList)) != S_OK ||
			d3d12CommandList->Close() != S_OK)
			return false;
		D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
		descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		descriptorBackBuffers.NumDescriptors = buffersCounts;
		descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		descriptorBackBuffers.NodeMask = 1;
		if (d3d12Device->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
			return false;
		const auto rtvDescriptorSize = d3d12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = d3d12DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();
		for (size_t i = 0; i < buffersCounts; i++) {
			ID3D12Resource* pBackBuffer = nullptr;
			frameContext[i].main_render_target_descriptor = rtvHandle;
			ppSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			d3d12Device->CreateRenderTargetView(pBackBuffer, nullptr, rtvHandle);
			frameContext[i].main_render_target_resource = pBackBuffer;
			rtvHandle.ptr += rtvDescriptorSize;
			pBackBuffer->Release();
		};
		if (!initContext) {
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX12_Init(d3d12Device, buffersCounts,
				DXGI_FORMAT_R8G8B8A8_UNORM,
				d3d12DescriptorHeapImGuiRender,
				d3d12DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				d3d12DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
			initContext = true;
		};
		if (d3d12CommandQueue == nullptr)
			goto out;

		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		{
			D3D12_DESCRIPTOR_HEAP_DESC desc = {};
			desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			desc.NumDescriptors = 2; // <-- Set this value to 2 (the first descriptor is used for the built-in font texture, the second for our new texture)
			desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			if (d3d12Device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&d3d12DescriptorHeapBackBuffers)) != S_OK)
				return false;
		}

		// Load the texture from a file
		if (my_texture == nullptr && my_texture == NULL) {
			//bool ret = LoadTextureFromFile("C:\\Users\\jcazm\\Downloads\\MyImage01.jpg", d3d12Device, my_texture_srv_cpu_handle, &my_texture, &my_image_width, &my_image_height);
			//IM_ASSERT(ret);
		}

		if (moduleMgr != nullptr) {
			moduleMgr->onImGuiRender();

			/*ImGui::Begin("DirectX12 Texture Test");
			ImGui::Text("CPU handle = %p", my_texture_srv_cpu_handle.ptr);
			ImGui::Text("GPU handle = %p", my_texture_srv_gpu_handle.ptr);
			ImGui::Text("size = %d x %d", my_image_width, my_image_height);
			// Note that we pass the GPU SRV handle here, *not* the CPU handle. We're passing the internal pointer value, cast to an ImTextureID
			//if (my_texture != nullptr && my_texture != NULL) 
			//ImGui::Image((ImTextureID)my_texture_srv_gpu_handle.ptr, ImVec2((float)my_image_width, (float)my_image_height));
			ImGui::End();
			*/
			
		}

		FrameContext& currentFrameContext = frameContext[ppSwapChain->GetCurrentBackBufferIndex()];
		currentFrameContext.commandAllocator->Reset();
		D3D12_RESOURCE_BARRIER barrier;
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = currentFrameContext.main_render_target_resource;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		d3d12CommandList->Reset(currentFrameContext.commandAllocator, nullptr);
		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->OMSetRenderTargets(1, &currentFrameContext.main_render_target_descriptor, FALSE, nullptr);
		d3d12CommandList->SetDescriptorHeaps(1, &d3d12DescriptorHeapImGuiRender);
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), d3d12CommandList);
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		d3d12CommandList->ResourceBarrier(1, &barrier);
		d3d12CommandList->Close();
		d3d12CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&d3d12CommandList));
		d3d12DescriptorHeapBackBuffers->Release();
		d3d12CommandList->Release();
		Allocator->Release();
		currentFrameContext.main_render_target_resource->Release();
		currentFrameContext.commandAllocator->Release();
		d3d12Device->Release();
		delete frameContext;
	};
	goto out;
out:
	return oPresentD3D12(ppSwapChain, syncInterval, flags);
};

typedef void(__thiscall* ExecuteCommandListsD3D12)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
ExecuteCommandListsD3D12 oExecuteCommandListsD3D12;

void hookExecuteCommandListsD3D12(ID3D12CommandQueue* queue, UINT NumCommandLists, ID3D12CommandList* ppCommandLists) {
	if (!d3d12CommandQueue)
		d3d12CommandQueue = queue;

	oExecuteCommandListsD3D12(queue, NumCommandLists, ppCommandLists);
};

void Hooks::InitImGui() {
	if (kiero::init(kiero::RenderType::D3D12) == kiero::Status::Success)
		logF("Created hook for SwapChain::Present (DX12)!");

	if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		logF("Created hook for SwapChain::Present (DX11)!");

	kiero::bind(54, (void**)&oExecuteCommandListsD3D12, hookExecuteCommandListsD3D12);
	kiero::bind(140, (void**)&oPresentD3D12, hookPresentD3D12);
}
#pragma endregion
