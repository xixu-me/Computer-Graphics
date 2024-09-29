
// NationalFlagView.cpp : implementation of the CNationalFlagView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NationalFlag.h"
#endif

#include "NationalFlagDoc.h"
#include "NationalFlagView.h"

#define M_PI 3.14159265358979323846
#define ROUND(x) x >= 0 ? (int)(x + 0.5) : (int)(x - 0.5)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CNationalFlagView

IMPLEMENT_DYNCREATE(CNationalFlagView, CView)

BEGIN_MESSAGE_MAP(CNationalFlagView, CView)
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNationalFlagView construction/destruction

CNationalFlagView::CNationalFlagView() noexcept {
	// TODO: add construction code here
}

CNationalFlagView::~CNationalFlagView() {
}

BOOL CNationalFlagView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNationalFlagView drawing

void CNationalFlagView::OnDraw(CDC *pDC) {
	CNationalFlagDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(ROUND(rect.Width() / 2), ROUND(rect.Height() / 2));
	if (rect.Width() > (ROUND(rect.Height() * 1.5)))
		DrawFlag(pDC, CPoint(0, 0), rect.Height());
	else
		DrawFlag(pDC, CPoint(0, 0), ROUND(rect.Width() / 1.5));
}

// CNationalFlagView printing

BOOL CNationalFlagView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNationalFlagView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CNationalFlagView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

// CNationalFlagView diagnostics

#ifdef _DEBUG
void CNationalFlagView::AssertValid() const {
	CView::AssertValid();
}

void CNationalFlagView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CNationalFlagDoc *CNationalFlagView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNationalFlagDoc)));
	return (CNationalFlagDoc *)m_pDocument;
}
#endif //_DEBUG

void CNationalFlagView::DrawWuLine(CDC *pDC, CPoint begin, CPoint end) {
	COLORREF foreColor = RGB(255, 255, 0);
	COLORREF backColor = RGB(238, 28, 37);
	CPoint temp;
	LONG dx = end.x - begin.x, dy = end.y - begin.y;
	int totalSteps = max(abs(dx), abs(dy));
	auto downPixelColor = [&](COLORREF foreColor, double e) -> COLORREF {
		BYTE r = static_cast<BYTE>((GetRValue(backColor) - GetRValue(foreColor)) * e + GetRValue(foreColor));
		BYTE g = static_cast<BYTE>((GetGValue(backColor) - GetGValue(foreColor)) * e + GetGValue(foreColor));
		BYTE b = static_cast<BYTE>((GetBValue(backColor) - GetBValue(foreColor)) * e + GetBValue(foreColor));
		return RGB(r, g, b);
	};
	auto upPixelColor = [&](COLORREF foreColor, double e) -> COLORREF {
		BYTE r = static_cast<BYTE>((GetRValue(backColor) - GetRValue(foreColor)) * (1 - e) + GetRValue(foreColor));
		BYTE g = static_cast<BYTE>((GetGValue(backColor) - GetGValue(foreColor)) * (1 - e) + GetGValue(foreColor));
		BYTE b = static_cast<BYTE>((GetBValue(backColor) - GetBValue(foreColor)) * (1 - e) + GetBValue(foreColor));
		return RGB(r, g, b);
	};
	if (begin.x == end.x) {
		if (begin.y > end.y) {
			temp = begin;
			begin = end;
			end = temp;
		}
		for (int y = begin.y; y < end.y; y++)
			pDC->SetPixelV(begin.x, y, foreColor);
	}
	else {
		double k = (double)dy / dx, e;
		if (k >= 0.0 && k <= 1.0) {
			if (begin.x > end.x) {
				temp = begin;
				begin = end;
				end = temp;
			}
			for (temp = begin, e = k; temp.x < end.x; temp.x++) {
				pDC->SetPixelV(temp.x, temp.y, downPixelColor(foreColor, e));
				pDC->SetPixelV(temp.x, temp.y + 1, upPixelColor(foreColor, e));
				e += k;
				if (e >= 1.0) {
					temp.y++;
					e--;
				}
			}
		}
		else if (k > 1.0) {
			if (begin.y > end.y) {
				temp = begin;
				begin = end;
				end = temp;
			}
			for (temp = begin, e = 1 / k; temp.y < end.y; temp.y++) {
				pDC->SetPixelV(temp.x, temp.y, downPixelColor(foreColor, e));
				pDC->SetPixelV(temp.x + 1, temp.y, upPixelColor(foreColor, e));
				e += 1 / k;
				if (e >= 1.0) {
					temp.x++;
					e--;
				}
			}
		}
		else if (k < 0.0 && k >= -1.0) {
			if (begin.x > end.x) {
				temp = begin;
				begin = end;
				end = temp;
			}
			for (temp = begin, e = -k; temp.x < end.x; temp.x++) {
				pDC->SetPixelV(temp.x, temp.y, downPixelColor(foreColor, e));
				pDC->SetPixelV(temp.x, temp.y - 1, upPixelColor(foreColor, e));
				e -= k;
				if (e >= 1.0) {
					temp.y--;
					e--;
				}
			}
		}
		else {
			if (begin.y < end.y) {
				temp = begin;
				begin = end;
				end = temp;
			}
			for (temp = begin, e = -1 / k; temp.y > end.y; temp.y--) {
				pDC->SetPixelV(temp.x, temp.y, downPixelColor(foreColor, e));
				pDC->SetPixelV(temp.x + 1, temp.y, upPixelColor(foreColor, e));
				e -= 1 / k;
				if (e >= 1.0) {
					temp.x++;
					e--;
				}
			}
		}
	}
}

void CNationalFlagView::DrawFlag(CDC *pDC, CPoint center, int height) {
	int width = ROUND(height * 1.5);
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(238, 28, 37));
	pOldPen = pDC->SelectObject(&NewPen);
	CBrush NewBrush, *pOldBrush;
	NewBrush.CreateSolidBrush(RGB(238, 28, 37));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->Rectangle(ROUND(center.x - width / 2), ROUND(center.y - height / 2), ROUND(center.x + width / 2), ROUND(center.y + height / 2));
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	int cell = ROUND(height / 20);
	DrawStar(pDC, CPoint(-10 * cell, 5 * cell), 0, 3 * cell);
	DrawStar(pDC, CPoint(-5 * cell, 8 * cell), -(atan(3.0 / 5) + M_PI / 10), cell);
	DrawStar(pDC, CPoint(-3 * cell, 6 * cell), -(atan(1.0 / 7) + M_PI / 10), cell);
	DrawStar(pDC, CPoint(-3 * cell, 3 * cell), atan(2.0 / 7) - M_PI / 10, cell);
	DrawStar(pDC, CPoint(-5 * cell, cell), atan(4.0 / 5) - M_PI / 10, cell);
}

void CNationalFlagView::DrawStar(CDC *pDC, CPoint center, double angle, int radius) {
	CPoint points[10];
	for (int i = 0; i < 10; i++) {
		double theta = angle + i * 36.0 * M_PI / 180.0 - M_PI / 2;
		double r = (i % 2 == 0) ? radius : radius * 0.382;
		points[i].x = ROUND(center.x + r * cos(theta));
		points[i].y = ROUND(center.y - r * sin(theta));
	}
	// for (int i = 0; i < 10; i++)
	// 	DrawWuLine(pDC, points[i], points[(i + 1) % 10]);
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	pOldPen = pDC->SelectObject(&NewPen);
	CBrush NewBrush, *pOldBrush;
	NewBrush.CreateSolidBrush(RGB(255, 255, 0));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->BeginPath();
	pDC->MoveTo(points[0]);
	for (int i = 1; i < 10; i++)
		pDC->LineTo(points[i]);
	pDC->LineTo(points[0]);
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

// CNationalFlagView message handlers
