// BookAppointmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "BookAppointmentDlg.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include<stdlib.h>


// CBookAppointmentDlg dialog

IMPLEMENT_DYNAMIC(CBookAppointmentDlg, CDialogEx)

CBookAppointmentDlg::CBookAppointmentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKAPPOINTMENT_DLG, pParent)
	, m_AppointmentDate(COleDateTime::GetCurrentTime())
	, m_AppointmentStartTime(COleDateTime::GetCurrentTime())
	, m_AppointmentEndTime(COleDateTime::GetCurrentTime())
	, m_sSelectDepartment(_T(""))
	, m_iDoctorID(0)
	, m_iPatientID(0)
	, m_sSubjectLine(_T(""))
	, m_sAppointmentLocation(_T(""))
{
	m_sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	m_sDSN = L"BookAppointAccess";
	m_sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
}

CBookAppointmentDlg::~CBookAppointmentDlg()
{
}

void CBookAppointmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APPOINTMENT_DATE, m_EditAppointmentDate);
	DDX_DateTimeCtrl(pDX, IDC_APPOINTMENT_DATE, m_AppointmentDate);
	DDX_Control(pDX, IDC_APPOINTMENT_START_TIME, m_EditAppointmentStartTime);
	DDX_DateTimeCtrl(pDX, IDC_APPOINTMENT_START_TIME, m_AppointmentStartTime);
	DDX_Control(pDX, IDC_APPOINTMENT_END_TIME, m_EditAppointmentEndTime);
	DDX_DateTimeCtrl(pDX, IDC_APPOINTMENT_END_TIME, m_AppointmentEndTime);
	DDX_Control(pDX, IDC_COMBO_SELECT_DEPARTMENT, m_EditSelectDepartment);
	DDX_CBString(pDX, IDC_COMBO_SELECT_DEPARTMENT, m_sSelectDepartment);
	DDX_Control(pDX, IDC_EDIT_DOCTOR_ID, m_EditDoctorID);
	DDX_Text(pDX, IDC_EDIT_DOCTOR_ID, m_iDoctorID);
	DDX_Control(pDX, IDC_EDIT_PATIENT_ID, m_EditPatientID);
	DDX_Text(pDX, IDC_EDIT_PATIENT_ID, m_iPatientID);
	DDX_Control(pDX, IDC_EDIT_SUBJECT_LINE, m_EditSubjectLine);
	DDX_Text(pDX, IDC_EDIT_SUBJECT_LINE, m_sSubjectLine);
	DDX_Control(pDX, IDC_LIST1, m_EditBookAppointmentListCtrl);
	DDX_Control(pDX, IDC_APPOINTMENT_LOCATION, m_EditAppointmentLocation);
	DDX_Text(pDX, IDC_APPOINTMENT_LOCATION, m_sAppointmentLocation);
}


BEGIN_MESSAGE_MAP(CBookAppointmentDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LIST_DOCTORS, &CBookAppointmentDlg::OnBnClickedButtonListDoctors)
	ON_BN_CLICKED(IDOK, &CBookAppointmentDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD_DOCTOR_ID, &CBookAppointmentDlg::OnBnClickedAddDoctorId)
END_MESSAGE_MAP()


// CBookAppointmentDlg message handlers





void CBookAppointmentDlg::ResetListControl()
{
	int iNbrOfColumns;
	m_EditBookAppointmentListCtrl.DeleteAllItems();
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_EditBookAppointmentListCtrl.GetDlgItem(0);
	if (pHeader)
	{
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--)
	{
		m_EditBookAppointmentListCtrl.DeleteColumn(i);
	}
}

BOOL CBookAppointmentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData();
	CDatabase database;
	CString sDepartmentName;

	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);
		CRecordset recset(&database);
		m_sSqlString.Format(L"SELECT * FROM departments;");
		recset.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		while (!recset.IsEOF())
		{
			// Copy each column into a variable
			recset.GetFieldValue(L"Department_Name", sDepartmentName);
			m_EditSelectDepartment.AddString(sDepartmentName);
			// goto next record
			recset.MoveNext();
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



	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}






void CBookAppointmentDlg::OnBnClickedButtonListDoctors()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CDatabase database;
	CString sDoctorName, sDoctorTitle, sDoctorID,sDoctorLocation, sDepartmentID;
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

		ListView_SetExtendedListViewStyle(m_EditBookAppointmentListCtrl, LVS_EX_GRIDLINES);
		m_EditBookAppointmentListCtrl.InsertColumn(0, L"Doctor ID", LVCFMT_LEFT, -1, 0);
		m_EditBookAppointmentListCtrl.InsertColumn(1, L"Doctor Name", LVCFMT_LEFT, -1, 1);
		m_EditBookAppointmentListCtrl.InsertColumn(2, L"Doctor Posting", LVCFMT_LEFT, -1, 1);
		m_EditBookAppointmentListCtrl.InsertColumn(3, L"Location", LVCFMT_LEFT, -1, 1);

		m_EditBookAppointmentListCtrl.SetColumnWidth(0, 100);
		m_EditBookAppointmentListCtrl.SetColumnWidth(1, 200);
		m_EditBookAppointmentListCtrl.SetColumnWidth(2, 200);
		m_EditBookAppointmentListCtrl.SetColumnWidth(3, 200);

		while (!recset.IsEOF()) {
			// Copy each column into a variable
			recset.GetFieldValue(L"Doctor_ID", sDoctorID);
			recset.GetFieldValue(L"Doctor_Name", sDoctorName);
			recset.GetFieldValue(L"Doctor_Title", sDoctorTitle);
			recset.GetFieldValue(L"Location", sDoctorLocation);


			// Insert values into the list control
			iRec = m_EditBookAppointmentListCtrl.InsertItem(0, sDoctorID, 0);
			m_EditBookAppointmentListCtrl.SetItemText(0, 1, sDoctorName);
			m_EditBookAppointmentListCtrl.SetItemText(0, 2, sDoctorTitle);
			m_EditBookAppointmentListCtrl.SetItemText(0, 3, sDoctorLocation);


			recset.MoveNext();
		}
		m_EditBookAppointmentListCtrl.ModifyStyle(0, LVS_REPORT, NULL);
		m_EditBookAppointmentListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

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





void CBookAppointmentDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	COleDateTime currDate;
	currDate = COleDateTime::GetCurrentTime();
	if (m_AppointmentDate.GetYear() < currDate.GetYear() || (m_AppointmentDate.GetYear() == currDate.GetYear() && m_AppointmentDate.GetDayOfYear() < currDate.GetDayOfYear()))
	{
		AfxMessageBox(L"You must provide a valid date , Appointment date cannot be in the past");
		this->m_EditAppointmentDate.SetFocus();
		return;
	}

	else if (m_AppointmentDate.GetYear() == currDate.GetYear() && m_AppointmentDate.GetDayOfYear() == currDate.GetDayOfYear())
	{
		if (COleDateTime::GetCurrentTime() > m_AppointmentStartTime)
		{
			AfxMessageBox(L"Appointment start time cannot be before current time.");
			this->m_EditAppointmentStartTime.SetFocus();
			return;
		}

	}

	if (m_AppointmentStartTime > m_AppointmentEndTime)
	{
		AfxMessageBox(L"Appointment end time cannot be before start time.");
		this->m_EditAppointmentEndTime.SetFocus();
		return;
	}


	if (m_iPatientID < 0 || m_iPatientID == 0)
	{
		AfxMessageBox(L"You must provide a Valid Patient ID");
		this->m_EditPatientID.SetFocus();
		return;
	}

	if (m_sSubjectLine == "")
	{
		AfxMessageBox(L"You must provide a Subject Line");
		this->m_EditSubjectLine.SetFocus();
		return;
	}


	//find if doctor is available
	CDatabase database;
	CString sStartTimeDB, sEndTimeDB, sReasonUnavailable;
	COleDateTime startTime, endTime;
	bool bValidAppointment = TRUE;

	// Build ODBC connection string
	m_sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", m_sDriver, m_sDSN, m_sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,m_sDsn);
		CRecordset recsetPatientID(&database);
		CRecordset recsetIsAvailable(&database);
		CString sAppointmentDate = m_AppointmentDate.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
		CString sAppointmentStartTime, sAppointmentEndTime;
		sAppointmentStartTime = m_AppointmentStartTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
		sAppointmentEndTime = m_AppointmentEndTime.Format(LOCALE_NOUSEROVERRIDE | VAR_TIMEVALUEONLY);
		CString sPatientID;
		bool bValidPatientID = FALSE;

		m_sSqlString = L"SELECT Patient_ID from patients";
	
		recsetPatientID.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
		while (!recsetPatientID.IsEOF())
		{
			recsetPatientID.GetFieldValue(L"Patient_ID", sPatientID);

			if (_wtoi(sPatientID) == m_iPatientID)
			{
				bValidPatientID = TRUE;
				break;
			}
			recsetPatientID.MoveNext();

		}
		if (bValidPatientID)
		{

			m_sSqlString.Format(L"SELECT Start_Time,End_Time,Reason_Unavailable FROM is_Available WHERE Doctor_ID=%d and Date_Unavailable = #%s#;", m_iDoctorID, sAppointmentDate);
			recsetIsAvailable.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
			int  iStartHour, iStartMinute, iEndHour, iEndMinute, iStartHourDB, iStartMinuteDB, iEndHourDB, iEndMinuteDB;

			// Copy each column into a variable
			while (!recsetIsAvailable.IsEOF())
			{
				// Copy each column into a variable
				recsetIsAvailable.GetFieldValue(L"Start_Time", sStartTimeDB);
				recsetIsAvailable.GetFieldValue(L"End_Time", sEndTimeDB);
				recsetIsAvailable.GetFieldValue(L"Reason_Unavailable", sReasonUnavailable);

				startTime.ParseDateTime(sStartTimeDB);
				endTime.ParseDateTime(sEndTimeDB);
			
				iStartHour = m_AppointmentStartTime.GetHour();
				iStartMinute = m_AppointmentStartTime.GetMinute();
				iEndHour = m_AppointmentEndTime.GetHour();
				iEndMinute = m_AppointmentEndTime.GetMinute();
			
				iStartHourDB = startTime.GetHour();
				iStartMinuteDB = startTime.GetMinute();
				iEndHourDB = endTime.GetHour();
				iEndMinuteDB = endTime.GetMinute();


				if (iStartHour > iStartHourDB || iStartHour == iStartHourDB && iStartMinute >= iStartMinuteDB)
				{
					if (iStartHour < iEndHourDB || iStartHour == iEndHourDB &&  iStartMinute <= iEndMinuteDB || iEndHour < iEndHourDB || iEndHour == iEndHourDB && iEndMinute <= iEndMinuteDB)
						bValidAppointment = FALSE;
				}
				else if (iStartHour < iStartHourDB && iEndHour > iEndHourDB )
					bValidAppointment = FALSE;
				else if (iEndHour > iStartHourDB || iEndHour == iStartHourDB && iEndMinute >= iStartMinuteDB && iEndHour < iEndHourDB || iEndHour == iEndHourDB && iEndMinute <= iEndMinuteDB)
					bValidAppointment = FALSE;
				else
					bValidAppointment = TRUE;

				if (!bValidAppointment)
				{
					AfxMessageBox(sReasonUnavailable);
					this->m_EditAppointmentStartTime.SetFocus();
					return;
				}

				recsetIsAvailable.MoveNext();
			}
		}
		else
		{
			AfxMessageBox(L"Patient does not exist. Enter a valid patient ID or Register new patient");
			this->m_EditPatientID.SetFocus();
			return;
		}
	
		if (bValidAppointment)
		{
			m_sSqlString.Format(L"INSERT INTO appointments (Appointment_Start,Appointment_End,Appointment_Status,Location,Doctor_ID,Patient_ID,Appointment_Date,Subject_Line) VALUES (#%s#,#%s#,'Confirmed','%s',%d,%d,#%s#,'%s')", sAppointmentStartTime, sAppointmentEndTime, m_sAppointmentLocation, m_iDoctorID, m_iPatientID, sAppointmentDate, m_sSubjectLine);
			database.ExecuteSQL(m_sSqlString);
			CString sAppointmentID;
			int iAppointmentID;
			m_sSqlString.Format(L"SELECT Appointment_ID FROM appointments WHERE Appointment_Start=#%s# AND Appointment_End=#%s# AND Doctor_ID=%d AND Patient_ID=%d AND Appointment_Date=#%s#;", sAppointmentStartTime, sAppointmentEndTime, m_iDoctorID, m_iPatientID, sAppointmentDate);
			CRecordset recsetGetAppointmentID(&database);
			recsetGetAppointmentID.Open(CRecordset::forwardOnly, m_sSqlString, CRecordset::readOnly);
			recsetGetAppointmentID.GetFieldValue(L"Appointment_ID", sAppointmentID);
			iAppointmentID = _wtoi(sAppointmentID);
			m_sSqlString.Format(L"INSERT INTO is_Available (Doctor_ID,Date_Unavailable,Start_Time,End_Time,Reason_Unavailable,Appointment_ID) VALUES (%d,#%s#,#%s#,#%s#,'Patient Appointment',%d);", m_iDoctorID, sAppointmentDate, sAppointmentStartTime, sAppointmentEndTime, iAppointmentID);
			database.ExecuteSQL(m_sSqlString);

			AfxMessageBox(L"Appointment Booked Successfully");
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

	CDialogEx::OnOK();
}


void CBookAppointmentDlg::OnBnClickedAddDoctorId()
{
	// TODO: Add your control notification handler code here
	int selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST1))->GetSelectionMark();
	CString strDoctorID = m_EditBookAppointmentListCtrl.GetItemText(selectedRow, 0);
	CString sAppointmentLocation = m_EditBookAppointmentListCtrl.GetItemText(selectedRow, 3);
	m_iDoctorID = _wtoi(strDoctorID);
	m_EditDoctorID.SetWindowText(strDoctorID);
	m_EditAppointmentLocation.SetWindowText(sAppointmentLocation);


}
