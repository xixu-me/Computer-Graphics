#pragma once

#include "AET.h"

class CBucket {
public:
	CBucket();
	virtual ~CBucket();

public:
	int ScanLine;
	CAET *pET;
	CBucket *pNext;
};
