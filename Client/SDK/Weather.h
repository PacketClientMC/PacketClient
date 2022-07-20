#pragma once

class Weather {
private:
    char pad_0x8[0x44]; //0x8
public:
    float fogLevel; //0x4C
};