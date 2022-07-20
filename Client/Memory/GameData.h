#pragma once

#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <set>

#include "../PacketClient/Config/AccountInformation.h"
#include "../SDK/ChestBlockActor.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/GameMode.h"
#include "../SDK/HIDController.h"
#include "../SDK/MoveInputHandler.h"
#include "../SDK/RakNetInstance.h"
#include "../Utils/SkinUtil.h"
#include "../Utils/TextFormat.h"
#include "SlimMem.h"
#include <string>

enum DATAPACKET_CMD : int {
	CMD_INIT = 0,
	CMD_PING,
	CMD_PONG,
	CMD_OPENBROWSER,
	CMD_FILECHOOSER,
	CMD_RESPONSE,
	CMD_FOLDERCHOOSER,  // sets permissions for a whole folder and sends the path over
	CMD_LOG
};

struct NetworkedData {
	unsigned int xorKey = 0;
	unsigned int localPlayerOffset = 0x94;  // Scrambled data
	bool dataSet = false;
};

struct SkinData;

class GameData {
private:
	ClientInstance* clientInstance = nullptr;
	LocalPlayer* localPlayer = nullptr;
	GameMode* gameMode = nullptr;
	EntityList* entityList = nullptr;
	RakNetInstance* raknetInstance = nullptr;

	void* hDllInst = 0;
	std::vector<std::shared_ptr<AABB>> chestList;
	std::vector<std::string> textPrintList;
	std::mutex textPrintLock;
	std::mutex chestListMutex;
	int lastRequestId = 0;
	std::shared_ptr<std::string> customGeometry;
	bool customGeoActive = false;
	std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexture;
	bool customTextureActive = false;

	bool injectorConnectionActive = false;
	const SlimUtils::SlimModule* gameModule = 0;
	SlimUtils::SlimMem* slimMem;
	bool shouldTerminateB = false;
	bool shouldHideB = false;
	bool isAllowingWIPFeatures = false;
	__int64 lastUpdate;
	static void retrieveClientInstance();
	TextHolder* fakeName;

public:
	NetworkedData networkedData;
	HIDController* hidController = nullptr;

	static bool canUseMoveKeys();
	static bool isKeyDown(int key);
	static bool isKeyPressed(int key);
	static bool isRightClickDown();
	static bool isLeftClickDown();
	static bool isWheelDown();
	static bool shouldTerminate();
	static bool shouldHide();
	static void hide();
	static void terminate();
	static void updateGameData(GameMode* gameMode);
	static void initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem, void* hDllInst);
	static void addChestToList(ChestBlockActor* ChestBlock2);
	static void EntityList_tick(EntityList* list);
	static void setHIDController(HIDController* Hid);
	static void setRakNetInstance(RakNetInstance* raknet);
	static void log(const char* fmt, ...);
	float fov = 0.f;
	int fps = 0;
	int frameCount = 0;
	int cpsLeft = 0;
	int cpsRight = 0;
	int leftclickCount = 0;
	int rightclickCount = 0;

	void clearChestList() {
		std::lock_guard<std::mutex> listGuard(chestListMutex);
		this->chestList.clear();
	}

	inline void setCustomGeometryOverride(bool setActive, std::shared_ptr<std::string> customGeoPtr) {
		this->customGeoActive = setActive;
		if (setActive)
			this->customGeometry.swap(customGeoPtr);
		else
			this->customGeometry.reset();
	}

	inline std::tuple<bool, std::shared_ptr<std::string>> getCustomGeoOverride() {
		return std::make_tuple(this->customGeoActive, this->customGeometry);
	}

	inline void setCustomTextureOverride(bool setActive, std::shared_ptr<std::tuple<std::shared_ptr<unsigned char[]>, size_t>> customTexturePtr) {
		this->customTextureActive = setActive;
		if (setActive)
			this->customTexture.swap(customTexturePtr);
		else
			this->customTexture.reset();
	}

	inline auto getCustomTextureOverride() {
		return std::make_tuple(this->customTextureActive, this->customTexture);
	}
	
	inline void setAllowWIPFeatures(bool enable = false) { isAllowingWIPFeatures = enable; };
	inline bool isInjectorConnectionActive() { return injectorConnectionActive; };
	inline void* getDllModule() { return hDllInst; };
	inline ClientInstance* getClientInstance() { return clientInstance; };
	inline GuiData* getGuiData() { return clientInstance->getGuiData(); };
	inline LocalPlayer* getLocalPlayer() {
		localPlayer = clientInstance->getLocalPlayer();
		if (localPlayer == nullptr) gameMode = nullptr;
		return localPlayer;
	};

	LocalPlayer** getPtrLocalPlayer() {
		return &localPlayer;
	};

	bool isInGame() {
		return localPlayer != nullptr;
	}

	const SlimUtils::SlimModule* getModule() {
		return gameModule;
	};

	const SlimUtils::SlimMem* getSlimMem() {
		return slimMem;
	};

	GameMode* getGameMode() { return gameMode; };
	EntityList* getEntityList() { return entityList; };
	HIDController** getHIDController() { return &hidController; };
	RakNetInstance* getRakNetInstance() { return raknetInstance; };
	std::vector<std::shared_ptr<AABB>>* getChestList() { return &chestList; };
	auto lockChestList() { return std::lock_guard<std::mutex>(this->chestListMutex); }
	void setFakeName(TextHolder* name) { fakeName = name; };
	TextHolder* getFakeName() { return fakeName; };
	inline __int64 getLastUpdateTime() { return lastUpdate; };
	void forEachEntity(std::function<void(Entity*, bool)>);
	int getFPS() { return fps; };
	int getLeftCPS() { return cpsLeft; };
	int getRightCPS() { return cpsRight; };
};

extern GameData g_Data;