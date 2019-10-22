// BookAppointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "BookAppointDlg.h"
#include "afxdialogex.h"
#include "RegistrationDlg.h"
#include "ModifyDataDlg.h"
#include "BookAppointmentDlg.h"
#include "odbcinst.h"
#include "afxdb.h"


// CBookAppointDlg dialog

IMPLEMENT_DYNAMIC(CBookAppointDlg, CDialogEx)

CBookAppointDlg::CBookAppointDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKAPPOINT, pParent)
	, m_MonthCalendar(COleDateTime::GetCurrentTime())
	, m_ViewAppointmentStartDate(COleDateTime::GetCurrentTime())
	, m_ViewAppointmentEndDate(COleDateTime::GetCurrentTime())
	, m_iViewAppointmentDoctorID(0)
	, m_iViewAppointmentPatientID(0)
	, m_ViewAppointmentStartTime(COleDateTime::GetCurrentTime())
	, m_ViewAppointmentEndTime(COleDateTime::GetCurrentTime())
	, m_sSelectDepartment(_T(""))
	, m_RadioDoctorID(false)
	, m_RadioPatientID(0)
	, m_RadioDateInterval(0)
	, m_RadioTimeInterval(0)
{
	m_sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	m_sDSN = L"BookAppointAccess";
	m_sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	m_bIsListEmpty = true;
	m_iRadioButtonOption = 0;
	m_iDateIntervalChoice = 0;
}

CBookAppointDlg::~CBookAppointDlg()
{
}

void CBookAppointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONTHCALENDAR_VIEW_APPOINTMENTS, m_EditMonthCalendar);
	DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR_VIEW_APPOINTMENTS, m_MonthCalendar);
	DDX_Control(pDX, IDC_LIST_VIEW_APPOINTMENTS, m_EditListControlViewAppointments);
	DDX_Control(pDX, IDC_EDIT_VIEW_APPOINTMENTS_DOCTOR_ID, m_EditViewAppointmentDoctorID);
	DDX_Control(pDX, IDC_EDIT_VIEW_APPOINTMENTS_PATIENT_ID, m_EditViewAppointmentPatientID);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_START_DATE, m_ViewAppointmentStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_END_DATE, m_ViewAppointmentEndDate);
	DDX_Text(pDX, IDC_EDIT_VIEW_APPOINTMENTS_DOCTOR_ID, m_iViewAppointmentDoctorID);
	DDX_Text(pDX, IDC_EDIT_VIEW_APPOINTMENTS_PATIENT_ID, m_iViewAppointmentPatientID);
	DDX_Control(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_END_DATE, m_EditViewAppointmentEndDate);
	DDX_Control(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_START_DATE, m_EditViewAppointmentStartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_START_TIME, m_ViewAppointmentStartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_END_TIME, m_EditViewAppointmentEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_END_TIME, m_ViewAppointmentEndTime);
	DDX_Control(pDX, IDC_COMBO_LIST_DEPARTMENT, m_EditSelectDepartment);
	DDX_CBString(pDX, IDC_COMBO_LIST_DEPARTMENT, m_sSelectDepartment);
	DDX_Control(pDX, IDC_DATETIMEPICKER_VIEW_APPOINTMENTS_START_TIME, m_EditViewAppointmentStartTime);
}


BEGIN_MESSAGE_MAP(CBookAppointDlg, CDialogEx)
	ON_COMMAND(ID_REGISTRATION, &CBookAppointDlg::OnRegistration)
	ON_COMMAND(ID_MODIFY_UPDATE, &CBookAppointDlg::OnModifyUpdate)
	ON_COMMAND(ID_BOOKAPPOINTMENT, &CBookAppointDlg::OnBookappointment)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR_VIEW_APPOINTMENTS, &CBookAppointDlg::OnMcnSelchangeMonthcalendarViewAppointments)
	ON_BN_CLICKED(IDSEARCH, &CBookAppointDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_RADIO_VIEW_APPOINTMENTS_DOCTOR_ID, &CBookAppointDlg::OnBnClickedRadioViewAppointmentsDoctorId)
	ON_BN_CLICKED(IDC_RADIO_VIEW_APPOINTMENTS_PATIENT_ID, &CBookAppointDlg::OnBnClickedRadioViewAppointmentsPatientId)
	ON_BN_CLICKED(IDC_RADIO_VIEW_APPOINTMENTS_DATE_INTERVAL, &CBookAppointDlg::OnBnClickedRadioViewAppointmentsDateInterval)
	ON_BN_CLICKED(IDC_CANCEL_APPOINTMENT, &CBookAppointDlg::OnBnClickedCancelAppointment)
	ON_BN_CLICKED(IDC_RADIO_VIEW_APPOINTMENTS_TIME_INTERVAL, &CBookAppointDlg::OnBnClickedRadioViewAppointmentsTimeInterval)
	ON_BN_CLICKED(IDC_BUTTON_MARK_AS_SEEN, &CBookAppointDlg::OnBnClickedButtonMarkAsSeen)
	ON_BN_CLICKED(IDC_BUTTON_MARK_AS_NO_SHOW, &CBookAppointDlg::OnBnClickedButtonMarkAsNoShow)
	ON_BN_CLICKED(IDC_BUTTON_LIST_DOCTORS, &CBookAppointDlg::OnBnClickedButtonListDoctors)
	ON_BN_CLICKED(IDC_BUTTON_ADD_DOCTOR, &CBookAppointDlg::OnBnClickedButtonAddDoctor)
	ON_BN_CLICKED(IDC_RADIO_VIEW_APPOINTMENTS_All, &CBookAppointDlg::OnBnClickedRadioViewAppointmentsAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_FILTER, &CBookAppointDlg::OnBnClickedButtonClearFilter)
END_MESSAGE_MAP()


// CBookAppointDlg message handlers




BOOL CBookAppointDlg::OnInitDialog()
{
	// TODO: initialization Code
	CDialogEx::OnInitDialog();
	ShowWindow(SW_SHOWMAXIMIZED);
	DisplayDefaultAppointments();
	CDatabase database;
	CString sDepartmentName;
	// Build ODBC connection string
	TRY
	{
		m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
		CRecordset recsetGetDepartment(&database);
		m_sSqlString.Format(L"SELECT * FROM departments;");
		database.Open(NULL, false, false, m_sDsn);

		recsetGetDepartment.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		while (!recsetGetDepartment.IsEOF())
		{
			// Copy each column into a variable
			recsetGetDepartment.GetFieldValue(L"Department_Name", sDepartmentName);
			m_EditSelectDepartment.AddString(sDepartmentName);
			// goto next record
			recsetGetDepartment.MoveNext();
		}
		//		m_EditSelectDepartment.SetWindowPos(NULL, 0, 0, 0, 5, SWP_NOMOVE | SWP_NOZORDER);
		// Close the database
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CBookAppointDlg::DisplayDefaultAppointments()
{
	UpdateData();
	CDatabase database;
	int iRec = 0;
	CString sAppointmentID, sDoctorID, sDoctorName, sPatientID, sPatientName, sSubjectLine, sAppointmentStatus, sAppointmentDate, sAppointmentStartTime, sAppointmentEndTime, sAppointmentLocation;
	CString sViewAppointmentDate = m_MonthCalendar.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	COleDateTime appointmentDate, appointmentStartTime, appointmentEndTime;
	m_bIsListEmpty = true;


	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Appointment_Date= #%s#;", sViewAppointmentDate);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		ResetListControl();

		ListView_SetExtendedListViewStyle(m_EditListControlViewAppointments, LVS_EX_GRIDLINES);
		m_EditListControlViewAppointments.InsertColumn(0, L"Appointment ID", LVCFMT_LEFT, -1, 0);
		m_EditListControlViewAppointments.InsertColumn(1, L"Doctor ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(2, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(3, L"Patient ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(4, L"Patient Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(5, L"Subject Line", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(6, L"Appointment Date", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(7, L"Appointment Start Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(8, L"Appointment End Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(9, L"Appointment Status", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(10, L"Location", LVCFMT_LEFT, -1, 1);


		m_EditListControlViewAppointments.SetColumnWidth(0, 100);
		m_EditListControlViewAppointments.SetColumnWidth(1, 100);
		m_EditListControlViewAppointments.SetColumnWidth(2, 200);
		m_EditListControlViewAppointments.SetColumnWidth(3, 100);
		m_EditListControlViewAppointments.SetColumnWidth(4, 200);
		m_EditListControlViewAppointments.SetColumnWidth(5, 200);
		m_EditListControlViewAppointments.SetColumnWidth(6, 100);
		m_EditListControlViewAppointments.SetColumnWidth(7, 200);
		m_EditListControlViewAppointments.SetColumnWidth(8, 200);
		m_EditListControlViewAppointments.SetColumnWidth(9, 200);
		m_EditListControlViewAppointments.SetColumnWidth(10, 100);


		while (!recset.IsEOF())
		{
			// Copy each column into a variable

			recset.GetFieldValue(L"Appointment_ID", sAppointmentID);
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Patient_ID", sPatientID);
			recset.GetFieldValue(L"Patient_Name", sPatientName);
			recset.GetFieldValue(L"Subject_Line", sSubjectLine);
			recset.GetFieldValue(L"Appointment_Date", sAppointmentDate);
			recset.GetFieldValue(L"Appointment_Start", sAppointmentStartTime);
			recset.GetFieldValue(L"Appointment_End", sAppointmentEndTime);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);
			recset.GetFieldValue(L"Location", sAppointmentLocation);

			appointmentDate.ParseDateTime(sAppointmentDate);
			appointmentStartTime.ParseDateTime(sAppointmentStartTime);
			appointmentEndTime.ParseDateTime(sAppointmentEndTime);

			sAppointmentDate = appointmentDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
			sAppointmentStartTime = appointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
			sAppointmentEndTime = appointmentEndTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);

			// Insert values into the list control
			iRec = m_EditListControlViewAppointments.InsertItem(0, sAppointmentID, 0);
			m_EditListControlViewAppointments.SetItemText(0, 1, sDoctorID);
			m_EditListControlViewAppointments.SetItemText(0, 2, sDoctorName);
			m_EditListControlViewAppointments.SetItemText(0, 3, sPatientID);
			m_EditListControlViewAppointments.SetItemText(0, 4, sPatientName);
			m_EditListControlViewAppointments.SetItemText(0, 5, sSubjectLine);
			m_EditListControlViewAppointments.SetItemText(0, 6, sAppointmentDate);
			m_EditListControlViewAppointments.SetItemText(0, 7, sAppointmentStartTime);
			m_EditListControlViewAppointments.SetItemText(0, 8, sAppointmentEndTime);
			m_EditListControlViewAppointments.SetItemText(0, 9, sAppointmentStatus);
			m_EditListControlViewAppointments.SetItemText(0, 10, sAppointmentLocation);

			m_bIsListEmpty = false;

			recset.MoveNext();

		}
		m_EditListControlViewAppointments.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditListControlViewAppointments.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

		if (m_bIsListEmpty)
		{
			AfxMessageBox(L"No appointments found");
		}

		// Close the database
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;


	UpdateData(FALSE);
}


void CBookAppointDlg::ResetListControl()
{
	int iNbrOfColumns;
	m_EditListControlViewAppointments.DeleteAllItems();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_EditListControlViewAppointments.GetDlgItem(0);
	if (pHeader)
	{
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--)
	{
		m_EditListControlViewAppointments.DeleteColumn(i);
	}
}


void CBookAppointDlg::OnRegistration()
{
	// TODO: Add your command handler code here
	CRegistrationDlg regDlg;
	regDlg.DoModal();
}



void CBookAppointDlg::OnModifyUpdate()
{
	// TODO: Add your command handler code here
	CModifyDataDlg modDlg;
	modDlg.DoModal();
}


void CBookAppointDlg::OnBookappointment()
{
	// TODO: Add your command handler code here
	CBookAppointmentDlg bookAppointmentDlg;
	bookAppointmentDlg.DoModal();
}


void CBookAppointDlg::OnMcnSelchangeMonthcalendarViewAppointments(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMSELCHANGE pSelChange = reinterpret_cast<LPNMSELCHANGE>(pNMHDR);
	UpdateData();
	CDatabase database;
	int iRec = 0;
	CString sAppointmentID,sDoctorID, sDoctorName, sPatientID, sPatientName, sSubjectLine, sAppointmentStatus, sAppointmentDate, sAppointmentStartTime, sAppointmentEndTime, sAppointmentLocation;
	CString sViewAppointmentDate = m_MonthCalendar.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	COleDateTime appointmentDate, appointmentStartTime, appointmentEndTime;
	m_bIsListEmpty = true;

	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Appointment_Date= #%s#;", sViewAppointmentDate);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		ResetListControl();

		ListView_SetExtendedListViewStyle(m_EditListControlViewAppointments, LVS_EX_GRIDLINES);
		m_EditListControlViewAppointments.InsertColumn(0, L"Appointment ID", LVCFMT_LEFT, -1, 0);
		m_EditListControlViewAppointments.InsertColumn(1, L"Doctor ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(2, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(3, L"Patient ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(4, L"Patient Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(5, L"Subject Line", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(6, L"Appointment Date", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(7, L"Appointment Start Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(8, L"Appointment End Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(9, L"Appointment Status", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(10, L"Location", LVCFMT_LEFT, -1, 1);


		m_EditListControlViewAppointments.SetColumnWidth(0, 100);
		m_EditListControlViewAppointments.SetColumnWidth(1, 100);
		m_EditListControlViewAppointments.SetColumnWidth(2, 200);
		m_EditListControlViewAppointments.SetColumnWidth(3, 100);
		m_EditListControlViewAppointments.SetColumnWidth(4, 200);
		m_EditListControlViewAppointments.SetColumnWidth(5, 200);
		m_EditListControlViewAppointments.SetColumnWidth(6, 100);
		m_EditListControlViewAppointments.SetColumnWidth(7, 200);
		m_EditListControlViewAppointments.SetColumnWidth(8, 200);
		m_EditListControlViewAppointments.SetColumnWidth(9, 200);
		m_EditListControlViewAppointments.SetColumnWidth(10, 100);


		while (!recset.IsEOF())
		{
			// Copy each column into a variable
			recset.GetFieldValue(L"Appointment_ID", sAppointmentID);
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Patient_ID", sPatientID);
			recset.GetFieldValue(L"Patient_Name", sPatientName);
			recset.GetFieldValue(L"Subject_Line", sSubjectLine);
			recset.GetFieldValue(L"Appointment_Date", sAppointmentDate);
			recset.GetFieldValue(L"Appointment_Start", sAppointmentStartTime);
			recset.GetFieldValue(L"Appointment_End", sAppointmentEndTime);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);
			recset.GetFieldValue(L"Location", sAppointmentLocation);

			appointmentDate.ParseDateTime(sAppointmentDate);
			appointmentStartTime.ParseDateTime(sAppointmentStartTime);
			appointmentEndTime.ParseDateTime(sAppointmentEndTime);

			sAppointmentDate = appointmentDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
			sAppointmentStartTime = appointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
			sAppointmentEndTime = appointmentEndTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);

			// Insert values into the list control
			iRec = m_EditListControlViewAppointments.InsertItem(0, sAppointmentID, 0);
			m_EditListControlViewAppointments.SetItemText(0, 1, sDoctorID);
			m_EditListControlViewAppointments.SetItemText(0, 2, sDoctorName);
			m_EditListControlViewAppointments.SetItemText(0, 3, sPatientID);
			m_EditListControlViewAppointments.SetItemText(0, 4, sPatientName);
			m_EditListControlViewAppointments.SetItemText(0, 5, sSubjectLine);
			m_EditListControlViewAppointments.SetItemText(0, 6, sAppointmentDate);
			m_EditListControlViewAppointments.SetItemText(0, 7, sAppointmentStartTime);
			m_EditListControlViewAppointments.SetItemText(0, 8, sAppointmentEndTime);
			m_EditListControlViewAppointments.SetItemText(0, 9, sAppointmentStatus);
			m_EditListControlViewAppointments.SetItemText(0, 10, sAppointmentLocation);

			m_bIsListEmpty = false;

			recset.MoveNext();

		}
		m_EditListControlViewAppointments.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditListControlViewAppointments.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

		if (m_bIsListEmpty)
		{
			AfxMessageBox(L"No appointments found");
		}


		// Close the database
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;


	UpdateData(FALSE);
	*pResult = 0;
}


void CBookAppointDlg::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString sViewAppointmentDate = m_MonthCalendar.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	CString test = m_ViewAppointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
	COleDateTime currDate;
	currDate = COleDateTime::GetCurrentTime();
	m_bIsListEmpty = true;
	switch (m_iRadioButtonOption)
	{
	
	case 0:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID;");
		break;
	case 1:
		if (m_iViewAppointmentDoctorID == 0)
		{
			AfxMessageBox(L"You must provide a Doctor ID to Search");
			this->m_EditViewAppointmentDoctorID.SetFocus();
			return;
		}


		if (m_ViewAppointmentStartDate.GetYear() == m_ViewAppointmentEndDate.GetYear() && m_ViewAppointmentStartDate.GetMonth() == m_ViewAppointmentEndDate.GetMonth() && m_ViewAppointmentStartDate.GetDay() == m_ViewAppointmentEndDate.GetDay() && m_ViewAppointmentStartTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentStartTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute() && m_ViewAppointmentEndTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentEndTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute())
		{
			m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
				"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
				"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
				"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
				"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
				"patients.Patient_ID = appointments.Patient_ID AND appointments.Doctor_ID= %d;", m_iViewAppointmentDoctorID);

		}
		else if(m_ViewAppointmentStartTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentStartTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute())
		{
			if (m_ViewAppointmentStartDate > m_ViewAppointmentEndDate)
			{
				AfxMessageBox(L"Appointment end date cannot be before start date.");
				this->m_EditViewAppointmentStartDate.SetFocus();
				return;
			}
			m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
				"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
				"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
				"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
				"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
				"patients.Patient_ID = appointments.Patient_ID AND appointments.Doctor_ID= %d AND (appointments.Appointment_Date BETWEEN #%s# AND #%s#);", m_iViewAppointmentDoctorID, m_ViewAppointmentStartDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY), m_ViewAppointmentEndDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY));

		}
		else if (m_ViewAppointmentStartDate.GetYear() == m_ViewAppointmentEndDate.GetYear() && m_ViewAppointmentStartDate.GetMonth() == m_ViewAppointmentEndDate.GetMonth() && m_ViewAppointmentStartDate.GetDay() == m_ViewAppointmentEndDate.GetDay() )
		{
			if (m_ViewAppointmentStartTime > m_ViewAppointmentEndTime)
			{
				AfxMessageBox(L"Appointment end time cannot be before start time.");
				this->m_EditViewAppointmentEndTime.SetFocus();
				return;
			}
			m_iDateIntervalChoice = 3;
			OnSearchTimeInterval();
			return;
		}
		else
		{
			if (m_ViewAppointmentStartDate > m_ViewAppointmentEndDate)
			{
				AfxMessageBox(L"Appointment end date cannot be before start date.");
				this->m_EditViewAppointmentStartDate.SetFocus();
				return;
			}
			if (m_ViewAppointmentStartTime > m_ViewAppointmentEndTime)
			{
				AfxMessageBox(L"Appointment end time cannot be before start time.");
				this->m_EditViewAppointmentEndTime.SetFocus();
				return;
			}
			m_iDateIntervalChoice = 2;
			OnSearchTimeInterval();
			return;

		}
		break;
	case 2:
		if (!m_iViewAppointmentPatientID)
		{
			AfxMessageBox(L"You must provide a Patient ID to Search");
			this->m_EditViewAppointmentPatientID.SetFocus();
			return;
		}
		if (m_ViewAppointmentStartDate.GetYear() == m_ViewAppointmentEndDate.GetYear() && m_ViewAppointmentStartDate.GetMonth() == m_ViewAppointmentEndDate.GetMonth() && m_ViewAppointmentStartDate.GetDay() == m_ViewAppointmentEndDate.GetDay() && m_ViewAppointmentStartTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentStartTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute() && m_ViewAppointmentEndTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentEndTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute())
		{
			m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
				"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
				"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
				"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
				"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
				"patients.Patient_ID = appointments.Patient_ID AND appointments.Patient_ID= %d;", m_iViewAppointmentPatientID);

		}
		else if (m_ViewAppointmentStartTime.GetHour() == m_ViewAppointmentEndTime.GetHour() && m_ViewAppointmentStartTime.GetMinute() == m_ViewAppointmentEndTime.GetMinute())
		{
			if (m_ViewAppointmentStartDate > m_ViewAppointmentEndDate)
			{
				AfxMessageBox(L"Appointment end date cannot be before start date.");
				this->m_EditViewAppointmentStartDate.SetFocus();
				return;
			}
			m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
				"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
				"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
				"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
				"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
				"patients.Patient_ID = appointments.Patient_ID AND appointments.Patient_ID= %d AND (appointments.Appointment_Date BETWEEN #%s# AND #%s#);", m_iViewAppointmentDoctorID, m_ViewAppointmentStartDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY), m_ViewAppointmentEndDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY));

		}
		else if (m_ViewAppointmentStartDate.GetYear() == m_ViewAppointmentEndDate.GetYear() && m_ViewAppointmentStartDate.GetMonth() == m_ViewAppointmentEndDate.GetMonth() && m_ViewAppointmentStartDate.GetDay() == m_ViewAppointmentEndDate.GetDay())
		{
			if (m_ViewAppointmentStartTime > m_ViewAppointmentEndTime)
			{
				AfxMessageBox(L"Appointment end time cannot be before start time.");
				this->m_EditViewAppointmentEndTime.SetFocus();
				return;
			}
			m_iDateIntervalChoice = 4;
			OnSearchTimeInterval();
			return;
		}
		else
		{
			if (m_ViewAppointmentStartDate > m_ViewAppointmentEndDate)
			{
				AfxMessageBox(L"Appointment end date cannot be before start date.");
				this->m_EditViewAppointmentStartDate.SetFocus();
				return;
			}
			if (m_ViewAppointmentStartTime > m_ViewAppointmentEndTime)
			{
				AfxMessageBox(L"Appointment end time cannot be before start time.");
				this->m_EditViewAppointmentEndTime.SetFocus();
				return;
			}
			m_iDateIntervalChoice = 5;
			OnSearchTimeInterval();
			return;
		}

		break;
	case 3:
		if (m_ViewAppointmentStartDate > m_ViewAppointmentEndDate)
		{
			AfxMessageBox(L"Appointment end date cannot be before start date.");
			this->m_EditViewAppointmentStartDate.SetFocus();
			return;
		}
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND (appointments.Appointment_Date BETWEEN #%s# AND #%s#);", m_ViewAppointmentStartDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY), m_ViewAppointmentEndDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY));
		break;
	case 4:
		if (m_ViewAppointmentStartTime > m_ViewAppointmentEndTime)
		{
			AfxMessageBox(L"Appointment end time cannot be before start time.");
			this->m_EditViewAppointmentEndTime.SetFocus();
			return;
		}
		m_iDateIntervalChoice = 1;
		OnSearchTimeInterval();
		return;
	default:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
		"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
		"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
		"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
		"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Appointment_Date = #%s#;", sViewAppointmentDate);
		break;
	}
	CDatabase database;
	int iRec = 0;
	CString sAppointmentID,sDoctorID, sDoctorName, sPatientID, sPatientName, sSubjectLine, sAppointmentStatus, sAppointmentDate, sAppointmentStartTime, sAppointmentEndTime, sAppointmentLocation;
	COleDateTime appointmentDate, appointmentStartTime, appointmentEndTime;

	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		ResetListControl();

		ListView_SetExtendedListViewStyle(m_EditListControlViewAppointments, LVS_EX_GRIDLINES);
		m_EditListControlViewAppointments.InsertColumn(0, L"Appointment ID", LVCFMT_LEFT, -1, 0);
		m_EditListControlViewAppointments.InsertColumn(1, L"Doctor ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(2, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(3, L"Patient ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(4, L"Patient Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(5, L"Subject Line", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(6, L"Appointment Date", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(7, L"Appointment Start Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(8, L"Appointment End Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(9, L"Appointment Status", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(10, L"Location", LVCFMT_LEFT, -1, 1);


		m_EditListControlViewAppointments.SetColumnWidth(0, 100);
		m_EditListControlViewAppointments.SetColumnWidth(1, 100);
		m_EditListControlViewAppointments.SetColumnWidth(2, 200);
		m_EditListControlViewAppointments.SetColumnWidth(3, 100);
		m_EditListControlViewAppointments.SetColumnWidth(4, 200);
		m_EditListControlViewAppointments.SetColumnWidth(5, 200);
		m_EditListControlViewAppointments.SetColumnWidth(6, 100);
		m_EditListControlViewAppointments.SetColumnWidth(7, 200);
		m_EditListControlViewAppointments.SetColumnWidth(8, 200);
		m_EditListControlViewAppointments.SetColumnWidth(9, 200);
		m_EditListControlViewAppointments.SetColumnWidth(10, 100);


		while (!recset.IsEOF())
		{
			// Copy each column into a variable
			recset.GetFieldValue(L"Appointment_ID", sAppointmentID);
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Patient_ID", sPatientID);
			recset.GetFieldValue(L"Patient_Name", sPatientName);
			recset.GetFieldValue(L"Subject_Line", sSubjectLine);
			recset.GetFieldValue(L"Appointment_Date", sAppointmentDate);
			recset.GetFieldValue(L"Appointment_Start", sAppointmentStartTime);
			recset.GetFieldValue(L"Appointment_End", sAppointmentEndTime);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);
			recset.GetFieldValue(L"Location", sAppointmentLocation);

			appointmentDate.ParseDateTime(sAppointmentDate);
			appointmentStartTime.ParseDateTime(sAppointmentStartTime);
			appointmentEndTime.ParseDateTime(sAppointmentEndTime);

			sAppointmentDate = appointmentDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
			sAppointmentStartTime = appointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
			sAppointmentEndTime = appointmentEndTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);

			// Insert values into the list control
			iRec = m_EditListControlViewAppointments.InsertItem(0, sAppointmentID, 0);
			m_EditListControlViewAppointments.SetItemText(0, 1, sDoctorID);
			m_EditListControlViewAppointments.SetItemText(0, 2, sDoctorName);
			m_EditListControlViewAppointments.SetItemText(0, 3, sPatientID);
			m_EditListControlViewAppointments.SetItemText(0, 4, sPatientName);
			m_EditListControlViewAppointments.SetItemText(0, 5, sSubjectLine);
			m_EditListControlViewAppointments.SetItemText(0, 6, sAppointmentDate);
			m_EditListControlViewAppointments.SetItemText(0, 7, sAppointmentStartTime);
			m_EditListControlViewAppointments.SetItemText(0, 8, sAppointmentEndTime);
			m_EditListControlViewAppointments.SetItemText(0, 9, sAppointmentStatus);
			m_EditListControlViewAppointments.SetItemText(0, 10, sAppointmentLocation);

			m_bIsListEmpty = false;

			recset.MoveNext();

		}
		m_EditListControlViewAppointments.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditListControlViewAppointments.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

		if (m_bIsListEmpty)
		{
			AfxMessageBox(L"No appointments found");
		}


		// Close the database
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	UpdateData(FALSE);
}

void CBookAppointDlg::OnSearchTimeInterval()
{
	UpdateData();
	CDatabase database;
	int iRec = 0;
	CString sAppointmentID, sDoctorID, sDoctorName, sPatientID, sPatientName, sSubjectLine, sAppointmentStatus, sAppointmentDate, sAppointmentStartTime, sAppointmentEndTime, sAppointmentLocation;
	CString sViewAppointmentDate = m_MonthCalendar.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	COleDateTime appointmentDate, appointmentStartTime, appointmentEndTime;
	m_bIsListEmpty = true;

	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);
	switch (m_iDateIntervalChoice)
	{
	case 1:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND (appointments.Appointment_date = #%s#);", sViewAppointmentDate);

		break;
	case 2:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Doctor_ID= %d AND (appointments.Appointment_Date BETWEEN #%s# AND #%s#);", m_iViewAppointmentDoctorID, m_ViewAppointmentStartDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY), m_ViewAppointmentEndDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY));

		break;
	case 3:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Doctor_ID= %d AND (appointments.Appointment_date = #%s#);", m_iViewAppointmentDoctorID, sViewAppointmentDate);
		break;
	case 4:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Patient_ID= %d AND (appointments.Appointment_date = #%s#);", m_iViewAppointmentPatientID, sViewAppointmentDate);

		break;
	case 5:
		m_sSqlString.Format(L"SELECT appointments.Appointment_ID,appointments.Doctor_ID,doctors.Doctor_Name,"
			"appointments.Patient_ID,patients.Patient_Name,appointments.Subject_Line,"
			"appointments.Appointment_Date,appointments.Appointment_Start,appointments.Appointment_End,"
			"appointments.Appointment_Status,appointments.Location FROM appointments,doctors,patients "
			"WHERE doctors.Doctor_ID = appointments.Doctor_ID AND "
			"patients.Patient_ID = appointments.Patient_ID AND appointments.Doctor_ID= %d AND (appointments.Appointment_Date BETWEEN #%s# AND #%s#);", m_iViewAppointmentPatientID, m_ViewAppointmentStartDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY), m_ViewAppointmentEndDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY));

		break;

	 }

		CRecordset recset(&database);
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		ResetListControl();

		ListView_SetExtendedListViewStyle(m_EditListControlViewAppointments, LVS_EX_GRIDLINES);
		m_EditListControlViewAppointments.InsertColumn(0, L"Appointment ID", LVCFMT_LEFT, -1, 0);
		m_EditListControlViewAppointments.InsertColumn(1, L"Doctor ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(2, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(3, L"Patient ID", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(4, L"Patient Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(5, L"Subject Line", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(6, L"Appointment Date", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(7, L"Appointment Start Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(8, L"Appointment End Time", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(9, L"Appointment Status", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(10, L"Location", LVCFMT_LEFT, -1, 1);


		m_EditListControlViewAppointments.SetColumnWidth(0, 100);
		m_EditListControlViewAppointments.SetColumnWidth(1, 100);
		m_EditListControlViewAppointments.SetColumnWidth(2, 200);
		m_EditListControlViewAppointments.SetColumnWidth(3, 100);
		m_EditListControlViewAppointments.SetColumnWidth(4, 200);
		m_EditListControlViewAppointments.SetColumnWidth(5, 200);
		m_EditListControlViewAppointments.SetColumnWidth(6, 100);
		m_EditListControlViewAppointments.SetColumnWidth(7, 200);
		m_EditListControlViewAppointments.SetColumnWidth(8, 200);
		m_EditListControlViewAppointments.SetColumnWidth(9, 200);
		m_EditListControlViewAppointments.SetColumnWidth(10, 100);

		int iStartHour, iStartHourDB, iStartMinute, iStartMinuteDB, iEndHour, iEndHourDB, iEndMinute, iEndMinuteDB;
	
		while (!recset.IsEOF())
		{
			// Copy each column into a variable
			recset.GetFieldValue(L"Appointment_ID", sAppointmentID);
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Patient_ID", sPatientID);
			recset.GetFieldValue(L"Patient_Name", sPatientName);
			recset.GetFieldValue(L"Subject_Line", sSubjectLine);
			recset.GetFieldValue(L"Appointment_Date", sAppointmentDate);
			recset.GetFieldValue(L"Appointment_Start", sAppointmentStartTime);
			recset.GetFieldValue(L"Appointment_End", sAppointmentEndTime);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);
			recset.GetFieldValue(L"Location", sAppointmentLocation);

			appointmentDate.ParseDateTime(sAppointmentDate);
			appointmentStartTime.ParseDateTime(sAppointmentStartTime);
			appointmentEndTime.ParseDateTime(sAppointmentEndTime);

			iStartHour = m_ViewAppointmentStartTime.GetHour();
			iStartMinute = m_ViewAppointmentStartTime.GetMinute();
			iEndHour = m_ViewAppointmentEndTime.GetHour();
			iEndMinute = m_ViewAppointmentEndTime.GetMinute();

			iStartHourDB = appointmentStartTime.GetHour();
			iStartMinuteDB = appointmentStartTime.GetMinute();

			if ((iStartHourDB > iStartHour && 
				( iStartHourDB < iEndHour ||(iStartHourDB == iEndHour && iStartMinuteDB < iEndMinute))) ||
				(iStartHourDB == iStartHour && iStartMinuteDB >= iStartMinute &&
				(iStartHourDB < iEndHour || (iStartHourDB == iEndHour && iStartMinuteDB < iEndMinute))))

			{
				sAppointmentDate = appointmentDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
				sAppointmentStartTime = appointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
				sAppointmentEndTime = appointmentEndTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);

				// Insert values into the list control
				iRec = m_EditListControlViewAppointments.InsertItem(0, sAppointmentID, 0);
				m_EditListControlViewAppointments.SetItemText(0, 1, sDoctorID);
				m_EditListControlViewAppointments.SetItemText(0, 2, sDoctorName);
				m_EditListControlViewAppointments.SetItemText(0, 3, sPatientID);
				m_EditListControlViewAppointments.SetItemText(0, 4, sPatientName);
				m_EditListControlViewAppointments.SetItemText(0, 5, sSubjectLine);
				m_EditListControlViewAppointments.SetItemText(0, 6, sAppointmentDate);
				m_EditListControlViewAppointments.SetItemText(0, 7, sAppointmentStartTime);
				m_EditListControlViewAppointments.SetItemText(0, 8, sAppointmentEndTime);
				m_EditListControlViewAppointments.SetItemText(0, 9, sAppointmentStatus);
				m_EditListControlViewAppointments.SetItemText(0, 10, sAppointmentLocation);

				m_bIsListEmpty = false;
			}


			recset.MoveNext();

		}
		m_EditListControlViewAppointments.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditListControlViewAppointments.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

		if (m_bIsListEmpty)
		{
			AfxMessageBox(L"No appointments found");
		}


		// Close the database
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;


	UpdateData(FALSE);


}

void CBookAppointDlg::OnBnClickedRadioViewAppointmentsDoctorId()
{
	// TODO: Add your control notification handler code here
	m_iRadioButtonOption = 1;

}


void CBookAppointDlg::OnBnClickedRadioViewAppointmentsPatientId()
{
	// TODO: Add your control notification handler code here
	m_iRadioButtonOption = 2;
}


void CBookAppointDlg::OnBnClickedRadioViewAppointmentsDateInterval()
{
	// TODO: Add your control notification handler code here
	m_iRadioButtonOption = 3;
}


void CBookAppointDlg::OnBnClickedCancelAppointment()
{
	// TODO: Add your control notification handler code here
	int selectedRow = -1;
	selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST_VIEW_APPOINTMENTS))->GetSelectionMark();
	CString sAppointmentID = m_EditListControlViewAppointments.GetItemText(selectedRow, 0);
	int iAppointmentID = _wtoi(sAppointmentID);
	CString sAppointmentStatus;
	CDatabase database;
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		if (selectedRow == -1) 
		{
			AfxMessageBox(L"Select an appointment to cancel");
		}
		else
		{
			m_sSqlString.Format(L"SELECT Appointment_Status FROM appointments WHERE Appointment_ID = %d;", iAppointmentID);
			database.Open(NULL, false, false, m_sDsn);
			CRecordset recset(&database);
			recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);
			
			if (sAppointmentStatus == "Confirmed")
			{
				m_sSqlString.Format(L"DELETE FROM is_Available WHERE Appointment_ID = %d;", iAppointmentID);
				database.ExecuteSQL(m_sSqlString);
				AfxMessageBox(L"Appointment cancelled successfully");
				m_sSqlString.Format(L"UPDATE appointments SET Appointment_Status = 'Cancelled'  WHERE Appointment_ID = %d;", iAppointmentID);
				database.ExecuteSQL(m_sSqlString);
			}
			else
			{
				CString sMsg = L"Appointment already marked as " + sAppointmentStatus;
				AfxMessageBox(sMsg);
			}


		}

		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	DisplayDefaultAppointments();
}


void CBookAppointDlg::OnBnClickedRadioViewAppointmentsTimeInterval()
{
	// TODO: Add your control notification handler code here
	m_iRadioButtonOption = 4;

}


void CBookAppointDlg::OnBnClickedButtonMarkAsSeen()
{
	// TODO: Add your control notification handler code here
	int selectedRow = -1;
	selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST_VIEW_APPOINTMENTS))->GetSelectionMark();
	CString sAppointmentID = m_EditListControlViewAppointments.GetItemText(selectedRow, 0);
	int iAppointmentID = _wtoi(sAppointmentID);
	CString sAppointmentStatus;
	CDatabase database;
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		if (selectedRow == -1)
		{
			AfxMessageBox(L"Select an appointment to Mark as seen");
		}
		else
		{
			// Open the database
			m_sSqlString.Format(L"SELECT Appointment_Status FROM appointments WHERE Appointment_ID = %d;", iAppointmentID);
			database.Open(NULL, false, false, m_sDsn);
			CRecordset recset(&database);
			recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);

		if (sAppointmentStatus == "Confirmed")
		{
			m_sSqlString.Format(L"UPDATE appointments SET Appointment_Status = 'Seen'  WHERE Appointment_ID = %d;", iAppointmentID);
			database.ExecuteSQL(m_sSqlString);
			AfxMessageBox(L"Successfully marked appointment as seen");
			m_sSqlString.Format(L"DELETE FROM is_Available WHERE Appointment_ID = %d;", iAppointmentID);
			database.ExecuteSQL(m_sSqlString);
		}
		else
		{
			CString sMsg = L"Appointment already marked as " + sAppointmentStatus;
			AfxMessageBox(sMsg);
		}

		}
		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	DisplayDefaultAppointments();

}


void CBookAppointDlg::OnBnClickedButtonMarkAsNoShow()
{
	// TODO: Add your control notification handler code here
	int selectedRow = -1;
	selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST_VIEW_APPOINTMENTS))->GetSelectionMark();
	CString sAppointmentID = m_EditListControlViewAppointments.GetItemText(selectedRow, 0);
	int iAppointmentID = _wtoi(sAppointmentID);
	CDatabase database;
	CString sAppointmentStatus;

	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		if (selectedRow == -1)
		{
			AfxMessageBox(L"Select an appointment to Mark as seen");
		}
		else
		{
			m_sSqlString.Format(L"SELECT Appointment_Status FROM appointments WHERE Appointment_ID = %d;", iAppointmentID);
			database.Open(NULL, false, false, m_sDsn);
			CRecordset recset(&database);
			recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
			recset.GetFieldValue(L"Appointment_Status", sAppointmentStatus);

			if (sAppointmentStatus == "Confirmed")
			{
				m_sSqlString.Format(L"UPDATE appointments SET Appointment_Status = 'Did not show up'  WHERE Appointment_ID = %d;", iAppointmentID);
				database.ExecuteSQL(m_sSqlString);
				AfxMessageBox(L"Successfully marked appointment as \'Did not show up\'");
				m_sSqlString.Format(L"DELETE FROM is_Available WHERE Appointment_ID = %d;", iAppointmentID);
				database.ExecuteSQL(m_sSqlString);
			}


			else
			{
				CString sMsg = L"Appointment already marked as " + sAppointmentStatus;
				AfxMessageBox(sMsg);
			}
		}
		// Open the database


		database.Close();
	}


	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	DisplayDefaultAppointments();
}


void CBookAppointDlg::OnBnClickedButtonListDoctors()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_sSelectDepartment == "")
	{
		AfxMessageBox(L"Select a department from the list box");
	}
	else
	{
		UpdateData();
		CDatabase database;
		CString sDoctorName, sDoctorTitle, sDoctorID, sDoctorLocation, sDepartmentID;
		int iRec = 0;

		// Build ODBC connection string
		m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
		TRY
		{
			// Open the database
			database.Open(NULL,false,false,m_sDsn);
		CRecordset recsetDeptID(&database);
		m_sSqlString.Format(L"SELECT Department_ID FROM departments WHERE Department_Name='%s';",m_sSelectDepartment);
		recsetDeptID.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);

		// Copy each column into a variable
		recsetDeptID.GetFieldValue(L"Department_ID", sDepartmentID);

		CRecordset recset(&database);
		m_sSqlString.Format(L"SELECT Doctor_ID,Doctor_Name,Doctor_Title,Location FROM doctors WHERE Department_ID=%s;", sDepartmentID);
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		ResetListControl();

		ListView_SetExtendedListViewStyle(m_EditListControlViewAppointments, LVS_EX_GRIDLINES);
		m_EditListControlViewAppointments.InsertColumn(0, L"Doctor ID", LVCFMT_LEFT, -1, 0);
		m_EditListControlViewAppointments.InsertColumn(1, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(2, L"Doctor Title", LVCFMT_LEFT, -1, 1);
		m_EditListControlViewAppointments.InsertColumn(3, L"Location", LVCFMT_LEFT, -1, 1);

		m_EditListControlViewAppointments.SetColumnWidth(0, 100);
		m_EditListControlViewAppointments.SetColumnWidth(1, 200);
		m_EditListControlViewAppointments.SetColumnWidth(2, 200);
		m_EditListControlViewAppointments.SetColumnWidth(3, 200);

		while (!recset.IsEOF()) {
			// Copy each column into a variable
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Doctor_Title", sDoctorTitle);
			recset.GetFieldValue(L"Location", sDoctorLocation);


			// Insert values into the list control
			iRec = m_EditListControlViewAppointments.InsertItem(0, sDoctorID, 0);
			m_EditListControlViewAppointments.SetItemText(0, 1, sDoctorName);
			m_EditListControlViewAppointments.SetItemText(0, 2, sDoctorTitle);
			m_EditListControlViewAppointments.SetItemText(0, 3, sDoctorLocation);


			recset.MoveNext();
		}
		m_EditListControlViewAppointments.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditListControlViewAppointments.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

		// Close the database
		database.Close();
		}
			CATCH(CDBException, e)
		{
			// If a database exception occured, show error mgs
			AfxMessageBox(L"Database error: " + e->m_strError);
		}
		END_CATCH;


		UpdateData(FALSE);
	}

}


void CBookAppointDlg::OnBnClickedButtonAddDoctor()
{
	// TODO: Add your control notification handler code here
	int selectedRow = -1;
	selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST_VIEW_APPOINTMENTS))->GetSelectionMark();
	if (selectedRow == -1)
	{
		AfxMessageBox(L"Select a Doctor from the list");
	}
	else
	{
		CString strDoctorID = m_EditListControlViewAppointments.GetItemText(selectedRow, 0);
		m_iViewAppointmentDoctorID = _wtoi(strDoctorID);
		m_EditViewAppointmentDoctorID.SetWindowText(strDoctorID);
	}

}





void CBookAppointDlg::OnBnClickedRadioViewAppointmentsAll()
{
	// TODO: Add your control notification handler code here
	m_iRadioButtonOption = 0;

}


void CBookAppointDlg::OnBnClickedButtonClearFilter()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);	
	CBookAppointDlg mainDlg;
	mainDlg.DoModal();
	CDialogEx::OnCancel();
	UpdateData(FALSE);
	
}
