#include "ConfigManagerMod.h"

#include "../../../Menu/ConfigManagerMenu.h"


ConfigManagerMod::ConfigManagerMod() : IModule(0, Category::UNUSED, "A GUI where you can manage your configs") {
	shouldHide = true;
}

const char* ConfigManagerMod::getModuleName() {
	return ("ConfigManager");
}

bool ConfigManagerMod::allowAutoStart() {
	return false;
}

void ConfigManagerMod::onEnable() {
	g_Data.getClientInstance()->releaseMouse();
}

void ConfigManagerMod::onTick(GameMode* gm) {
	shouldHide = true;
}

void ConfigManagerMod::onPostRender(MinecraftUIRenderContext* renderCtx) {
	if (GameData::canUseMoveKeys()) g_Data.getClientInstance()->releaseMouse();
}

void ConfigManagerMod::onPreRender(MinecraftUIRenderContext* renderCtx) {
}

void ConfigManagerMod::onDisable() {
	g_Data.getClientInstance()->grabMouse();
}

void ConfigManagerMod::onLoadConfig(void* conf) {
	//IModule::onLoadConfig(conf);
	//ConfigManager::onLoadSettings(conf);
}
void ConfigManagerMod::onSaveConfig(void* conf) {
	//ConfigManager::onSaveSettings(conf);
}