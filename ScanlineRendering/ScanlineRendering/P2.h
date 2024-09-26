#pragma once
#include "RGB.h"

class CP2 {
public:
	CP2();
	virtual ~CP2();
	CP2(double, double);

public:
	double x;
	double y;
	CRGB c;
};
