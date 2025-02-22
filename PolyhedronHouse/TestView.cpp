// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"
#define Round(d) int(floor(d + 0.5)) // 四舍五入宏定义
#define PI 3.1415926				 // 圆周率
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
//{{AFX_MSG_MAP(CTestView)
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDBLCLK()
ON_WM_TIMER()
ON_WM_KEYDOWN()
ON_WM_ERASEBKGND()
ON_COMMAND(IDM_PLAY, OnPlay)
ON_UPDATE_COMMAND_UI(IDM_PLAY, OnUpdatePlay)
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView() {
	// TODO: add construction code here
	bPlay = FALSE;
	R = 1000.0, d = 800.0, Phi = 90.0, Theta = 0.0; // 视点位于正前方
	Near = 800.0, Far = 1200.0;						// 近剪切面与远剪切面
	Alpha = 0.0;
	Beta = 0.0;
}

CTestView::~CTestView() {
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC *pDC) {
	CTestDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	DoubleBuffer();
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo *pInfo) {
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/) {
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const {
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CTestDoc *CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc *)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::ReadVertex() // 点表
{
	// 顶点的三维坐标(x,y,z),立方体边长为2a
	double a = 150;
	V[0].z = 0;
	V[0].x = 0;
	V[0].y = 0;
	V[1].z = 100;
	V[1].x = 0;
	V[1].y = 0;
	V[2].z = 100;
	V[2].x = 200;
	V[2].y = 0;
	V[3].z = 0;
	V[3].x = 200;
	V[3].y = 0;
	V[4].z = 0;
	V[4].x = 0;
	V[4].y = 100;
	V[5].z = 100;
	V[5].x = 0;
	V[5].y = 100;
	V[6].z = 100;
	V[6].x = 200;
	V[6].y = 100;
	V[7].z = 0;
	V[7].x = 200;
	V[7].y = 100;
	V[8].z = 50;
	V[8].x = 0;
	V[8].y = 150;
	V[9].z = 50;
	V[9].x = 200;
	V[9].y = 150;
}

void CTestView::ReadFace() // 面表
{
	// 面的顶点数、面的顶点索引号与面的颜色
	F[0].SetNum(4);
	F[0].vI[0] = 1;
	F[0].vI[1] = 2;
	F[0].vI[2] = 6;
	F[0].vI[3] = 5; // 前面
	F[1].SetNum(4);
	F[1].vI[0] = 0;
	F[1].vI[1] = 4;
	F[1].vI[2] = 7;
	F[1].vI[3] = 3; // 后面
	F[2].SetNum(5);
	F[2].vI[0] = 0;
	F[2].vI[1] = 1;
	F[2].vI[2] = 5;
	F[2].vI[3] = 8;
	F[2].vI[4] = 4; // 左面
	F[3].SetNum(5);
	F[3].vI[0] = 2;
	F[3].vI[1] = 3;
	F[3].vI[2] = 7;
	F[3].vI[3] = 9;
	F[3].vI[4] = 6; // 右面
	F[4].SetNum(4);
	F[4].vI[0] = 4;
	F[4].vI[1] = 8;
	F[4].vI[2] = 9;
	F[4].vI[3] = 7; // 顶面1
	F[5].SetNum(4);
	F[5].vI[0] = 0;
	F[5].vI[1] = 3;
	F[5].vI[2] = 2;
	F[5].vI[3] = 1; // 底面
	F[6].SetNum(4);
	F[6].vI[0] = 5;
	F[6].vI[1] = 6;
	F[6].vI[2] = 9;
	F[6].vI[3] = 8; // 顶面2

	F[0].fClr = CRGB(0.98, 0.68, 0.48);
	F[1].fClr = CRGB(0.29, 0.58, 0.38);
	F[2].fClr = CRGB(0.69, 0.27, 0.88);
	F[3].fClr = CRGB(0.92, 0.78, 0.68);
	F[4].fClr = CRGB(0.49, 0.38, 0.28);
	F[5].fClr = CRGB(0.89, 0.78, 0.68);
	F[6].fClr = CRGB(0.39, 0.28, 0.18);
}

void CTestView::InitParameter() // 透视变换参数初始化
{
	k[1] = sin(PI * Theta / 180);
	k[2] = sin(PI * Phi / 180);
	k[3] = cos(PI * Theta / 180);
	k[4] = cos(PI * Phi / 180);
	k[5] = k[2] * k[3];
	k[6] = k[2] * k[1];
	k[7] = k[4] * k[3];
	k[8] = k[4] * k[1];
	ViewPoint.x = R * k[6]; // 用户坐标系的视点球坐标
	ViewPoint.y = R * k[4];
	ViewPoint.z = R * k[5];
}

void CTestView::PerProject(CP3 P) // 透视变换
{
	CP3 ViewP;
	ViewP.x = k[3] * P.x - k[1] * P.z; // 观察坐标系三维坐标
	ViewP.y = -k[8] * P.x + k[2] * P.y - k[7] * P.z;
	ViewP.z = -k[6] * P.x - k[4] * P.y - k[5] * P.z + R;
	ScreenP.x = d * ViewP.x / ViewP.z; // 屏幕坐标系三维坐标
	ScreenP.y = Round(d * ViewP.y / ViewP.z);
	ScreenP.z = Far * (1 - Near / ViewP.z) / (Far - Near);
}

void CTestView::DoubleBuffer() // 双缓冲
{
	CDC *pDC = GetDC();
	CRect rect;															// 定义客户区
	GetClientRect(&rect);												// 获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);									// pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(), rect.Height());						// 设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height());					// x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);			// 屏幕中心为原点
	CDC MemDC;															// 内存DC
	CBitmap NewBitmap, *pOldBitmap;										// 内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);										// 建立与屏幕pDC兼容的MemDC
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()); // 创建兼容位图
	pOldBitmap = MemDC.SelectObject(&NewBitmap);						// 将兼容位图选入MemDC
	MemDC.FillSolidRect(&rect, pDC->GetBkColor());						// 按原来背景填充客户区，否则是黑色
	MemDC.SetMapMode(MM_ANISOTROPIC);									// MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(), rect.Height());
	MemDC.SetViewportExt(rect.Width(), -rect.Height());
	MemDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width() / 2, -rect.Height() / 2, rect.Width(), rect.Height(), &MemDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY); // 将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);																											 // 恢复位图
	NewBitmap.DeleteObject();																												 // 删除位图
	ReleaseDC(pDC);																															 // 释放DC
}

void CTestView::DrawObject(CDC *pDC) {
	CPi3 Point[5];							// 透视投影后面的二维顶点数组
	CZBuffer *zbuf = new CZBuffer;			// 申请内存
	zbuf->InitDeepBuffer(1000, 1000, 1000); // 初始化深度缓存器
	for (int nFace = 0; nFace < 7; nFace++) // 面循环
	{
		// 计算视向量和面法向量，判断是否需要绘制
		CP3 p0 = V[F[nFace].vI[0]];
		CP3 p1 = V[F[nFace].vI[1]];
		CP3 p2 = V[F[nFace].vI[2]];

		// 计算视向量
		CVector ViewVector(p0, ViewPoint);
		ViewVector = ViewVector.Normalize();

		// 计算面法向量
		CFace f;
		f.SetFaceNormal(p0, p1, p2);
		f.fNormal.Normalize();

		// 如果视向量和法向量的点积大于等于0，说明面朝向观察者，需要绘制
		if (Dot(ViewVector, f.fNormal) >= 0) {
			for (int nVertex = 0; nVertex < F[nFace].vN; nVertex++) // 顶点循环
			{
				PerProject(V[F[nFace].vI[nVertex]]); // 透视投影
				Point[nVertex] = ScreenP;
				Point[nVertex].c = F[nFace].fClr;
			}
			zbuf->SetPoint(Point, F[nFace].vN); // 设置顶点
			zbuf->CreateBucket();				// 建立桶表
			zbuf->CreateEdge();					// 建立边表
			zbuf->Gouraud(pDC);					// 填充多边形
			zbuf->ClearMemory();				// 内存清理
		}
	}
	delete zbuf; // 释放内存
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) // 鼠标左键函数
{
	// TODO: Add your message handler code here and/or call default
	R = R + 100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDblClk(UINT nFlags, CPoint point) // 鼠标右键函数
{
	// TODO: Add your message handler code here and/or call default
	R = R - 100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnRButtonDblClk(nFlags, point);
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	// TODO: Add your message handler code here and/or call default
	if (!bPlay) {
		switch (nChar) {
		case VK_UP:
			Alpha = -5;
			tran.RotateX(Alpha);
			break;
		case VK_DOWN:
			Alpha = 5;
			tran.RotateX(Alpha);
			break;
		case VK_LEFT:
			Beta = -5;
			tran.RotateY(Beta);
			break;
		case VK_RIGHT:
			Beta = 5;
			tran.RotateY(Beta);
			break;
		default:
			break;
		}
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnTimer(UINT nIDEvent) // 动画时间函数
{
	// TODO: Add your message handler code here and/or call default
	// Phi-=5;//设定步长
	Theta -= 5;
	InitParameter();
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CTestView::OnPlay() // 菜单函数
{
	// TODO: Add your command handler code here
	bPlay = bPlay ? FALSE : TRUE;
	if (bPlay) // 设置定时器
		SetTimer(1, 150, NULL);
	else
		KillTimer(1);
}

void CTestView::OnUpdatePlay(CCmdUI *pCmdUI) // 菜单控制函数
{
	// TODO: Add your command update UI handler code here
	if (bPlay) {
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("停止");
	}
	else {
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("开始");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC *pDC) {
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate() {
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadVertex();
	ReadFace();
	tran.SetMat(V, 8);
	InitParameter();
}
