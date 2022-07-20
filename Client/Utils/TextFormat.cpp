#include "TextFormat.h"
#include <math.h>
#include <cstdarg>
#include <stdio.h>
#include <string>

char BASE_CHARACTER[2];
char BLACK[4];
char DARK_BLUE[4];
char DARK_GREEN[4];
char DARK_AQUA[4];
char DARK_RED[4];
char DARK_PURPLE[4];
char GOLD[4];
char GRAY[4];
char DARK_GRAY[4];
char BLUE[4];
char GREEN[4];
char AQUA[4];
char RED[4];
char LIGHT_PURPLE[4];
char YELLOW[4];
char WHITE[4];

char OBFUSCATED[4];
char BOLD[4];
char STRIKETHROUGH[4];
char UNDERLINE[4];
char ITALIC[4];
char RESET[4];

const char* Merge(const char* a1, ...) {
	va_list a2;
	va_start(a2, a1);

	auto length = _vscprintf(a1, a2) + 1;
	if (length >= 500) return "";

	char message[300];
	vsprintf_s(message, 300, a1, a2);
	std::string msg(message);

	va_end(a2);
	return msg.c_str();
}

void initMC_Color(char* variable, char color) {
	variable[0] = BASE_CHARACTER[0];
	variable[1] = BASE_CHARACTER[1];
	variable[2] = color;
	variable[3] = 0x0;
}

void init() {
	BASE_CHARACTER[0] = (char)(0xC2);
	BASE_CHARACTER[1] = (char)(0xA7);
	initMC_Color(BLACK, '0');
	initMC_Color(DARK_BLUE, '1');
	initMC_Color(DARK_GREEN, '2');
	initMC_Color(DARK_AQUA, '3');
	initMC_Color(DARK_RED, '4');
	initMC_Color(DARK_PURPLE, '5');
	initMC_Color(GOLD, '6');
	initMC_Color(GRAY, '7');
	initMC_Color(DARK_GRAY, '8');
	initMC_Color(BLUE, '9');
	initMC_Color(GREEN, 'a');
	initMC_Color(AQUA, 'b');
	initMC_Color(RED, 'c');
	initMC_Color(LIGHT_PURPLE, 'd');
	initMC_Color(YELLOW, 'e');
	initMC_Color(WHITE, 'f');

	initMC_Color(OBFUSCATED, 'k');
	initMC_Color(BOLD, 'l');
	initMC_Color(STRIKETHROUGH, 'm');
	initMC_Color(UNDERLINE, 'n');
	initMC_Color(ITALIC, 'o');
	initMC_Color(RESET, 'r');
}
