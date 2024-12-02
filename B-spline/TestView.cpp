// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "math.h"					 //��ѧͷ�ļ�
#define Round(d) int(floor(d + 0.5)) // ��������궨��
#include "TestDoc.h"
#include "TestView.h"

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
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_COMMAND(IDM_DRAWPIC, OnDrawpic)
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
	m_AbleToLeftBtn = FALSE;
	m_AbleToMove = FALSE;
	m_i = -1;

	P[0] = CPoint(900, 150);
	P[1] = CPoint(960, 300);
	P[2] = CPoint(1050, 450);
	P[3] = CPoint(900, 600);
	P[4] = CPoint(750, 450);
	P[5] = CPoint(840, 300);
	P[6] = CPoint(900, 150);
}

CTestView::~CTestView()
{
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
	CRect rect;															// ����ͻ���
	GetClientRect(&rect);												// ��ÿͻ����Ĵ�С
	CDC MemDC;															// �ڴ�DC
	CBitmap NewBitmap, *pOldBitmap;										// �ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);										// ��������ĻpDC���ݵ�MemDC
	NewBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()); // ��������λͼ
	pOldBitmap = MemDC.SelectObject(&NewBitmap);						// ������λͼѡ��MemDC
	MemDC.FillSolidRect(rect, pDC->GetBkColor());						// ��ԭ���������ͻ����������Ǻ�ɫ
	CPen NewPen, *pOldPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	pOldPen = MemDC.SelectObject(&NewPen);
	MemDC.MoveTo(P[0]);
	MemDC.Ellipse(P[0].x - 2, P[0].y - 2, P[0].x + 2, P[0].y + 2); // ���ƿ��ƶ���ζ���
	for (int i = 1; i < 7; i++)
	{
		MemDC.LineTo(P[i]);
		MemDC.Ellipse(P[i].x - 2, P[i].y - 2, P[i].x + 2, P[i].y + 2);
	}
	if (m_i != -1)
	{
		CString str;
		str.Format("x=%d,y=%d", P[m_i].x, P[m_i].y);
		MemDC.TextOut(P[m_i].x + 5, P[m_i].y + 5, str);
	}
	B3Curves(P, &MemDC);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY); // ���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldPen);
	NewPen.DeleteObject();
	MemDC.SelectObject(pOldBitmap); // �ָ�λͼ
	NewBitmap.DeleteObject();		// ɾ��λͼ
	MemDC.DeleteDC();				// ɾ��MemDC
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
void CTestView::B3Curves(CPoint q[], CDC *pDC)
{
	CPoint p;
	double F03, F13, F23, F33;

	// �������
	p.x = Round((q[0].x + 4.0 * q[1].x + q[2].x) / 6.0);
	p.y = Round((q[0].y + 4.0 * q[1].y + q[2].y) / 6.0);
	pDC->MoveTo(p);

	// ʹ�ú�ɫ����
	CPen NewPen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen *pOldPen = pDC->SelectObject(&NewPen);

	// ����B�������ߵ�
	for (int i = 0; i < 7; i++)
	{
		for (double t = 0; t <= 1.0; t += 0.05)
		{
			// B����������
			F03 = (-t * t * t + 3 * t * t - 3 * t + 1) / 6.0;
			F13 = (3 * t * t * t - 6 * t * t + 4) / 6.0;
			F23 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6.0;
			F33 = t * t * t / 6.0;

			// ���������ϵĵ�
			int j = i % 7; // ѭ������ȷ����β����
			int j1 = (i + 1) % 7;
			int j2 = (i + 2) % 7;
			int j3 = (i + 3) % 7;

			p.x = Round(q[j].x * F03 + q[j1].x * F13 + q[j2].x * F23 + q[j3].x * F33);
			p.y = Round(q[j].y * F03 + q[j1].y * F13 + q[j2].y * F23 + q[j3].y * F33);

			pDC->LineTo(p);
		}
	}

	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) // ����ƶ�����
{
	// TODO: Add your message handler code here and/or call default
	if (TRUE == m_AbleToMove)
		P[m_i] = point;
	m_i = -1;
	int i;
	for (i = 0; i < 7; i++)
	{
		if ((point.x - P[i].x) * (point.x - P[i].x) + (point.y - P[i].y) * (point.y - P[i].y) < 50)
		{
			m_i = i;
			m_AbleToLeftBtn = TRUE;
			SetCursor(LoadCursor(NULL, IDC_SIZEALL)); // �ı�Ϊʮ�ּ�ͷ���
			break;
		}
	}
	if (10 == i)
	{
		m_i = -1;
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) // ���������º���
{
	// TODO: Add your message handler code here and/or call default
	if (m_AbleToLeftBtn == TRUE)
		m_AbleToMove = TRUE;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) // ������������
{
	// TODO: Add your message handler code here and/or call default
	m_AbleToLeftBtn = FALSE;
	m_AbleToMove = FALSE;
	m_i = 0;
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::OnDrawpic()
{
	// TODO: Add your command handler code here
	MessageBox("��ֱ���϶����ƶ���ζ���۲�Ч����", "��Ϣ", MB_ICONINFORMATION | MB_OK);
}
