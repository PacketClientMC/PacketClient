#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <filesystem>
#include <fstream>
#include <string>
#pragma comment(lib, "runtimeobject")

#include "../../Utils/Json.hpp"
#include "../Command/CommandMgr.h"
#include "../Module/ModuleManager.h"

using json = nlohmann::json;

class ConfigManager {
private:
	std::wstring roamingFolder;
	static std::wstring GetRoamingFolderPath();
	json currentConfigObj;

public:
#ifdef _DEBUG
	std::string currentConfig = "PCBeta";
#else
	std::string currentConfig = "PCPublic";
#endif

	void loadConfig(std::string name, bool create);
	void saveConfig();
	void init();

	ConfigManager();
};

extern ConfigManager* configMgr;
