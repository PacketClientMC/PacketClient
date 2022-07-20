#pragma once
#include "../Module.h"

class Teams : public IModule {
private:
	bool alliedCheck = false;
	bool colorCheck = true;

public:
	Teams();

	bool isAlliedCheckEnabled() { return this->isEnabled() && alliedCheck; };
	bool isMC_ColorCheckEnabled() { return this->isEnabled() && colorCheck; };

	virtual const char* getModuleName();
};
