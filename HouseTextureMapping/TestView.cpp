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

CTestView::CTestView()
{
	// TODO: add construction code here
	bPlay = FALSE;
	R = 1000.0, d = 800.0, Phi = 70.0, Theta = 0.0; // 视点位于正前方
	Near = 200.0, Far = 1200.0;						// 近剪切面与远剪切面
	Alpha = 0.0, Beta = 0.0;						// 旋转角
	LightNum = 1;									// 光源个数
	pLight = new CLighting(LightNum);				// 一维光源动态数组
	pLight->Light[0].SetPosition(0, 0, 800);		// 设置光源位置坐标
	for (int i = 0; i < LightNum; i++)
	{
		pLight->Light[i].L_Diffuse = CRGB(0.7, 0.7, 0.7);  // 光源的漫反射颜色
		pLight->Light[i].L_Specular = CRGB(0.5, 0.5, 0.5); // 光源镜面高光颜色
		pLight->Light[i].L_C0 = 1.0;					   // 常数衰减系数
		pLight->Light[i].L_C1 = 0.0000001;				   // 线性衰减系数
		pLight->Light[i].L_C2 = 0.00000001;				   // 二次衰减系数
		pLight->Light[i].L_OnOff = TRUE;				   // 光源开启
	}
	pMaterial = new CMaterial;						  // 一维材质动态数组
	pMaterial->SetAmbient(CRGB(0.247, 0.200, 0.075)); // 材质对环境光光的反射率
	pMaterial->SetDiffuse(CRGB(0.752, 0.606, 0.226)); // 材质对漫反射光的反射率
	pMaterial->SetSpecular(CRGB(1.0, 1.0, 1.0));	  // 材质对镜面反射光的反射率
	pMaterial->SetEmit(CRGB(0.0, 0.0, 0.0));		  // 材质自身发散的颜色
	pMaterial->M_n = 30.0;							  // 高光指数
}

CTestView::~CTestView()
{
	if (pLight != NULL)
	{
		delete pLight;
		pLight = NULL;
	}
	if (pMaterial != NULL)
	{
		delete pMaterial;
		pMaterial = NULL;
	}
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT &cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC *pDC)
{
	CTestDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	DoubleBuffer();
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo *pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC * /*pDC*/, CPrintInfo * /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext &dc) const
{
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
	// double a = 200, b = 200, c = 50; // 长方体边长
	// // 顶点的三维坐标(x,y,z)
	// V[0].x = -a;
	// V[0].y = -b;
	// V[0].z = -c;
	// V[1].x = +a;
	// V[1].y = -b;
	// V[1].z = -c;
	// V[2].x = +a;
	// V[2].y = +b;
	// V[2].z = -c;
	// V[3].x = -a;
	// V[3].y = +b;
	// V[3].z = -c;
	// V[4].x = -a;
	// V[4].y = -b;
	// V[4].z = +c;
	// V[5].x = +a;
	// V[5].y = -b;
	// V[5].z = +c;
	// V[6].x = +a;
	// V[6].y = +b;
	// V[6].z = +c;
	// V[7].x = -a;
	// V[7].y = +b;
	// V[7].z = +c;
	V[0].z = 200;
	V[0].x = -200;
	V[0].y = 0;
	V[1].z = 400;
	V[1].x = -200;
	V[1].y = 0;
	V[2].z = 400;
	V[2].x = 200;
	V[2].y = 0;
	V[3].z = 200;
	V[3].x = 200;
	V[3].y = 0;
	V[4].z = 200;
	V[4].x = -200;
	V[4].y = 200;
	V[5].z = 400;
	V[5].x = -200;
	V[5].y = 200;
	V[6].z = 400;
	V[6].x = 200;
	V[6].y = 200;
	V[7].z = 200;
	V[7].x = 200;
	V[7].y = 200;
	V[8].z = 300;
	V[8].x = -200;
	V[8].y = 300;
	V[9].z = 300;
	V[9].x = 200;
	V[9].y = 300;
}

void CTestView::ReadFace() // 面表
{
	// // 面的边数、面的顶点编号
	// F[0].SetNum(4);
	// F[0].vI[0] = 4;
	// F[0].vI[1] = 5;
	// F[0].vI[2] = 6;
	// F[0].vI[3] = 7; // 前面顶点索引
	// F[0].t[0] = CT2(0, 0);
	// F[0].t[1] = CT2(437, 0);
	// F[0].t[2] = CT2(437, 437);
	// F[0].t[3] = CT2(0, 437); // 前面纹理坐标
	// F[1].SetNum(4);
	// F[1].vI[0] = 0;
	// F[1].vI[1] = 3;
	// F[1].vI[2] = 2;
	// F[1].vI[3] = 1; // 后面顶点索引
	// F[1].t[0] = CT2(437, 0);
	// F[1].t[1] = CT2(437, 437);
	// F[1].t[2] = CT2(0, 437);
	// F[1].t[3] = CT2(0, 0); // 后面纹理坐标
	// F[2].SetNum(4);
	// F[2].vI[0] = 0;
	// F[2].vI[1] = 4;
	// F[2].vI[2] = 7;
	// F[2].vI[3] = 3; // 左面顶点索引
	// F[2].t[0] = CT2(0, 0);
	// F[2].t[1] = CT2(107, 0);
	// F[2].t[2] = CT2(107, 437);
	// F[2].t[3] = CT2(0, 437); // 左面纹理坐标
	// F[3].SetNum(4);
	// F[3].vI[0] = 1;
	// F[3].vI[1] = 2;
	// F[3].vI[2] = 6;
	// F[3].vI[3] = 5; // 右面顶点索引
	// F[3].t[0] = CT2(107, 0);
	// F[3].t[1] = CT2(107, 437);
	// F[3].t[2] = CT2(0, 437);
	// F[3].t[3] = CT2(0, 0); // 右面纹理坐标
	// F[4].SetNum(4);
	// F[4].vI[0] = 2;
	// F[4].vI[1] = 3;
	// F[4].vI[2] = 7;
	// F[4].vI[3] = 6; // 顶面顶点索引
	// F[4].t[0] = CT2(0, 0);
	// F[4].t[1] = CT2(437, 0);
	// F[4].t[2] = CT2(437, 107);
	// F[4].t[3] = CT2(0, 107); // 顶面纹理坐标
	// F[5].SetNum(4);
	// F[5].vI[0] = 0;
	// F[5].vI[1] = 1;
	// F[5].vI[2] = 5;
	// F[5].vI[3] = 4; // 底面顶点索引
	// F[5].t[0] = CT2(0, 0);
	// F[5].t[1] = CT2(437, 0);
	// F[5].t[2] = CT2(437, 107);
	// F[5].t[3] = CT2(0, 107); // 底面纹理坐标
	F[0].SetNum(4);
	F[0].vI[0] = 1;
	F[0].vI[1] = 2;
	F[0].vI[2] = 6;
	F[0].vI[3] = 5; // 前面
	F[0].t[0] = CT2(0, 0);
	F[0].t[1] = CT2(579, 0);
	F[0].t[2] = CT2(579, 325);
	F[0].t[3] = CT2(0, 325); // 前面纹理坐标
	F[1].SetNum(4);
	F[1].vI[0] = 0;
	F[1].vI[1] = 4;
	F[1].vI[2] = 7;
	F[1].vI[3] = 3; // 后面
	F[1].t[0] = CT2(0, 0);
	F[1].t[1] = CT2(0, 289);
	F[1].t[2] = CT2(579, 289);
	F[1].t[3] = CT2(579, 0); // 后面纹理坐标
	F[2].SetNum(5);
	F[2].vI[0] = 0;
	F[2].vI[1] = 1;
	F[2].vI[2] = 5;
	F[2].vI[3] = 8;
	F[2].vI[4] = 4; // 左面
	F[2].t[0] = CT2(0, 0);
	F[2].t[1] = CT2(289, 0);
	F[2].t[2] = CT2(289, 289);
	F[2].t[3] = CT2(145, 434); // 左面纹理坐标
	F[2].t[4] = CT2(0, 289);
	F[3].SetNum(5);
	F[3].vI[0] = 2;
	F[3].vI[1] = 3;
	F[3].vI[2] = 7;
	F[3].vI[3] = 9;
	F[3].vI[4] = 6; // 右面
	F[3].t[0] = CT2(0, 0);
	F[3].t[1] = CT2(289, 0);
	F[3].t[2] = CT2(289, 289);
	F[3].t[3] = CT2(145, 434); // 右面纹理坐标
	F[3].t[4] = CT2(0, 289);
	F[4].SetNum(4);
	F[4].vI[0] = 4;
	F[4].vI[1] = 8;
	F[4].vI[2] = 9;
	F[4].vI[3] = 7; // 顶面1
	F[4].t[0] = CT2(0, 0);
	F[4].t[1] = CT2(0, 385);
	F[4].t[2] = CT2(579, 385);
	F[4].t[3] = CT2(579, 0); // 顶面1纹理坐标
	F[5].SetNum(4);
	F[5].vI[0] = 5;
	F[5].vI[1] = 6;
	F[5].vI[2] = 9;
	F[5].vI[3] = 8; // 顶面2
	F[5].t[0] = CT2(0, 0);
	F[5].t[1] = CT2(579, 0);
	F[5].t[2] = CT2(579, 385);
	F[5].t[3] = CT2(0, 385); // 顶面2纹理坐标
	F[6].SetNum(4);
	F[6].vI[0] = 0;
	F[6].vI[1] = 3;
	F[6].vI[2] = 2;
	F[6].vI[3] = 1; // 底面
}
void CTestView::ReadGroundVertex() // 点表
{
	double a = 500, b = 200, c = 500; // 长方体边长
	// 顶点的三维坐标(x,y,z)
	VGround[0].x = -a;
	VGround[3].y = -b;
	VGround[3].z = +c;
	VGround[1].x = +a;
	VGround[2].y = -b;
	VGround[2].z = +c;
	VGround[2].x = +a;
	VGround[1].y = -b;
	VGround[1].z = -c;
	VGround[3].x = -a;
	VGround[0].y = -b;
	VGround[0].z = -c;
}

void CTestView::ReadGroundFace() // 面表
{
	// 面的边数、面的顶点编号

	FGround.SetNum(4);
	FGround.vI[0] = 0;
	FGround.vI[1] = 1;
	FGround.vI[2] = 2;
	FGround.vI[3] = 3; // 底面顶点索引
	FGround.t[0] = CT2(0, 0);
	FGround.t[1] = CT2(755, 0);
	FGround.t[2] = CT2(755, 497);
	FGround.t[3] = CT2(0, 497); // 底面纹理坐标
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
	ViewPoint.x = R * k[6];
	ViewPoint.y = R * k[4];
	ViewPoint.z = R * k[5];
}

void CTestView::PerProject(CP3 P) // 透视变换
{
	CP3 ViewP;
	ViewP.x = P.x * k[3] - P.z * k[1]; // 观察坐标系三维坐标
	ViewP.y = -P.x * k[8] + P.y * k[2] - P.z * k[7];
	ViewP.z = -P.x * k[6] - P.y * k[4] - P.z * k[5] + R;
	ViewP.c = P.c;
	ScreenP.x = d * ViewP.x / ViewP.z; // 屏幕坐标系三维坐标
	ScreenP.y = Round(d * ViewP.y / ViewP.z);
	ScreenP.z = Far * (1 - Near / ViewP.z) / (Far - Near);
	ScreenP.c = ViewP.c;
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
	MemDC.SetMapMode(MM_ANISOTROPIC);									// MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(), rect.Height());
	MemDC.SetViewportExt(rect.Width(), -rect.Height());
	MemDC.SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	zbuf = new CZBuffer;
	zbuf->InitDeepBuffer(1500, 1500, 1000); // 初始化深度缓冲器
	DrawGround(&MemDC);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width() / 2, -rect.Height() / 2, rect.Width(), rect.Height(), &MemDC, -rect.Width() / 2, -rect.Height() / 2, SRCCOPY); // 将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);																											 // 恢复位图
	NewBitmap.DeleteObject();																												 // 删除位图
	ReleaseDC(pDC);																															 // 释放DC
}
void CTestView::DrawGround(CDC *pDC) // 绘制地
{
	CPi3 Point[4];	// 面的顶点坐标
	CT2 Texture[4]; // 面的纹理坐标
	CVector Normal4[4];

	FGround.SetFaceNormal(VGround[0], VGround[1], VGround[2]);
	FGround.fNormal.Normalize(); // 单位化法矢量

	for (int nVertex = 0; nVertex < 4; nVertex++) // 边循环
	{
		PerProject(VGround[nVertex]);
		Point[nVertex] = ScreenP;
		Normal4[nVertex] = FGround.fNormal;
		Texture[nVertex] = FGround.t[nVertex];
	}
	ReadImage(6);
	zbuf->SetPoint(Point, Normal4, Texture, 4);			   // 初始化
	zbuf->CreateBucket();								   // 创建桶表
	zbuf->CreateEdge();									   // 创建边表
	zbuf->Phong(pDC, ViewPoint, pLight, pMaterial, Image); // 纹理映射
	zbuf->ClearMemory();
	ClearImageMemory();
}

void CTestView::DrawObject(CDC *pDC) // 绘制物体
{
	CPi3 Point[5];	// 面的顶点坐标
	CT2 Texture[5]; // 面的纹理坐标
	CVector Normal4[5];

	for (int nFace = 0; nFace < 7; nFace++)
	{
		CVector ViewVector(V[F[nFace].vI[0]], ViewPoint); // 面的视矢量
		ViewVector = ViewVector.Normalize();			  // 单位化视矢量
		F[nFace].SetFaceNormal(V[F[nFace].vI[0]], V[F[nFace].vI[1]], V[F[nFace].vI[2]]);
		F[nFace].fNormal.Normalize();				// 单位化法矢量
		if (Dot(ViewVector, F[nFace].fNormal) >= 0) // 背面剔除
		{
			for (int nVertex = 0; nVertex < F[nFace].vN; nVertex++) // 边循环
			{
				PerProject(V[F[nFace].vI[nVertex]]);
				Point[nVertex] = ScreenP;
				Normal4[nVertex] = F[nFace].fNormal;
				Texture[nVertex] = F[nFace].t[nVertex];
			}
			ReadImage(nFace);
			zbuf->SetPoint(Point, Normal4, Texture, F[nFace].vN);  // 初始化
			zbuf->CreateBucket();								   // 创建桶表
			zbuf->CreateEdge();									   // 创建边表
			zbuf->Phong(pDC, ViewPoint, pLight, pMaterial, Image); // 纹理映射
			zbuf->ClearMemory();
			ClearImageMemory();
		}
	}
	delete zbuf;
}

void CTestView::ReadImage(int nface) // 读入纹理
{
	int n1;
	BYTE Texture[] = {IDB_FRONTWALL, IDB_REARWALL, IDB_LEFTWALL, IDB_RIGHTWALL, IDB_FRONTROOF, IDB_REARROOF, IDB_GROUND};
	CBitmap NewBitmap;
	NewBitmap.LoadBitmap(Texture[nface]); // 调入DDB位图
	NewBitmap.GetBitmap(&bmp);			  // 将CBitmap的信息保存到Bitmap结构体中
	int nbytesize = bmp.bmWidthBytes * bmp.bmHeight;
	im = new BYTE[nbytesize];
	NewBitmap.GetBitmapBits(nbytesize, (LPVOID)im);
	Image = new COLORREF *[bmp.bmHeight];
	for (int n1 = 0; n1 < bmp.bmHeight; n1++)
		Image[n1] = new COLORREF[bmp.bmWidth];
	for (n1 = 0; n1 < bmp.bmHeight; n1++)
	{
		for (int n2 = 0; n2 < bmp.bmWidth; n2++)
		{
			int pos = n1 * bmp.bmWidthBytes + 4 * n2; // 颜色分量位置
			n1 = bmp.bmHeight - 1 - n1;				  // 位图从左下角向右上角绘制
			Image[n1][n2] = RGB(im[pos + 2], im[pos + 1], im[pos]);
		}
	}
	delete[] im;
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CP3 center(0, 150, 300); // Center point of the cabin
	
	switch (nChar)
	{
	case VK_UP:
		tran.RotateX(Alpha, center);
		break;
	case VK_DOWN:
		tran.RotateX(-Alpha, center);
		break;
	case VK_LEFT:
		tran.RotateY(Beta, center);
		break;
	case VK_RIGHT:
		tran.RotateY(-Beta, center);
		break;
	}
	Invalidate(FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnPlay()
{
	// TODO: Add your command handler code here
	bPlay = bPlay ? FALSE : TRUE;
	if (bPlay) // 设置定时器
		SetTimer(1, 150, NULL);
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent)
{
	Alpha = 5;
	Beta = 5;
	CP3 center(0, 150, 300);
	tran.RotateY(Beta, center);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CTestView::OnUpdatePlay(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (bPlay)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("停止");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("开始");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC *pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadVertex();
	ReadFace();
	ReadGroundVertex();
	ReadGroundFace();
	tran.SetMat(V, 10);
	InitParameter();
}

void CTestView::ClearImageMemory()
{
	for (int n = 0; n < bmp.bmHeight; n++) // 注意撤销次序,先列后行,与设置相反
	{
		delete[] Image[n];
		Image[n] = NULL;
	}
	delete[] Image;
	Image = NULL;
}
