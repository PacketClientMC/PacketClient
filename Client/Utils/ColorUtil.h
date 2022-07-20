#pragma once

struct MC_Color {
	union {
		struct { float r, g, b, a; };
		float arr[4]{};
	};

	MC_Color() {
		this->r = 1.f;
		this->g = 1.f;
		this->b = 1.f;
		this->a = 1.f;
	};

	MC_Color(const float r, const float g, const float b, const float a = 255) {
		this->r = r / 255.0f;
		this->g = g / 255.0f;
		this->b = b / 255.0f;
		this->a = a / 255.0f;
	};
};

class ColorUtil {
public:
	static MC_Color waveColor(int red, int green, int blue, int red2, int green2, int blue2, long index);
	static MC_Color RGBWave(int red, int green, int blue, int red2, int green2, int blue2, long index);
	static MC_Color rainbowColor(float seconds, float saturation, float brightness, long index);
	static MC_Color astolfoRainbow(int yOffset, int yTotal);
	static MC_Color interfaceColor(int index);
};