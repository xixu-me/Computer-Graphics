
// Clip2DView.cpp : implementation of the CClip2DView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Clip2D.h"
#endif

#include "Clip2DDoc.h"
#include "Clip2DView.h"

#define Round(x) ((int)(x + 0.5))
#define PI 3.1415926

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClip2DView

IMPLEMENT_DYNCREATE(CClip2DView, CView)

BEGIN_MESSAGE_MAP(CClip2DView, CView)
ON_WM_MOUSEMOVE()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CClip2DView construction/destruction

CClip2DView::CClip2DView() noexcept {
	// TODO: add construction code here
	nRHWidth = 100;
	nRHHeight = 100;
	nScale = 2;
}

CClip2DView::~CClip2DView() {
}

BOOL CClip2DView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CClip2DView drawing

void CClip2DView::OnDraw(CDC * /*pDC*/) {
	CClip2DDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	DoubleBuffer();
}

// CClip2DView printing

BOOL CClip2DView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CClip2DView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CClip2DView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

// CClip2DView diagnostics

#ifdef _DEBUG
void CClip2DView::AssertValid() const {
	CView::AssertValid();
}

void CClip2DView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CClip2DDoc *CClip2DView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClip2DDoc)));
	return (CClip2DDoc *)m_pDocument;
}
#endif //_DEBUG

void CClip2DView::DoubleBuffer() {
	CRect rect;
	GetClientRect(&rect);
	CDC *pDC = GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	CDC MemDC;
	CBitmap NewBitmap, *pOldBitmap;
	MemDC.CreateCompatibleDC(pDC);
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = MemDC.SelectObject(&NewBitmap);
	MemDC.FillSolidRect(&rect, pDC->GetBkColor());
	MemDC.SetMapMode(MM_ANISOTROPIC);
	MemDC.SetWindowExt(rect.Width(), rect.Height());
	MemDC.SetViewportExt(rect.Width(), -rect.Height());
	MemDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	Diamond(&MemDC, FALSE);
	DrawRect(&MemDC, nScale);
	Diamond(&MemDC, TRUE);

	pDC->BitBlt(-rect.Width() / 2, -rect.Height() / 2, rect.Width(), rect.Height(), &MemDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
	ReleaseDC(pDC);
}

void CClip2DView::DrawRect(CDC *pDC, int nScale) {
	pDC->Rectangle(Round(nRCenter.x - nRHWidth), Round(nRCenter.y + nRHHeight),
		Round(nRCenter.x + nRHWidth), Round(nRCenter.y - nRHHeight));
}

void CClip2DView::OnMouseMove(UINT nFlags, CPoint point) {
	// TODO: Add your message handler code here and/or call default
	nRCenter = Convert(point);
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CClip2DView::LBLineClip() {
	double tmax, tmin, dx, dy;
	dx = P[1].x - P[0].x;
	dy = P[1].y - P[0].y;
	tmax = 0.0, tmin = 1.0;
	double wxl = nRCenter.x - nRHWidth / nScale;
	double wxr = nRCenter.x + nRHWidth / nScale;
	double wyb = nRCenter.y - nRHHeight / nScale;
	double wyt = nRCenter.y + nRHHeight / nScale;
	if (ClipTest(-dx, P[0].x - wxl, tmax, tmin)) {
		if (ClipTest(dx, wxr - P[0].x, tmax, tmin)) {
			if (ClipTest(-dy, P[0].y - wyb, tmax, tmin)) {
				if (ClipTest(dy, wyt - P[0].y, tmax, tmin)) {
					if (tmin < 1.0) {
						P[1].x = P[0].x + tmin * dx;
						P[1].y = P[0].y + tmin * dy;
					}
					if (tmax > 0.0) {
						P[0].x = P[0].x + tmax * dx;
						P[0].y = P[0].y + tmax * dy;
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

BOOL CClip2DView::ClipTest(double u, double v, double &tmax, double &tmin) {
	double t;
	BOOL ReturnValue = TRUE;
	if (u < 0.0) {
		t = v / u;
		if (t > tmin)
			ReturnValue = FALSE;
		else if (t > tmax)
			tmax = t;
	}
	else {
		if (u > 0.0) {
			t = v / u;
			if (t < tmax)
				ReturnValue = FALSE;
			else if (t < tmin)
				tmin = t;
		}
		else {
			if (v < 0.0)
				ReturnValue = FALSE;
		}
	}
	return (ReturnValue);
}

CP2 CClip2DView::Convert(CPoint point) {
	CRect rect;
	GetClientRect(&rect);
	CP2 ptemp;
	ptemp.x = point.x - rect.Width() / 2;
	ptemp.y = rect.Height() / 2 - point.y;
	return ptemp;
}

BOOL CClip2DView::OnEraseBkgnd(CDC *pDC) {
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CClip2DView::Diamond(CDC *pDC, BOOL bclip) {
	CPen NewPen, *pOldPen;
	if (bclip)
		NewPen.CreatePen(PS_SOLID, nScale, RGB(0, 0, 0));
	else
		NewPen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&NewPen);
	int numHexagons = 30;
	double baseRadius = 10;
	double rotationAngle = -PI / 196;

	for (int h = 0; h < numHexagons; h++) {
		double r = baseRadius * (h + 1);
		int n = 6;
		V = new CP2[n];
		double thta = 2 * PI / n;

		for (int i = 0; i < n; i++) {
			double angle = i * thta + h * rotationAngle;
			V[i].x = r * cos(angle);
			V[i].y = r * sin(angle);
		}

		for (int i = 0; i < n; i++) {
			int j = (i + 1) % n;
			if (!bclip) {
				pDC->MoveTo(Round(V[i].x), Round(V[i].y));
				pDC->LineTo(Round(V[j].x), Round(V[j].y));
			}
			else {
				P[0] = V[i];
				P[1] = V[j];
				if (LBLineClip()) {
					pDC->MoveTo(ZoomX(Round(P[0].x)), ZoomY(Round(P[0].y)));
					pDC->LineTo(ZoomX(Round(P[1].x)), ZoomY(Round(P[1].y)));
				}
			}
		}

		delete[] V;
	}
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

int CClip2DView::ZoomX(int x) {
	return Round((x - nRCenter.x) * nScale + nRCenter.x);
}

int CClip2DView::ZoomY(int y) {
	return Round((y - nRCenter.y) * nScale + nRCenter.y);
}
