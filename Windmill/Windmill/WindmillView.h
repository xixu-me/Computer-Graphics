
// WindmillView.h : interface of the CWindmillView class
//

#pragma once


class CWindmillView : public CView
{
protected: // create from serialization only
	CWindmillView() noexcept;
	DECLARE_DYNCREATE(CWindmillView)

// Attributes
public:
	CWindmillDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWindmillView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	int m_nAngle;
};

#ifndef _DEBUG  // debug version in WindmillView.cpp
inline CWindmillDoc* CWindmillView::GetDocument() const
   { return reinterpret_cast<CWindmillDoc*>(m_pDocument); }
#endif

