
// StraightLineView.cpp : implementation of the CStraightLineView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "StraightLine.h"
#endif

#include "StraightLineDoc.h"
#include "StraightLineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStraightLineView

IMPLEMENT_DYNCREATE(CStraightLineView, CView)

BEGIN_MESSAGE_MAP(CStraightLineView, CView)
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CStraightLineView construction/destruction

CStraightLineView::CStraightLineView() noexcept {
	// TODO: add construction code here
	m_bDraw = FALSE;
}

CStraightLineView::~CStraightLineView() {
}

BOOL CStraightLineView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CStraightLineView drawing

void CStraightLineView::OnDraw(CDC *pDC) {
	CStraightLineDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 获取客户端矩形区域
	GetClientRect(&m_rect);

	// 创建内存设备上下文和兼容位图
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap NewBitmap, *pOldBitmap;
	NewBitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
	pOldBitmap = memDC.SelectObject(&NewBitmap);

	// 设置内存设备上下文的映射模式和视口
	memDC.SetMapMode(MM_ANISOTROPIC);
	memDC.SetWindowExt(m_rect.Width(), m_rect.Height());
	memDC.SetViewportExt(m_rect.Width(), -m_rect.Height());
	memDC.SetViewportOrg(m_rect.Width() / 2, m_rect.Height() / 2);
	m_rect.OffsetRect(-m_rect.Width() / 2, -m_rect.Height() / 2);

	// 填充背景颜色并绘制坐标轴
	memDC.FillSolidRect(&m_rect, RGB(255, 255, 255));
	memDC.MoveTo(-m_rect.Width() / 2, 0);
	memDC.LineTo(m_rect.Width() / 2, 0);
	memDC.LineTo(m_rect.Width() / 2 - 10, -5);
	memDC.MoveTo(m_rect.Width() / 2, 0);
	memDC.LineTo(m_rect.Width() / 2 - 10, 5);
	memDC.MoveTo(0, -m_rect.Height() / 2);
	memDC.LineTo(0, m_rect.Height() / 2);
	memDC.LineTo(-5, m_rect.Height() / 2 - 10);
	memDC.MoveTo(0, m_rect.Height() / 2);
	memDC.LineTo(5, m_rect.Height() / 2 - 10);

	// 绘制坐标轴标签
	memDC.TextOutW(10, 25, _T("O"));
	memDC.TextOutW(m_rect.Width() / 2 - 10, 25, _T("x"));
	memDC.TextOutW(10, m_rect.Height() / 2, _T("y"));

	// 调用Bresenham算法绘制直线
	BresenhamLine(&memDC);

	// 设置显示设备上下文的映射模式和视口
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(m_rect.Width(), m_rect.Height());
	pDC->SetViewportExt(m_rect.Width(), -m_rect.Height());
	pDC->SetViewportOrg(m_rect.Width() / 2, m_rect.Height() / 2);

	// 将内存设备上下文中的位图拷贝到显示设备上下文中
	pDC->BitBlt(-m_rect.Width() / 2, -m_rect.Height() / 2, m_rect.Width(), m_rect.Height(), &memDC, -m_rect.Width() / 2, -m_rect.Height() / 2, SRCCOPY);

	// 恢复旧位图并删除对象
	memDC.SelectObject(pOldBitmap);
	NewBitmap.DeleteObject();
	memDC.DeleteDC();
}

// CStraightLineView printing

BOOL CStraightLineView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CStraightLineView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CStraightLineView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

// CStraightLineView diagnostics

#ifdef _DEBUG
void CStraightLineView::AssertValid() const {
	CView::AssertValid();
}

void CStraightLineView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CStraightLineDoc *CStraightLineView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStraightLineDoc)));
	return (CStraightLineDoc *)m_pDocument;
}
#endif //_DEBUG

// CStraightLineView message handlers

void CStraightLineView::BresenhamLine(CDC *pDC) {
	// 定义起始颜色和结束颜色
	COLORREF startColor = RGB(0, 82, 217);
	COLORREF endColor = RGB(236, 189, 101);

	// 获取起点和终点坐标
	CPoint ptOrigin = m_ptOrigin, ptEnd = m_ptEnd, ptTemp;
	LONG dx = ptEnd.x - ptOrigin.x, dy = ptEnd.y - ptOrigin.y;
	int totalSteps = max(abs(dx), abs(dy));

	// 定义颜色插值函数
	auto interpolateColor = [](COLORREF start, COLORREF end, double factor) -> COLORREF {
		BYTE r = static_cast<BYTE>(GetRValue(start) + factor * (GetRValue(end) - GetRValue(start)));
		BYTE g = static_cast<BYTE>(GetGValue(start) + factor * (GetGValue(end) - GetGValue(start)));
		BYTE b = static_cast<BYTE>(GetBValue(start) + factor * (GetBValue(end) - GetBValue(start)));
		return RGB(r, g, b);
	};

	// 处理垂直线的情况
	if (ptOrigin.x == ptEnd.x) {
		if (ptOrigin.y > ptEnd.y) {
			ptTemp = ptOrigin;
			ptOrigin = ptEnd;
			ptEnd = ptTemp;
		}
		for (int y = ptOrigin.y; y <= ptEnd.y; y++) {
			double factor = (double)(y - ptOrigin.y) / totalSteps;
			COLORREF color = interpolateColor(startColor, endColor, factor);
			pDC->SetPixelV(ptOrigin.x, y, color);
		}
	}
	else {
		// 计算斜率
		DOUBLE k = (DOUBLE)dy / dx;
		DOUBLE d;

		// 处理斜率在[0, 1]之间的情况
		if (k >= 0 && k <= 1) {
			d = 0.5 - k;
			if (ptOrigin.x > ptEnd.x) {
				ptTemp = ptOrigin;
				ptOrigin = ptEnd;
				ptEnd = ptTemp;
			}
			for (ptTemp = ptOrigin; ptTemp.x <= ptEnd.x; ptTemp.x++) {
				double factor = (double)(ptTemp.x - ptOrigin.x) / totalSteps;
				COLORREF color = interpolateColor(startColor, endColor, factor);
				pDC->SetPixelV(ptTemp.x, ptTemp.y, color);
				if (d < 0) {
					d += 1 - k;
					ptTemp.y++;
				}
				else {
					d -= k;
				}
			}
		}
		// 处理斜率大于1的情况
		else if (k > 1) {
			d = 0.5 - 1 / k;
			if (ptOrigin.y > ptEnd.y) {
				ptTemp = ptOrigin;
				ptOrigin = ptEnd;
				ptEnd = ptTemp;
			}
			for (ptTemp = ptOrigin; ptTemp.y <= ptEnd.y; ptTemp.y++) {
				double factor = (double)(ptTemp.y - ptOrigin.y) / totalSteps;
				COLORREF color = interpolateColor(startColor, endColor, factor);
				pDC->SetPixelV(ptTemp.x, ptTemp.y, color);
				if (d < 0) {
					d += 1 - 1 / k;
					ptTemp.x++;
				}
				else {
					d -= 1 / k;
				}
			}
		}
		// 处理斜率在[-1, 0]之间的情况
		else if (k < 0 && k >= -1) {
			d = 0.5 + k;
			if (ptOrigin.x > ptEnd.x) {
				ptTemp = ptOrigin;
				ptOrigin = ptEnd;
				ptEnd = ptTemp;
			}
			for (ptTemp = ptOrigin; ptTemp.x <= ptEnd.x; ptTemp.x++) {
				double factor = (double)(ptTemp.x - ptOrigin.x) / totalSteps;
				COLORREF color = interpolateColor(startColor, endColor, factor);
				pDC->SetPixelV(ptTemp.x, ptTemp.y, color);
				if (d < 0) {
					d += 1 + k;
					ptTemp.y--;
				}
				else {
					d += k;
				}
			}
		}
		// 处理斜率小于-1的情况
		else {
			d = 0.5 + 1 / k;
			if (ptOrigin.y < ptEnd.y) {
				ptTemp = ptOrigin;
				ptOrigin = ptEnd;
				ptEnd = ptTemp;
			}
			for (ptTemp = ptOrigin; ptTemp.y >= ptEnd.y; ptTemp.y--) {
				double factor = (double)(ptOrigin.y - ptTemp.y) / totalSteps;
				COLORREF color = interpolateColor(startColor, endColor, factor);
				pDC->SetPixelV(ptTemp.x, ptTemp.y, color);
				if (d < 0) {
					d += 1 + 1 / k;
					ptTemp.x++;
				}
				else {
					d += 1 / k;
				}
			}
		}
	}
}

void CStraightLineView::OnLButtonDown(UINT nFlags, CPoint point) {
	// 记录起点坐标并转换为视图坐标系
	m_ptOrigin = CPoint(point.x - m_rect.Width() / 2, m_rect.Height() / 2 - point.y);
	// 设置绘制标志为真
	m_bDraw = TRUE;

	CView::OnLButtonDown(nFlags, point);
}

void CStraightLineView::OnMouseMove(UINT nFlags, CPoint point) {
	// 如果正在绘制，更新终点坐标并转换为视图坐标系
	if (m_bDraw) {
		m_ptEnd = CPoint(point.x - m_rect.Width() / 2, m_rect.Height() / 2 - point.y);
		// 使视图无效以触发重绘
		Invalidate(FALSE);
	}

	CView::OnMouseMove(nFlags, point);
}

void CStraightLineView::OnLButtonUp(UINT nFlags, CPoint point) {
	// 设置绘制标志为假
	m_bDraw = FALSE;
	// 更新终点坐标并转换为视图坐标系
	m_ptEnd = CPoint(point.x - m_rect.Width() / 2, m_rect.Height() / 2 - point.y);
	// 使视图无效以触发重绘
	Invalidate(FALSE);

	CView::OnLButtonUp(nFlags, point);
}
