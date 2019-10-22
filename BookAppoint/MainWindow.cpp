// MainWindow.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "MainWindow.h"
#include "afxdialogex.h"


// CMainWindow dialog

IMPLEMENT_DYNAMIC(CMainWindow, CDialog)

CMainWindow::CMainWindow(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BOOKAPPOINT_MAINWINDOW, pParent)
{

}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainWindow, CDialog)
END_MESSAGE_MAP()


// CMainWindow message handlers
