
// StraightLineView.h : interface of the CStraightLineView class
//

#pragma once

class CStraightLineView : public CView {
protected: // create from serialization only
	CStraightLineView() noexcept;
	DECLARE_DYNCREATE(CStraightLineView)

	// Attributes
public:
	CStraightLineDoc *GetDocument() const;

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
	virtual ~CStraightLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

protected:
	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	void BresenhamLine(CDC *pDC);
	CRect m_rect;
	CPoint m_ptOrigin, m_ptEnd;
	BOOL m_bDraw;

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG // debug version in StraightLineView.cpp
inline CStraightLineDoc *CStraightLineView::GetDocument() const { return reinterpret_cast<CStraightLineDoc *>(m_pDocument); }
#endif
