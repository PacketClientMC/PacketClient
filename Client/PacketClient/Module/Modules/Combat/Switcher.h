#include "../Module.h"

class Switcher : public IModule {
private:
	int ternary = false;
	int ternary1 = false;
	bool autos = false;
	bool ss = false;
	bool swap = false;
	bool ownage = false;
	bool ternaryO = false;
	bool toggler = false;

public:
	Switcher();

	virtual void onAttack(Entity* attackedEnt);
	virtual void onTick(GameMode* gm);
	virtual const char* getModuleName();
	virtual void onEnable();
};
