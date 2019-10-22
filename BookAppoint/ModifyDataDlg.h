#pragma once
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxwin.h"
#include "afxcmn.h"


// CModifyDataDlg dialog

class CModifyDataDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModifyDataDlg)

public:
	CModifyDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModifyDataDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKAPPOINT_MODIFY_UPDATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ResetListControl();

	DECLARE_MESSAGE_MAP()
public:
	int iRadioButtonOption ;
	CString m_sSearchModifyCountryCode;
	CListCtrl m_EditSearchModifyListControl;
	CDateTimeCtrl m_EditSearchModifyDOB;
	COleDateTime m_SearchModifyDOB;
	CComboBox m_EditSearchModifyCountryCode;
	CString m_sSearchModifyContactCode;
	CEdit m_EditSearchModifyContactNumber;
	CString m_sSearchModifyContactNumber;
	CEdit m_EditSearchModifyPatientName;
	CString m_sSearchModifyPatientName;

	afx_msg void OnBnClickedButtonSearchModifyReset();
	afx_msg void OnBnClickedButtonSearchModifySearch();
	afx_msg void OnBnClickedButtonSearchModifyEdit();
	afx_msg void OnBnClickedRadioSearchModifyDob();
	afx_msg void OnBnClickedRadioSearchModifyContactNumber();
	afx_msg void OnBnClickedRadioSearchModifyName();
	afx_msg void OnBnClickedRadioSearchModifyAll();
	virtual BOOL OnInitDialog();
};
