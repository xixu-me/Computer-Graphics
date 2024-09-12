
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

	// 设置定时器
	SetTimer(1, 1, NULL);

	// 获取客户端矩形区域
	CRect rect;
	GetClientRect(&rect);

	// 创建兼容的内存设备上下文
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// 创建兼容的位图并选择到内存设备上下文中
	CBitmap NewBitmap, *pOldBitmap;
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&NewBitmap);

	// 设置内存设备上下文的映射模式和视口
	memDC.SetMapMode(MM_ANISOTROPIC);
	memDC.SetWindowExt(rect.Width(), rect.Height());
	memDC.SetViewportExt(rect.Width(), -rect.Height());
	memDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	// 画风车底座
	CPen NewPen, *pOldPen;
	CBrush NewBrush, *pOldBrush;
	NewPen.CreatePen(PS_SOLID, 1, RGB(109, 104, 106));
	pOldPen = memDC.SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(109, 104, 106));
	pOldBrush = memDC.SelectObject(&NewBrush);

	int baseWidthTop = 100;
	int baseWidthBottom = 300;
	int baseHeight = 400;
	int bias = 65;
	POINT pt[4]{
		{ -baseWidthTop / 2, bias },
		{ baseWidthTop / 2, bias },
		{ baseWidthBottom / 2, bias - baseHeight },
		{ -baseWidthBottom / 2, bias - baseHeight }
	};
	memDC.Polygon(pt, 4);

	memDC.SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	memDC.SelectObject(pOldPen);
	NewPen.DeleteObject();

	// 画风车叶片
	float fAngle = m_nAngle * PI / 180;
	int R = 300, r = 75, h = 10;

	// 画第一组叶片
	NewPen.CreatePen(PS_SOLID, 1, RGB(0, 82, 217));
	pOldPen = memDC.SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(0, 82, 217));
	pOldBrush = memDC.SelectObject(&NewBrush);

	memDC.BeginPath();
	memDC.MoveTo(0, h);
	memDC.LineTo(R * cos(fAngle), R * sin(fAngle) + h);
	memDC.LineTo(R * cos(fAngle) + r * sin(fAngle), R * sin(fAngle) + h - r * cos(fAngle));
	memDC.LineTo(r * sqrt(2) * cos(fAngle - PI / 4), r * sqrt(2) * sin(fAngle - PI / 4) + h);
	memDC.LineTo(0, h);
	memDC.MoveTo(0, h);
	memDC.LineTo(R * cos(fAngle + PI), R * sin(fAngle + PI) + h);
	memDC.LineTo(R * cos(fAngle + PI) + r * sin(fAngle + PI), R * sin(fAngle + PI) + h - r * cos(fAngle + PI));
	memDC.LineTo(r * sqrt(2) * cos(fAngle + PI - PI / 4), r * sqrt(2) * sin(fAngle + PI - PI / 4) + h);
	memDC.LineTo(0, h);
	memDC.EndPath();
	memDC.FillPath();

	memDC.SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	memDC.SelectObject(pOldPen);
	NewPen.DeleteObject();

	// 画第二组叶片
	NewPen.CreatePen(PS_SOLID, 1, RGB(236, 189, 101));
	pOldPen = memDC.SelectObject(&NewPen);
	NewBrush.CreateSolidBrush(RGB(236, 189, 101));
	pOldBrush = memDC.SelectObject(&NewBrush);

	memDC.BeginPath();
	memDC.MoveTo(0, h);
	memDC.LineTo(R * cos(fAngle + PI / 2), R * sin(fAngle + PI / 2) + h);
	memDC.LineTo(R * cos(fAngle + PI / 2) + r * sin(fAngle + PI / 2), R * sin(fAngle + PI / 2) + h - r * cos(fAngle + PI / 2));
	memDC.LineTo(r * sqrt(2) * cos(fAngle + PI / 2 - PI / 4), r * sqrt(2) * sin(fAngle + PI / 2 - PI / 4) + h);
	memDC.LineTo(0, h);
	memDC.MoveTo(0, h);
	memDC.LineTo(R * cos(fAngle - PI / 2), R * sin(fAngle - PI / 2) + h);
	memDC.LineTo(R * cos(fAngle - PI / 2) + r * sin(fAngle - PI / 2), R * sin(fAngle - PI / 2) + h - r * cos(fAngle - PI / 2));
	memDC.LineTo(r * sqrt(2) * cos(fAngle - PI / 2 - PI / 4), r * sqrt(2) * sin(fAngle - PI / 2 - PI / 4) + h);
	memDC.LineTo(0, h);
	memDC.EndPath();
	memDC.FillPath();

	memDC.SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	memDC.SelectObject(pOldPen);
	NewPen.DeleteObject();

	GetClientRect(&rect);

	// 设置屏幕设备上下文的映射模式和视口
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);

	// 将内存设备上下文中的内容复制到屏幕设备上下文中
	pDC->BitBlt(-rect.Width() / 2, -rect.Height() / 2, rect.Width(), rect.Height(), &memDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY);

	// 清理资源
	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
	memDC.DeleteDC();
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
	// 增加角度
	m_nAngle += 1;

	// 如果角度超过360度，重置为0
	if (m_nAngle >= 360)
		m_nAngle = 0;

	// 使视图无效以触发重绘，但不擦除背景
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}
