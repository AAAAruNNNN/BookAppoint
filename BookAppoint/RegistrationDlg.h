#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"


// CRegistrationDlg dialog

class CRegistrationDlg : public CDialog
{
	DECLARE_DYNAMIC(CRegistrationDlg)
private:
	CEdit m_EditPatientName;
	CString m_sPatientName;
	CDateTimeCtrl m_EditDOB;
	COleDateTime m_DOB;
	CComboBox m_EditGender;
	CString m_sGender;
	CComboBox m_EditCountryCode;
	CString m_sCountryCode;
	CEdit m_EditContactNumber;
	CString m_sContactNumber;
	CEdit m_EditAddress;
	CString m_sAddress;
	
	afx_msg void OnBnClickedConfirm();
	virtual BOOL OnInitDialog();

public:
	CRegistrationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRegistrationDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


};
