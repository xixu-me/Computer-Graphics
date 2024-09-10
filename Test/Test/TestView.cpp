
// TestView.cpp : implementation of the CTestView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Test.h"
#endif

#include "TestDoc.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CTestView construction/destruction

CTestView::CTestView() noexcept {
	// TODO: add construction code here
}

CTestView::~CTestView() {
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT &cs) {
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTestView drawing

void CTestView::OnDraw(CDC * /*pDC*/) {
	CTestDoc *pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

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

// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const {
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext &dc) const {
	CView::Dump(dc);
}

CTestDoc *CTestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc *)m_pDocument;
}
#endif //_DEBUG

// CTestView message handlers
