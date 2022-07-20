#include "CameraMod.h"

CameraMod::CameraMod() : IModule(0, Category::VISUAL, "Modifies your camera") {
	registerBoolSetting("NoHurtcam", &nohurtcam, nohurtcam);
	registerBoolSetting("Fullbright", &fullbright, fullbright);
	registerBoolSetting("ViewClip", &viewClip, viewClip);
	registerIntSetting("Intensity", &intensity, intensity, -25, 25);
}

const char* CameraMod::getModuleName() {
	return "Camera";
}

float originalGamma = -1;

void CameraMod::onEnable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			originalGamma = *gammaPtr;
			*gammaPtr = 10;
		}
	}
}

void CameraMod::onTick(GameMode* gm) {
	if (fullbright) {
		if (gammaPtr != nullptr && *gammaPtr != 10)
			*gammaPtr = 10;
	}
}

void CameraMod::onDisable() {
	if (fullbright) {
		if (gammaPtr != nullptr) {
			if (originalGamma >= 0 && originalGamma <= 1)
				*gammaPtr = originalGamma;
			else
				*gammaPtr = 0.5f;
		}
	}
}