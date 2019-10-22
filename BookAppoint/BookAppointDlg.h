#pragma once
#include "afxdtctl.h"
#include "ATLComTime.h"
#include "afxcmn.h"
#include "afxwin.h"


// CBookAppointDlg dialog

class CBookAppointDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBookAppointDlg)

public:
	CBookAppointDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBookAppointDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BOOKAPPOINT };
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
	bool m_bIsListEmpty ;
	bool m_RadioDoctorID;
	int m_RadioPatientID;
	int m_RadioDateInterval;
	int m_RadioTimeInterval;
	int m_iRadioButtonOption ;
	int m_iDateIntervalChoice ;
	int m_iViewAppointmentDoctorID;
	int m_iViewAppointmentPatientID;
	CEdit m_EditViewAppointmentDoctorID;
	CEdit m_EditViewAppointmentPatientID;
	CString m_sSelectDepartment;
	CDateTimeCtrl m_EditViewAppointmentStartTime;
	CDateTimeCtrl m_EditViewAppointmentEndDate;
	CDateTimeCtrl m_EditViewAppointmentStartDate;
	CDateTimeCtrl m_EditViewAppointmentEndTime;
	COleDateTime m_ViewAppointmentStartDate;
	COleDateTime m_ViewAppointmentEndDate;
	COleDateTime m_MonthCalendar;
	COleDateTime m_ViewAppointmentStartTime;
	COleDateTime m_ViewAppointmentEndTime;
	CMonthCalCtrl m_EditMonthCalendar;
	CListCtrl m_EditListControlViewAppointments;
	CComboBox m_EditSelectDepartment;
	CButton m_EditRadioDoctorID;

	virtual BOOL OnInitDialog();
	afx_msg void OnMcnSelchangeMonthcalendarViewAppointments(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedRadioViewAppointmentsDoctorId();
	afx_msg void OnBnClickedRadioViewAppointmentsPatientId();
	afx_msg void OnBnClickedRadioViewAppointmentsDateInterval();
	afx_msg void OnBnClickedCancelAppointment();
	afx_msg void OnBnClickedRadioViewAppointmentsTimeInterval();
	afx_msg void OnBnClickedButtonMarkAsSeen();
	afx_msg void OnBnClickedButtonMarkAsNoShow();
	afx_msg void OnSearchTimeInterval();
	afx_msg void DisplayDefaultAppointments();
	afx_msg void OnBnClickedButtonListDoctors();
	afx_msg void OnBnClickedButtonAddDoctor();
	afx_msg void OnBnClickedRadioViewAppointmentsAll();
	afx_msg void OnBnClickedButtonClearFilter();
	afx_msg void ResetListControl();
	afx_msg void OnRegistration();
	afx_msg void OnModifyUpdate();
	afx_msg void OnBookappointment();
};
