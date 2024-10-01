#include "pch.h"
#include "Fill.h"
#define ROUND(x) x >= 0 ? (int)(x + 0.5) : (int)(x - 0.5)

CFill::CFill() {
	PNum = 0;
	P = NULL;
	pEdge = NULL;
	pHeadB = NULL;
	pHeadE = NULL;
}

CFill::~CFill() {
	if (P != NULL) {
		delete[] P;
		P = NULL;
	}
	ClearMemory();
}

void CFill::SetPoint(CPi2 *p, int m) {
	P = new CPi2[m];
	for (int i = 0; i < m; i++) {
		P[i] = p[i];
	}
	PNum = m;
}

void CFill::CreateBucket() {
	int yMin, yMax;
	yMin = yMax = P[0].y;
	for (int i = 0; i < PNum; i++) {
		if (P[i].y < yMin) {
			yMin = P[i].y;
		}
		if (P[i].y > yMax) {
			yMax = P[i].y;
		}
	}
	for (int y = yMin; y <= yMax; y++) {
		if (yMin == y) {
			pHeadB = new CBucket;
			pCurrentB = pHeadB;
			pCurrentB->ScanLine = yMin;
			pCurrentB->pET = NULL;
			pCurrentB->pNext = NULL;
		}
		else {
			pCurrentB->pNext = new CBucket;
			pCurrentB = pCurrentB->pNext;
			pCurrentB->ScanLine = y;
			pCurrentB->pET = NULL;
			pCurrentB->pNext = NULL;
		}
	}
}

void CFill::CreateEdge() {
	for (int i = 0; i < PNum; i++) {
		pCurrentB = pHeadB;
		int j = (i + 1) % PNum;
		if (P[i].y < P[j].y) {
			pEdge = new CAET;
			pEdge->x = P[i].x;
			pEdge->yMax = P[j].y;
			pEdge->k = (P[j].x - P[i].x) / (P[j].y - P[i].y);
			pEdge->ps = P[i];
			pEdge->pe = P[j];
			pEdge->pNext = NULL;
			while (pCurrentB->ScanLine != P[i].y)
				pCurrentB = pCurrentB->pNext;
		}
		if (P[j].y < P[i].y) {
			pEdge = new CAET;
			pEdge->x = P[j].x;
			pEdge->yMax = P[i].y;
			pEdge->k = (P[i].x - P[j].x) / (P[i].y - P[j].y);
			pEdge->ps = P[i];
			pEdge->pe = P[j];
			pEdge->pNext = NULL;
			while (pCurrentB->ScanLine != P[j].y)
				pCurrentB = pCurrentB->pNext;
		}
		if (P[i].y != P[j].y) {
			pCurrentE = pCurrentB->pET;
			if (pCurrentE == NULL) {
				pCurrentE = pEdge;
				pCurrentB->pET = pCurrentE;
			}
			else {
				while (pCurrentE->pNext != NULL)
					pCurrentE = pCurrentE->pNext;
				pCurrentE->pNext = pEdge;
			}
		}
	}
}

void CFill::Gouraud(CDC *pDC) {
	CAET *pT1 = NULL, *pT2 = NULL;
	pHeadE = NULL;
	for (pCurrentB = pHeadB; pCurrentB != NULL; pCurrentB = pCurrentB->pNext) {
		for (pCurrentE = pCurrentB->pET; pCurrentE != NULL; pCurrentE = pCurrentE->pNext) {
			pEdge = new CAET;
			pEdge->x = pCurrentE->x;
			pEdge->yMax = pCurrentE->yMax;
			pEdge->k = pCurrentE->k;
			pEdge->ps = pCurrentE->ps;
			pEdge->pe = pCurrentE->pe;
			pEdge->pNext = NULL;
			AddET(pEdge);
		}
		ETOrder();
		pT1 = pHeadE;
		if (pT1 == NULL)
			return;
		while (pCurrentB->ScanLine >= pT1->yMax) {
			CAET *pAETTEmp = pT1;
			pT1 = pT1->pNext;
			delete pAETTEmp;
			pHeadE = pT1;
			if (pHeadE == NULL)
				return;
		}
		if (pT1->pNext != NULL) {
			pT2 = pT1;
			pT1 = pT2->pNext;
		}
		while (pT1 != NULL) {
			if (pCurrentB->ScanLine >= pT1->yMax) {
				CAET *pAETTemp = pT1;
				pT2->pNext = pT1->pNext;
				pT1 = pT2->pNext;
				delete pAETTemp;
			}
			else {
				pT2 = pT1;
				pT1 = pT2->pNext;
			}
		}
		BOOL bInFlag = FALSE;
		double xb, xe;
		CRGB cb, ce, c;
		for (pT1 = pHeadE; pT1 != NULL; pT1 = pT1->pNext) {
			if (FALSE == bInFlag) {
				xb = pT1->x;
				cb = LinearDifferential(pT1->ps.y, pCurrentB->ScanLine, pT1->pe.y, pT1->ps.c, pT1->pe.c);
				bInFlag = TRUE;
			}
			else {
				xe = pT1->x;
				ce = LinearDifferential(pT1->ps.y, pCurrentB->ScanLine, pT1->pe.y, pT1->ps.c, pT1->pe.c);
				for (double x = xb; x < xe; x++) {
					c = LinearDifferential(xb, x, xe, cb, ce);
					pDC->SetPixelV(ROUND(x), pCurrentB->ScanLine, RGB(c.red * 255, c.green * 255, c.blue * 255));
				}
				bInFlag = FALSE;
			}
		}
		for (pT1 = pHeadE; pT1 != NULL; pT1 = pT1->pNext)
			pT1->x = pT1->x + pT1->k;
	}
}

void CFill::AddET(CAET *pNewEdge) {
	CAET *pCE = pHeadE;
	if (pCE == NULL) {
		pHeadE = pNewEdge;
		pCE = pHeadE;
	}
	else {
		while (pCE->pNext != NULL) {
			pCE = pCE->pNext;
		}
		pCE->pNext = pNewEdge;
	}
}

void CFill::ETOrder() {
	CAET *pT1, *pT2;
	int Count = 1;
	pT1 = pHeadE;
	if (pT1 == NULL) {
		return;
	}
	if (pT1->pNext == NULL) {
		return;
	}
	while (pT1->pNext != NULL) {
		Count++;
		pT1 = pT1->pNext;
	}
	for (int i = 0; i < Count - 1; i++) {
		CAET **pPre = &pHeadE;
		pT1 = pHeadE;
		for (int j = 0; j < Count - 1 - i; j++) {
			pT2 = pT1->pNext;

			if ((pT1->x > pT2->x) || ((pT1->x == pT2->x) && (pT1->k > pT2->k))) {
				pT1->pNext = pT2->pNext;
				pT2->pNext = pT1;
				*pPre = pT2;
				pPre = &(pT2->pNext);
			}
			else {
				pPre = &(pT1->pNext);
				pT1 = pT1->pNext;
			}
		}
	}
}

void CFill::ClearMemory() {
	DeleteAETChain(pHeadE);
	CBucket *pBucket = pHeadB;
	while (pBucket != NULL) {
		CBucket *pBucketTemp = pBucket->pNext;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket = pBucketTemp;
	}
	pHeadB = NULL;
	pHeadE = NULL;
}

void CFill::DeleteAETChain(CAET *pAET) {
	while (pAET != NULL) {
		CAET *pAETTemp = pAET->pNext;
		delete pAET;
		pAET = pAETTemp;
	}
}

CRGB CFill::LinearDifferential(double y0, double y, double y1, CRGB c0, CRGB c1) {
	CRGB c;
	double t;
	t = (y - y0) / (y1 - y0);
	c = (1 - t) * c0 + t * c1;
	return c;
}
