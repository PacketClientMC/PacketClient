#include "MoveInputHandler.h"
#include "../Utils/Utils.h"

__int64 MoveInputHandler::clearMovementState() {
	return Utils::CallVFunc<4, __int64>(this);
}