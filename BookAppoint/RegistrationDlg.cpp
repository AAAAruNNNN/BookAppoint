// RegistrationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "RegistrationDlg.h"
#include "afxdialogex.h"

#include "odbcinst.h"
#include "afxdb.h"

// CRegistrationDlg dialog

IMPLEMENT_DYNAMIC(CRegistrationDlg, CDialog)

CRegistrationDlg::CRegistrationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BOOKAPPOINT_REGISTRATION, pParent)
	, m_sPatientName(_T(""))
	, m_DOB(COleDateTime::GetCurrentTime())
	, m_sGender(_T(""))
	, m_sCountryCode(_T(""))
	, m_sContactNumber(_T(""))
	, m_sAddress(_T(""))
{

}

CRegistrationDlg::~CRegistrationDlg()
{
}

void CRegistrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATIENT_NAME, m_EditPatientName);
	DDX_Text(pDX, IDC_EDIT_PATIENT_NAME, m_sPatientName);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DOB, m_EditDOB);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DOB, m_DOB);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_EditGender);
	DDX_CBString(pDX, IDC_COMBO_GENDER, m_sGender);
	DDX_Control(pDX, IDC_COMBO_COUNTRY_CODE, m_EditCountryCode);
	DDX_CBString(pDX, IDC_COMBO_COUNTRY_CODE, m_sCountryCode);
	DDX_Control(pDX, IDC_EDIT_CONTACT_NUMBER, m_EditContactNumber);
	DDX_Text(pDX, IDC_EDIT_CONTACT_NUMBER, m_sContactNumber);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_EditAddress);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_sAddress);
}


BEGIN_MESSAGE_MAP(CRegistrationDlg, CDialog)
	ON_BN_CLICKED(IDCONFIRM, &CRegistrationDlg::OnBnClickedConfirm)
END_MESSAGE_MAP()


// CRegistrationDlg message handlers


void CRegistrationDlg::OnBnClickedConfirm()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDatabase database;
	CString sSqlString;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sDSN = L"BookAppointAccess";
	CString sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	// You must change above path if it's different

	if (m_sPatientName == "")
	{
		AfxMessageBox(L"You must provide a patient name or click Cancel");
		this->m_EditPatientName.SetFocus();

		return;
	}
	COleDateTime currDate;
	currDate = COleDateTime::GetCurrentTime();
	if (m_DOB > currDate)
	{
		AfxMessageBox(L"You must provide a valid date , Date of birth cannot be in the future");
		this->m_EditDOB.SetFocus();

		return;
	}
	if (m_sGender == "")
	{
		AfxMessageBox(L"You must provide a gender or click Cancel");
		this->m_EditGender.SetFocus();

		return;
	}
	if (m_sContactNumber == "")
	{
		AfxMessageBox(L"You must provide a contact number or click Cancel");
		this->m_EditContactNumber.SetFocus();

		return;
	}
	if (m_sAddress == "")
	{
		AfxMessageBox(L"You must provide an Address or click Cancel");
		this->m_EditContactNumber.SetFocus();

		return;
	}


	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", sDriver, sDSN, sFile);
	TRY
	{
		// Open the database
	database.Open(NULL,false,false,sDsn);
	CString sDate = m_DOB.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	sSqlString.Format(L"INSERT INTO Patients (Patient_Name,Contact_Number,Address,Gender,DOB) VALUES ('%s','%s','%s','%s','%s')",m_sPatientName,(m_sCountryCode + L"-" + m_sContactNumber),m_sAddress,m_sGender,sDate);
	database.ExecuteSQL(sSqlString);
	// Close the database
	database.Close();
	AfxMessageBox(L"Patient is Successfully Registered");
	OnCancel();

	}
	CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	UpdateData(FALSE);
}


BOOL CRegistrationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_EditCountryCode.AddString(L"+91");
	m_EditCountryCode.AddString(L"+1");
	m_EditCountryCode.AddString(L"+44");
	m_EditCountryCode.AddString(L"+86");

	m_EditGender.AddString(L"M");
	m_EditGender.AddString(L"F");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


