#pragma once
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxwin.h"
#include "afxcmn.h"


// CBookAppointmentDlg dialog

class CBookAppointmentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBookAppointmentDlg)

public:
	CBookAppointmentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBookAppointmentDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKAPPOINTMENT_DLG };
#endif

private:
	CString m_sSqlString;
	CString m_sDriver;
	CString m_sDsn;
	CString m_sDSN;
	CString m_sFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_iPatientID;
	int m_iDoctorID;
	CDateTimeCtrl m_EditAppointmentDate;
	COleDateTime m_AppointmentDate;
	CDateTimeCtrl m_EditAppointmentStartTime;
	COleDateTime m_AppointmentStartTime;
	CDateTimeCtrl m_EditAppointmentEndTime;
	COleDateTime m_AppointmentEndTime;
	CComboBox m_EditSelectDepartment;
	CString m_sSelectDepartment;
	CEdit m_EditDoctorID;
	CEdit m_EditPatientID;
	CEdit m_EditSubjectLine;
	CString m_sSubjectLine;
	CEdit m_EditAppointmentLocation;
	CString m_sAppointmentLocation;
	CListCtrl m_EditBookAppointmentListCtrl;

	virtual BOOL OnInitDialog();
	afx_msg void ResetListControl();
	afx_msg void OnBnClickedButtonListDoctors();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedAddDoctorId();

};
