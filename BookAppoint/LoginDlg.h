#pragma once
#include "afxwin.h"


// CLoginDlg dialog

class CLoginDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoginDlg)
private:
	CEdit m_EditUsername;
	CString m_Username;
	CString m_Password;
	afx_msg void OnBnClickedLogin();

public:
	CLoginDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLoginDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKAPPOINT_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_EditPassword;
	virtual BOOL OnInitDialog();
};
