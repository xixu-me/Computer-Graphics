
// ScanlineRenderingView.cpp : implementation of the CScanlineRenderingView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ScanlineRendering.h"
#endif

#include "ScanlineRenderingDoc.h"
#include "ScanlineRenderingView.h"
#include <math.h>
#define Round(x) (int)(x + 0.5)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CScanlineRenderingView

IMPLEMENT_DYNCREATE(CScanlineRenderingView, CView)

BEGIN_MESSAGE_MAP(CScanlineRenderingView, CView)
// Standard printing commands
// ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
// ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
// ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_COMMAND(ID_FILE_PRINT, OnDrawpic)
END_MESSAGE_MAP()

// CScanlineRenderingView construction/destruction

CScanlineRenderingView::CScanlineRenderingView() noexcept {
	// TODO: add construction code here
}

CScanlineRenderingView::~CScanlineRenderingView() {
}

BOOL CScanlineRenderingView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CScanlineRenderingView drawing

void CScanlineRenderingView::OnDraw(CDC * /*pDC*/) {
	CScanlineRenderingDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawGraph();
}

// CScanlineRenderingView printing

BOOL CScanlineRenderingView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CScanlineRenderingView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CScanlineRenderingView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

// CScanlineRenderingView diagnostics

#ifdef _DEBUG
void CScanlineRenderingView::AssertValid() const {
	CView::AssertValid();
}

void CScanlineRenderingView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CScanlineRenderingDoc *CScanlineRenderingView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CScanlineRenderingDoc)));
	return (CScanlineRenderingDoc *)m_pDocument;
}
#endif //_DEBUG

// CScanlineRenderingView message handlers

void CScanlineRenderingView::ReadPoint() {
	P[0].x = 70;
	P[0].y = 130;
	P[0].c = C[0];
	P[1].x = -300;
	P[1].y = -100;
	P[1].c = C[1];
	P[2].x = 300;
	P[2].y = -130;
	P[2].c = C[2];
}

void CScanlineRenderingView::DrawGraph() {
	CRect rect;
	GetClientRect(&rect);
	CDC *pDC = GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	rect.OffsetRect(-rect.Width() / 2, -rect.Height() / 2);
	if (!bFill)
		DrawPolygon(pDC);
	else
		FillPolygon(pDC);
	ReleaseDC(pDC);
}

void CScanlineRenderingView::DrawPolygon(CDC *pDC) {
	CLine *line = new CLine;
	CP2 t;
	for (int i = 0; i < 3; i++)
		if (i == 0) {
			line->MoveTo(pDC, P[i]);
			t = P[i];
		}
		else
			line->LineTo(pDC, P[i]);
	line->LineTo(pDC, t);
	delete line;
}

void CScanlineRenderingView::FillPolygon(CDC *pDC) {
	ReadPoint();
	for (int i = 0; i < 3; i++) {
		P1[i].x = P[i].x;
		P1[i].y = Round(P[i].y);
		P1[i].c = P[i].c;
	}
	CFill *fill = new CFill;
	fill->SetPoint(P1, 3);
	fill->CreateBucket();
	fill->CreateEdge();
	fill->Gouraud(pDC);
	delete fill;
}

void CScanlineRenderingView::OnDrawpic() {
	for (int i = 0; i < 3; i++) {
		COLORREF GetClr = RGB(0, 0, 0);
		CColorDialog ccd(GetClr, CC_SOLIDCOLOR);
		if (IDOK == ccd.DoModal())
			GetClr = ccd.GetColor();
		else
			return;
		C[i] = CRGB(GetRValue(GetClr) / 255.0, GetGValue(GetClr) / 255.0, GetBValue(GetClr) / 255.0);
	}

	bFill = TRUE;
	Invalidate();
}

void CScanlineRenderingView::OnInitialUpdate() {
	CView::OnInitialUpdate();

	bFill = FALSE;
	ReadPoint();
}
