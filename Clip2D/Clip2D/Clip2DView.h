
// Clip2DView.h : interface of the CClip2DView class
//

#pragma once

#include "P2.h"

class CClip2DView : public CView {
protected: // create from serialization only
	CClip2DView() noexcept;
	DECLARE_DYNCREATE(CClip2DView)

	// Attributes
public:
	CClip2DDoc *GetDocument() const;

	// Operations
public:
	void DoubleBuffer();
	void DrawRect(CDC *pDC, int nscale);
	CP2 Convert(CPoint point);
	void Diamond(CDC *pDC, BOOL bClip);
	int ZoomX(int x);
	int ZoomY(int y);
	BOOL LBLineClip();
	BOOL ClipTest(double, double, double &, double &);

	// Overrides
public:
	virtual void OnDraw(CDC *pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	// Implementation
public:
	virtual ~CClip2DView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	CP2 P[2];
	CP2 *V;
	UINT nScale;
	CP2 nRCenter;
	double nRHHeight, nRHWidth;

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // debug version in Clip2DView.cpp
inline CClip2DDoc *CClip2DView::GetDocument() const { return reinterpret_cast<CClip2DDoc *>(m_pDocument); }
#endif
