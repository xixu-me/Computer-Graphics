#pragma once

#include "Bucket.h"

class CFill {
public:
	CFill();
	virtual ~CFill();
	void SetPoint(CPi2 *p, int);
	void CreateBucket();
	void CreateEdge();
	void AddET(CAET *);
	void ETOrder();
	void Gouraud(CDC *);
	void ClearMemory();
	void DeleteAETChain(CAET *pAET);

protected:
	int PNum;
	CPi2 *P;
	CAET *pHeadE, *pCurrentE, *pEdge;
	CBucket *pHeadB, *pCurrentB;

public:
	CRGB LinearDifferential(double y0, double y, double y1, CRGB c0, CRGB c1);
};
