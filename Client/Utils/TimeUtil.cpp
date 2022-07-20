#include "TimeUtil.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <Windows.h>
#include "../Utils/Logger.h"

__int64 lastMS = TimeUtil::getCurrentMs();
__int64 ms;
__int64 prevMS = 0;
__int64 timeMS = -1;

__int64 TimeUtil::getCurrentMs() {
	FILETIME f;
	GetSystemTimeAsFileTime(&f);
	(long long)f.dwHighDateTime;
	__int64 nano = ((__int64)f.dwHighDateTime << 32LL) + (__int64)f.dwLowDateTime;
	return (nano - 116444736000000000LL) / 10000;
}

bool TimeUtil::hasTimePassed(__int64 ms) {
	return getCurrentMs() >= timeMS + ms;
}

__int64 TimeUtil::getTime() {
	return getCurrentMs();
}

void TimeUtil::reset2() {
	prevMS = getTime();
}

__int64 TimeUtil::getDifference() {
	return getTime() - prevMS;
}

void TimeUtil::setDifference(__int64 difference) {
	prevMS = getTime() - difference;
}

void TimeUtil::resetTime() {
	lastMS = getCurrentMs();
	timeMS = getCurrentMs();
}

__int64 TimeUtil::getElapsedTime1() {
	return (getCurrentMs() - timeMS);
}

bool TimeUtil::hasTimedElapsed(__int64 time, bool reset) {
	if (getCurrentMs() - lastMS > time) {
		if (reset)
			resetTime();
		return true;
	}
	return false;
}

void TimeUtil::reset1() {
	ms = getCurrentMs();
}

bool TimeUtil::elapsed(__int64 milliseconds) {
	return (getCurrentMs() - ms) > milliseconds;
}

__int64 TimeUtil::getElapsedTime() {
	return getCurrentMs() - ms;
}