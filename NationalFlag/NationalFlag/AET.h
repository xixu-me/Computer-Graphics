#pragma once

#include "Pi2.h"

class CAET {
public:
	CAET();
	virtual ~CAET();

public:
	double x;
	int yMax;
	double k;
	CPi2 ps;
	CPi2 pe;
	CAET *pNext;
};
