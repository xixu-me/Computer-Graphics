#pragma once
#include "RGB.h"

class CPi2 {
public:
	CPi2();
	virtual ~CPi2();
	CPi2(double, int);
	double x;
	int y;
	CRGB c;
};
