#pragma once
#include "../Module.h"

class FastSwim : public IModule {
private:
	float speedh = 0.40f;
	float speedv = 0.40f;
public:
	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
	FastSwim();
};