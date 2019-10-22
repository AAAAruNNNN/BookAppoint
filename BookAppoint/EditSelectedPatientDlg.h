#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"


// CEditSelectedPatientDlg dialog

class CEditSelectedPatientDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEditSelectedPatientDlg)

private:
	CString sPatientID;
	CEdit m_EditDetailsPatientName;
	CDateTimeCtrl m_EditDetailsDOB;
	COleDateTime m_EditDetailsDateOfBirth;
	CComboBox m_EditDetailsGender;
	CString m_sEditDetailsGender;
	CComboBox m_EditDetailsCountryCode;
	CString m_sEditDetailsCountryCode;
	CString m_sEditDetailsContactNumber;
	CEdit m_EditDetailsAddress;
	CString m_sEditDetailsAddress;
	CEdit m_EditDetailsContactNumber;
	CString m_sEditDetailsPatientName;
public:
	CEditSelectedPatientDlg(CWnd* pParent = NULL);   // standard constructor
	CEditSelectedPatientDlg(CString PID,CString PName, CString PDOB, CString PContactNumber, CString PGender,CString PAddress,CWnd* pParent = NULL);
	virtual ~CEditSelectedPatientDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKAPPOINT_MODIFY_UPDATE_ONCLICKEDEDITBUTTON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConfirm();
	virtual BOOL OnInitDialog();
};
