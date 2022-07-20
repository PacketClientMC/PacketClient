#pragma once

class Dimension {
private:
	char pad_0x0[0xB8]; //0x0
public:
	class BrightnessRamp* brightnessRamp; //0xB8
private:
	char pad_0xC0[0xA8]; //0xC0
public:
	int dimensionId; //0x168
private:
	char pad_0x16C[0x1]; //0x16C
public:
	bool hasCeiling; //0x16D
private:
	char pad_0x16E[0x2]; //0x16E
public:
	int timeThingy; //0x170
private:
	char pad_0x174[0x34]; //0x174
public:
	class Weather* weather; //0x1A8
};