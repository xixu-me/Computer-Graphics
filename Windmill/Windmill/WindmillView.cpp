
// WindmillView.cpp : implementation of the CWindmillView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Windmill.h"
#endif

#include "WindmillDoc.h"
#include "WindmillView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

// CWindmillView

IMPLEMENT_DYNCREATE(CWindmillView, CView)

BEGIN_MESSAGE_MAP(CWindmillView, CView)
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_TIMER()
END_MESSAGE_MAP()

// CWindmillView construction/destruction

CWindmillView::CWindmillView() noexcept {
	// TODO: add construction code here
	m_nAngle = 0;
}

CWindmillView::~CWindmillView() {
}

BOOL CWindmillView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWindmillView drawing

void CWindmillView::OnDraw(CDC *pDC) {
	CWindmillDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	// 设置定时器
	SetTimer(1, 40, NULL);

	// 获取客户端区域并设置坐标系
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	// 定义画笔和画刷
	CPen NewPen, *pOldPen;
	CBrush NewBrush, *pOldBrush;

	// 定义风车叶片和底座的参数
	float fAngle = m_nAngle * PI / 180;
	int R = 300, r = 75, h = 10;
	int baseWidthTop = 100;
	int baseWidthBottom = 300;
	int baseHeight = 400;
	int bias = 65;
	POINT pt[4]{
		{ -baseWidthTop / 2, h + bias },
		{ baseWidthTop / 2, h + bias },
		{ baseWidthBottom / 2, h + bias - baseHeight },
		{ -baseWidthBottom / 2, h + bias - baseHeight }
	};

	// 绘制风车底座
	NewPen.CreatePen(PS_SOLID, 1, RGB(109, 104, 106));
	pOldPen = pDC->SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(109, 104, 106));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->Polygon(pt, 4);
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();

	// 绘制风车叶片（蓝色部分）
	NewPen.CreatePen(PS_SOLID, 500, RGB(0, 82, 217));
	pOldPen = pDC->SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(0, 82, 217));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->BeginPath();
	pDC->MoveTo(0, h);
	pDC->LineTo(R * cos(fAngle), R * sin(fAngle) + h);
	pDC->LineTo(R * cos(fAngle) + r * sin(fAngle), R * sin(fAngle) + h - r * cos(fAngle));
	pDC->LineTo(r * sqrt(2) * cos(fAngle - PI / 4), r * sqrt(2) * sin(fAngle - PI / 4) + h);
	pDC->LineTo(r * cos(fAngle), r * sin(fAngle) + h);
	pDC->MoveTo(0, h);
	pDC->LineTo(R * cos(fAngle + PI), R * sin(fAngle + PI) + h);
	pDC->LineTo(R * cos(fAngle + PI) + r * sin(fAngle + PI), R * sin(fAngle + PI) + h - r * cos(fAngle + PI));
	pDC->LineTo(r * sqrt(2) * cos(fAngle + PI - PI / 4), r * sqrt(2) * sin(fAngle + PI - PI / 4) + h);
	pDC->LineTo(r * cos(fAngle + PI), r * sin(fAngle + PI) + h);
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();

	// 绘制风车叶片（黄色部分）
	NewPen.CreatePen(PS_SOLID, 500, RGB(236, 189, 101));
	pOldPen = pDC->SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(236, 189, 101));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->BeginPath();
	pDC->MoveTo(0, h);
	pDC->LineTo(R * cos(fAngle + PI / 2), R * sin(fAngle + PI / 2) + h);
	pDC->LineTo(R * cos(fAngle + PI / 2) + r * sin(fAngle + PI / 2), R * sin(fAngle + PI / 2) + h - r * cos(fAngle + PI / 2));
	pDC->LineTo(r * sqrt(2) * cos(fAngle + PI / 2 - PI / 4), r * sqrt(2) * sin(fAngle + PI / 2 - PI / 4) + h);
	pDC->LineTo(r * cos(fAngle + PI / 2), r * sin(fAngle + PI / 2) + h);
	pDC->MoveTo(0, h);
	pDC->LineTo(R * cos(fAngle - PI / 2), R * sin(fAngle - PI / 2) + h);
	pDC->LineTo(R * cos(fAngle - PI / 2) + r * sin(fAngle - PI / 2), R * sin(fAngle - PI / 2) + h - r * cos(fAngle - PI / 2));
	pDC->LineTo(r * sqrt(2) * cos(fAngle - PI / 2 - PI / 4), r * sqrt(2) * sin(fAngle - PI / 2 - PI / 4) + h);
	pDC->LineTo(r * cos(fAngle - PI / 2), r * sin(fAngle - PI / 2) + h);
	pDC->EndPath();
	pDC->FillPath();
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

// CWindmillView printing

BOOL CWindmillView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWindmillView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CWindmillView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

// CWindmillView diagnostics

#ifdef _DEBUG
void CWindmillView::AssertValid() const {
	CView::AssertValid();
}

void CWindmillView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CWindmillDoc *CWindmillView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWindmillDoc)));
	return (CWindmillDoc *)m_pDocument;
}
#endif //_DEBUG

// CWindmillView message handlers

void CWindmillView::OnTimer(UINT_PTR nIDEvent) {
	// TODO: Add your message handler code here and/or call default
	m_nAngle += 5;
	if (m_nAngle >= 360)
		m_nAngle = 0;
	Invalidate();

	CView::OnTimer(nIDEvent);
}
