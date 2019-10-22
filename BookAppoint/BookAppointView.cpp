
// BookAppointView.cpp : implementation of the CBookAppointView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BookAppoint.h"
#endif

#include "BookAppointDoc.h"
#include "BookAppointView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBookAppointView

IMPLEMENT_DYNCREATE(CBookAppointView, CView)

BEGIN_MESSAGE_MAP(CBookAppointView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBookAppointView construction/destruction

CBookAppointView::CBookAppointView()
{
	// TODO: add construction code here

}

CBookAppointView::~CBookAppointView()
{
}

BOOL CBookAppointView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBookAppointView drawing

void CBookAppointView::OnDraw(CDC* /*pDC*/)
{
	CBookAppointDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CBookAppointView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBookAppointView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBookAppointView diagnostics

#ifdef _DEBUG
void CBookAppointView::AssertValid() const
{
	CView::AssertValid();
}

void CBookAppointView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBookAppointDoc* CBookAppointView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBookAppointDoc)));
	return (CBookAppointDoc*)m_pDocument;
}
#endif //_DEBUG


// CBookAppointView message handlers
