// EditSelectedPatientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "EditSelectedPatientDlg.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include <stdio.h>



// CEditSelectedPatientDlg dialog

IMPLEMENT_DYNAMIC(CEditSelectedPatientDlg, CDialogEx)

CEditSelectedPatientDlg::CEditSelectedPatientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKAPPOINT_MODIFY_UPDATE_ONCLICKEDEDITBUTTON, pParent)
	, m_EditDetailsDateOfBirth(COleDateTime::GetCurrentTime())
	, m_sEditDetailsGender(_T(""))
	, m_sEditDetailsCountryCode(_T(""))
	, m_sEditDetailsContactNumber(_T(""))
	, m_sEditDetailsAddress(_T(""))
	, m_sEditDetailsPatientName(_T(""))
{

}

CEditSelectedPatientDlg::CEditSelectedPatientDlg(CString PID, CString PName, CString PDOB, CString PContactNumber, CString PGender, CString PAddress, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKAPPOINT_MODIFY_UPDATE_ONCLICKEDEDITBUTTON, pParent)
	, m_EditDetailsDateOfBirth(COleDateTime::GetCurrentTime())
	, m_sEditDetailsGender(_T(""))
	, m_sEditDetailsCountryCode(_T(""))
	, m_sEditDetailsContactNumber(_T(""))
	, m_sEditDetailsAddress(_T(""))
	, m_sEditDetailsPatientName(_T(""))

{
	sPatientID = PID;
	m_sEditDetailsPatientName = PName;
	COleDateTime date;
	date.ParseDateTime(PDOB);
	m_EditDetailsDateOfBirth = date;
	int iTokenPos = 0;
	m_sEditDetailsCountryCode = PContactNumber.Tokenize(L"-",iTokenPos);
	m_sEditDetailsContactNumber = PContactNumber.Tokenize(L"-",iTokenPos);
	m_sEditDetailsGender = PGender;
	m_sEditDetailsAddress = PAddress;
}

CEditSelectedPatientDlg::~CEditSelectedPatientDlg()
{
}

void CEditSelectedPatientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATIENT_NAME, m_EditDetailsPatientName);
	DDX_Control(pDX, IDC_DATETIMEPICKER_DOB, m_EditDetailsDOB);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_DOB, m_EditDetailsDateOfBirth);
	DDX_Control(pDX, IDC_COMBO_GENDER, m_EditDetailsGender);
	DDX_CBString(pDX, IDC_COMBO_GENDER, m_sEditDetailsGender);
	DDX_Control(pDX, IDC_COMBO_COUNTRY_CODE, m_EditDetailsCountryCode);
	DDX_CBString(pDX, IDC_COMBO_COUNTRY_CODE, m_sEditDetailsCountryCode);
	DDX_Text(pDX, IDC_EDIT_CONTACT_NUMBER, m_sEditDetailsContactNumber);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_EditDetailsAddress);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_sEditDetailsAddress);
	DDX_Control(pDX, IDC_EDIT_CONTACT_NUMBER, m_EditDetailsContactNumber);
	DDX_Text(pDX, IDC_EDIT_PATIENT_NAME, m_sEditDetailsPatientName);
}


BEGIN_MESSAGE_MAP(CEditSelectedPatientDlg, CDialogEx)
	ON_BN_CLICKED(IDCONFIRM, &CEditSelectedPatientDlg::OnBnClickedConfirm)
END_MESSAGE_MAP()


// CEditSelectedPatientDlg message handlers


void CEditSelectedPatientDlg::OnBnClickedConfirm()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	BOOL bIsUpdated = TRUE;
	CDatabase database;
	CString sSqlString;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sDSN = L"BookAppointAccess";
	CString sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	// You must change above path if it's different

	if (m_sEditDetailsPatientName == "")
	{
		AfxMessageBox(L"You must provide a patient name or click Cancel");
		this->m_EditDetailsPatientName.SetFocus();

		return;
	}
	COleDateTime currDate;
	currDate = COleDateTime::GetCurrentTime();
	if (m_EditDetailsDateOfBirth > currDate)
	{
		AfxMessageBox(L"You must provide a valid date , Date of birth cannot be in the future");
		this->m_EditDetailsDOB.SetFocus();

		return;
	}
	if (m_sEditDetailsGender == "")
	{
		AfxMessageBox(L"You must provide a gender or click Cancel");
		this->m_EditDetailsGender.SetFocus();

		return;
	}
	if (m_sEditDetailsContactNumber == "")
	{
		AfxMessageBox(L"You must provide a contact number or click Cancel");
		this->m_EditDetailsContactNumber.SetFocus();

		return;
	}
	if (m_sEditDetailsAddress == "")
	{
		AfxMessageBox(L"You must provide an Address or click Cancel");
		this->m_EditDetailsAddress.SetFocus();

		return;
	}
	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", sDriver, sDSN, sFile);
	TRY
	{
	// Open the database
	database.Open(NULL,false,false,sDsn);

	//CString sDate = m_DOB.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	sSqlString.Format(L"UPDATE patients SET Patient_Name = '%s' , Contact_Number = '%s', Address = '%s', Gender = '%s' WHERE Patient_ID = %s;", m_sEditDetailsPatientName, (m_sEditDetailsCountryCode + L"-" + m_sEditDetailsContactNumber), m_sEditDetailsAddress, m_sEditDetailsGender,sPatientID);
	database.ExecuteSQL(sSqlString);
	// Close the database
	database.Close();
	}
	CATCH(CDBException, e) 
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
		bIsUpdated = FALSE;
	}
	END_CATCH;
	UpdateData(FALSE);
	if (bIsUpdated)
	{
		AfxMessageBox(L"Patient details updated");
		CDialogEx::OnOK();

	}
	else
		AfxMessageBox(L"Failed to Update patient details");
}


BOOL CEditSelectedPatientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_EditDetailsCountryCode.AddString(L"+91");
	m_EditDetailsCountryCode.AddString(L"+1");
	m_EditDetailsCountryCode.AddString(L"+44");
	m_EditDetailsCountryCode.AddString(L"+86");


	m_EditDetailsGender.AddString(L"M");
	m_EditDetailsGender.AddString(L"F");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
