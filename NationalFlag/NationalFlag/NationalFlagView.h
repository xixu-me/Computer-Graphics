
// NationalFlagView.h : interface of the CNationalFlagView class
//

#pragma once

class CNationalFlagView : public CView {
protected: // create from serialization only
	CNationalFlagView() noexcept;
	DECLARE_DYNCREATE(CNationalFlagView)

	// Attributes
public:
	CNationalFlagDoc *GetDocument() const;

	// Operations
public:
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
	virtual ~CNationalFlagView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	void DrawFlag(CDC *pDC, CPoint center, int height);
	void DrawStar(CDC *pDC, CPoint center, double angle, int radius);

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG // debug version in NationalFlagView.cpp
inline CNationalFlagDoc *CNationalFlagView::GetDocument() const { return reinterpret_cast<CNationalFlagDoc *>(m_pDocument); }
#endif
