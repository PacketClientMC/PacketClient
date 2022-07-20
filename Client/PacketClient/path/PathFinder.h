#pragma once

#include "../../Utils/HMath.h"
#include "Path.h"
#include "PathSegment.h"
#include "goals/PathGoal.h"
#include "../../SDK/BlockLegacy.h"
#include "PathConstants.h"
#include <memory>

class PathFinder {
private:
	vec3_ti startPos;
	Path currentPath;
	BlockSource* region;
	std::shared_ptr<PathGoal> goal;
public:
	bool terminateSearch = false;
	float pathSearchTimeout = 5.f;

	PathFinder(vec3_ti start, BlockSource* reg, std::shared_ptr<PathGoal> goal);

	Path findPath();
	const Path& getCurrentPath() const;
};