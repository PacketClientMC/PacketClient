#pragma once

#include "../PacketClient/Menu/ConfigManagerMenu.h"
#include "../PacketClient/Config/ConfigManager.h"
#include "../PacketClient/Module/ModuleManager.h"
#include "../PacketClient/Command/CommandMgr.h"
#include "../include/imgui/imgui_impl_win32.h"
#include "../include/imgui/imgui_impl_dx12.h"
#include "../include/imgui/imgui_impl_dx11.h"
#include "../SDK/MinecraftUIRenderContext.h"
#include "../PacketClient/Menu/HudEditor.h"
#include "../PacketClient/Menu/ClickGui.h"
#include "../PacketClient/Menu/TabGui.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/ChestBlockActor.h"
#include "../include/imgui/imgui.h"
#include "../include/kiero/kiero.h"
#include "../Utils/ImmediateGui.h"
#include "../SDK/RakNetInstance.h"
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d3d12.lib")
#include "../Utils/TextFormat.h"
#pragma comment(lib, "d2d1.lib")
#include "../Utils/DrawUtils.h"
#include "../SDK/BlockLegacy.h"
#include "../SDK/TextHolder.h"
#include "../Utils/SkinUtil.h"
#include "../SDK/GameMode.h"
#include "../Utils/Logger.h"
#include "../SDK/UIScene.h"
#include "../SDK/Camera.h"
#include <combaseapi.h>
#include "GameData.h"
#include <initguid.h>
#include <dwrite_1.h>
#include "MinHook.h"
#include <dxgi1_4.h>
#include <d2d1_1.h>
#include <intrin.h>
#include <d3d11.h>
#include <d3d12.h>
#include <thread>
#include <dxgi.h>


class VMTHook;
class FuncHook;

struct CoolSkinData {
	TextHolder unknown;
	TextHolder unknown2;
	TextHolder skinResourcePatch;  // 0x040
	TextHolder geometryName;       // 0x060 "geometry.humanoid.custom"
	unsigned char gap2[0x40];      // 0x080
	void* startAnimatedFrames;     // 0x0C0
	void* endAnimatedFrames;       // 0x0C8
	unsigned char gap3[0x8];       // 0x0D0
	TextHolder geometryData;       // 0x0D8
	TextHolder skinAnimationData;  // 0x0F8
	unsigned char gap4[0x20];      // 0x118
	bool isPremiumSkin;            // 0x138
	bool isPersonaSkin;
	bool isCapeOnClassicSkin;
	void* startPersonaPieces;
	void* endPersonaPieces;
	unsigned char gap5[0x8];  // 0x150
	TextHolder armSize;       // 0x158
	unsigned char gap6[0x8];  // 0x178
	void* startPieces;
};

class Hooks {
private:
	bool shouldRender = true;
public:
	bool isThirdPerson = false;

	char currentScreenName[100];
	std::vector<std::shared_ptr<FuncHook>> lambdaHooks;
	struct EntityListPointerHolder {
		Entity* ent;
		int addedTick;
	};

	std::vector<EntityListPointerHolder> entityList;
	bool isPlayerImmobile = false;

	static void Init();
	static void InitImGui();
	static void Restore();
	static void Enable();
private:
	/* Tick Hooks */
	static float playerCallBack(Player* lp, __int64 a2, __int64 a3);
	static void multiLevelPlayer_tick(EntityList* entityList);
	static void* tickWorld(Player* _this, __int64);
	static void baseTick(Entity* _this);

	/* Main Hooks */
	static void loopbackPacketSender_sendToServer(LoopbackPacketSender* a, Packet* packet);
	static __int64 renderText(__int64 a1, MinecraftUIRenderContext* renderCtx);
	static __int64 render(UIScene* uiscene, __int64 screencontext);
	static void chatLog(__int64 a1, TextHolder* msg, uint32_t a2);
	static float getDestroySpeed(Player* _this, Block& block);
	static float getTimeOfDay(__int64, int a2, float a3);
	static void cameraRotation(__int64 _this, void* a2);
	static void rotation(Entity* _this, vec2_t& angle);
	static int forceThirdPerson(__int64 perspective);
	static void lerpMotion(Entity* _this, vec3_t);
	static void setPos(Entity* ent, vec3_t& pos);
	static float getGamma(uintptr_t* a1);
	static void swing(Entity* _this);
	static bool isImmobile(Entity*);

	/* Other Hooks */
	static __int64 connectionRequestCreate(__int64 _this, __int64 privateKeyManager, void* a3, TextHolder* selfSignedId, TextHolder* serverAddress, __int64 clientRandomId, TextHolder* skinId, SkinData* skinData, __int64 capeData, CoolSkinData* coolSkinStuff, TextHolder* deviceId, int inputMode, int uiProfile, int guiScale, TextHolder* languageCode, bool sendEduModeParams, TextHolder* tenantId, __int64 unused, TextHolder* platformUserId, TextHolder* thirdPartyName, bool thirdPartyNameOnly, TextHolder* platformOnlineId, TextHolder* platformOfflineId, TextHolder* capeId);
	static void clickFunc(__int64 a1, char a2, char a3, __int16 a4, __int16 a5, __int16 a6, __int16 a7, char a8);
	static void startDestroyBlock(GameMode* _this, vec3_ti* a2, uint8_t face, void* a4, void* a5);
	static __int64 skinRepository_loadSkinPack(__int64 _this, __int64 pack, __int64 a3);
	static void renderNameTags(__int64 a1, __int64 a2, TextHolder* name, __int64 a4);
	static float* getFogColor(__int64, float* color, __int64 brightness, float a4);
	static GamerTextHolder* toStyledString(__int64 strIn, GamerTextHolder* strOut);
	static void rakNetInstance_tick(RakNetInstance* _this, __int64 a2, __int64 a3);
	static void autoComplete(__int64 _this, __int64 a2, TextHolder* text, int a4);
	static void HIDController_keyMouse(HIDController* _this, void* a2, void* a3);
	static __int64 moveInputHandler_tick(MoveInputHandler* _this, Entity* a2);
	static __int64 chestScreenController_tick(ChestScreenController* _this);
	static __int64 setupAndRender(UIScene* uiscene, __int64 screencontext);
	static __int64 prepFeaturedServersFirstTime(__int64 a1, __int64 a2);
	static __int64 renderLevel(__int64 _this, __int64 a2, __int64 a3);
	static bool directoryPackAccessStrategy_isTrusted(__int64 _this);
	static float getPickRange(GameMode* _this, __int64 a2, char a3);
	static __int8* getLightEmission(BlockLegacy* _this, __int8* a2);
	static void chestBlockActor_tick(ChestBlockActor*, void* a);
	static void sendChatMessage(void* _this, TextHolder* text);
	static __int64 attack(GameMode* _this, Entity*);
	static __int64 prepFeaturedServers(__int64 a1);
	static int getRenderLayer(BlockLegacy* a1);
	static bool returnTrue(__int64 _this);

	std::unique_ptr<FuncHook> directoryPackAccessStrategy_isTrustedHook;
	std::unique_ptr<FuncHook> skinRepository_checkSignatureFileInPack;
	std::unique_ptr<FuncHook> loopbackPacketSender_sendToServerHook;
	std::unique_ptr<FuncHook> zipPackAccessStrategy_isTrustedHook;
	std::unique_ptr<FuncHook> prepFeaturedServersFirstTimeHook;
	std::unique_ptr<FuncHook> skinRepository_loadSkinPackHook;
	std::unique_ptr<FuncHook> chestScreenController_tickHook;
	std::unique_ptr<FuncHook> connectionRequestCreateHook;
	std::unique_ptr<FuncHook> HIDController_keyMouseHook;
	std::unique_ptr<FuncHook> multiLevelPlayer_tickHook;
	std::unique_ptr<FuncHook> moveInputHandler_tickHook;
	std::unique_ptr<FuncHook> chestBlockActor_tickHook;
	std::unique_ptr<FuncHook> rakNetInstance_tickHook;
	std::unique_ptr<FuncHook> prepFeaturedServersHook;
	std::unique_ptr<FuncHook> startDestroyBlockHook;
	std::unique_ptr<FuncHook> getLightEmissionHook;
	std::unique_ptr<FuncHook> forceThirdPersonHook;
	std::unique_ptr<FuncHook> sendChatMessageHook;
	std::unique_ptr<FuncHook> playerCallBack_Hook;
	std::unique_ptr<FuncHook> getDestroySpeedHook;
	std::unique_ptr<FuncHook> renderNameTagsHook;
	std::unique_ptr<FuncHook> getRenderLayerHook;
	std::unique_ptr<FuncHook> setupAndRenderHook;
	std::unique_ptr<FuncHook> toStyledStringHook;
	std::unique_ptr<FuncHook> cameraRotationHook;
	std::unique_ptr<FuncHook> getTimeOfDayHook;
	std::unique_ptr<FuncHook> getPickRangeHook;
	std::unique_ptr<FuncHook> autoCompleteHook;
	std::unique_ptr<FuncHook> renderLevelHook;
	std::unique_ptr<FuncHook> getFogColorHook;
	std::unique_ptr<FuncHook> lerpMotionHook;
	std::unique_ptr<FuncHook> isImmobileHook;
	std::unique_ptr<FuncHook> renderTextHook;
	std::unique_ptr<FuncHook> tickWorldHook;
	std::unique_ptr<FuncHook> clickFuncHook;
	std::unique_ptr<FuncHook> rotationHook;
	std::unique_ptr<FuncHook> baseTickHook;
	std::unique_ptr<FuncHook> getGammaHook;
	std::unique_ptr<FuncHook> chatLogHook;
	std::unique_ptr<FuncHook> attackHook;
	std::unique_ptr<FuncHook> renderHook;
	std::unique_ptr<FuncHook> setPosHook;
	std::unique_ptr<FuncHook> swingHook; //./aidfix.storager./69
};

extern Hooks g_Hooks;

class FuncHook {
public:
	void* funcPtr;
	void* funcReal;

	FuncHook(void* func, void* hooked) {
		funcPtr = func;

		MH_STATUS ret = MH_CreateHook(func, hooked, &funcReal);
		if (ret == MH_OK && (__int64)func > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	FuncHook(uintptr_t func, void* hooked) {
		funcPtr = reinterpret_cast<void*>(func);

		MH_STATUS ret = MH_CreateHook(funcPtr, hooked, &funcReal);
		if (ret == MH_OK && (__int64)funcPtr > 10) {
		} else
			logF("MH_CreateHook = %i", ret);
	};

	void enableHook(bool enable = true) {
		if (funcPtr != nullptr) {
			int ret = enable ? MH_EnableHook(funcPtr) : MH_DisableHook(funcPtr);
			if (ret != MH_OK)
				logF("MH_EnableHook = %i", ret);
		} else
			logF("enableHook() called with nullptr func!");
	}

	~FuncHook() {
		Restore();
	}

	void Restore() {
		if (funcPtr != nullptr)
			MH_DisableHook(funcPtr);
	}

	template <typename TRet, typename... TArgs>
	inline auto* GetFastcall() {
		using Fn = TRet(__fastcall*)(TArgs...);
		return reinterpret_cast<Fn>(funcReal);
	};
};