// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"
#include "Vector.h"
#include "ZBuffer.h"
#include "Lighting.h"
#include "Transform.h"

class CTestView : public CView
{
protected: // create from serialization only
	CTestView();
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:
	CTestDoc* GetDocument();

// Operations
public:
	void ReadImage(int);//读入纹理
	void ReadVertex();//读入点表
	void ReadFace();//读入面表
	void ReadGroundVertex();//读入点表
	void ReadGroundFace();//读入面表
	void InitParameter();//参数初始化
	void PerProject(CP3);//透视投影
	void DoubleBuffer();//双缓冲绘图
	void DrawObject(CDC *);//绘制长方体表面
	void DrawGround(CDC* pDC);//绘制地
	void ClearImageMemory();//清理位图内存
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CP3 V[10];//点表
	CFace F[7];//面表
	CP3 VGround[4];//点表
	CFace FGround;//面表
	CZBuffer *zbuf;
	double R,Theta,Phi;//视点在用户坐标系中的球坐标
	double d;//视距
	double Near,Far;//远近剪切面
	double k[9];//运算常量
	CP3 ViewPoint;//视点球坐标
	CPi3 ScreenP;//屏幕坐标系的三维坐标点
	double Alpha,Beta;//x方向旋转α角,y方向旋转β角
	int LightNum;//光源数量
	CLighting *pLight;//光照环境
	CMaterial *pMaterial;//物体材质
	BOOL bPlay;//动画开关
	COLORREF **Image;//二维动态数组
	CTransform tran;//变换对象
	BITMAP bmp;//BITMAP结构体变量
	BYTE *im;
	// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPlay();
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestView.cpp
inline CTestDoc* CTestView::GetDocument()
   { return (CTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
