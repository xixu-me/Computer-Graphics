
// ScanlineRenderingView.h : interface of the CScanlineRenderingView class
//

#pragma once
#include "Line.h"
#include "Fill.h"

class CScanlineRenderingView : public CView {
protected: // create from serialization only
	CScanlineRenderingView() noexcept;
	DECLARE_DYNCREATE(CScanlineRenderingView)

	// Attributes
public:
	CScanlineRenderingDoc *GetDocument() const;

	// Operations
public:
	void ReadPoint();
	void DrawGraph();
	void DrawPolygon(CDC *pDC);
	void FillPolygon(CDC *pDC);

	// Overrides
public:
	virtual void OnDraw(CDC *pDC); // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT &cs);
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo *pInfo);
	virtual void OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo);
	virtual void OnEndPrinting(CDC *pDC, CPrintInfo *pInfo);

	// Implementation
public:
	virtual ~CScanlineRenderingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	CP2 P[3];
	CPi2 P1[3];
	CRGB C[3];
	BOOL bFill;

protected:
	// Generated message map functions
protected:
	afx_msg void OnDrawpic();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // debug version in ScanlineRenderingView.cpp
inline CScanlineRenderingDoc *CScanlineRenderingView::GetDocument() const { return reinterpret_cast<CScanlineRenderingDoc *>(m_pDocument); }
#endif
