// ModifyDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "ModifyDataDlg.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include "EditSelectedPatientDlg.h"

// CModifyDataDlg dialog

IMPLEMENT_DYNAMIC(CModifyDataDlg, CDialogEx)

CModifyDataDlg::CModifyDataDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BOOKAPPOINT_MODIFY_UPDATE, pParent)
	, m_SearchModifyDOB(COleDateTime::GetCurrentTime())
	, m_sSearchModifyContactNumber(_T(""))
	, m_sSearchModifyPatientName(_T(""))
	, m_sSearchModifyCountryCode(_T(""))
{
	iRadioButtonOption = 0;

}

CModifyDataDlg::~CModifyDataDlg()
{
}

void CModifyDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_SEARCH_MODIFY_DOB, m_EditSearchModifyDOB);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_SEARCH_MODIFY_DOB, m_SearchModifyDOB);
	DDX_Control(pDX, IDC_COMBO_SEARCH_MODIFY_COUNTRY_CODE, m_EditSearchModifyCountryCode);
	DDX_CBString(pDX, IDC_COMBO_SEARCH_MODIFY_COUNTRY_CODE, m_sSearchModifyContactNumber);
	DDX_Control(pDX, IDC_EDIT_SEARCH_MODIFY_CONTACT_NUMBER, m_EditSearchModifyContactNumber);
	DDX_Text(pDX, IDC_EDIT_SEARCH_MODIFY_CONTACT_NUMBER, m_sSearchModifyContactNumber);
	DDX_Control(pDX, IDC_EDIT_SEARCH_MODIFY_PATIENT_NAME, m_EditSearchModifyPatientName);
	DDX_Text(pDX, IDC_EDIT_SEARCH_MODIFY_PATIENT_NAME, m_sSearchModifyPatientName);
	DDX_CBString(pDX, IDC_COMBO_SEARCH_MODIFY_COUNTRY_CODE, m_sSearchModifyCountryCode);
	DDX_Control(pDX, IDC_LIST_MODIFY_SEARCH_OUTPUT, m_EditSearchModifyListControl);
}


BEGIN_MESSAGE_MAP(CModifyDataDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_MODIFY_RESET, &CModifyDataDlg::OnBnClickedButtonSearchModifyReset)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_MODIFY_SEARCH, &CModifyDataDlg::OnBnClickedButtonSearchModifySearch)
	ON_BN_CLICKED(ID_BUTTON_SEARCH_MODIFY_EDIT, &CModifyDataDlg::OnBnClickedButtonSearchModifyEdit)
	ON_BN_CLICKED(IDC_RADIO_SEARCH_MODIFY_DOB, &CModifyDataDlg::OnBnClickedRadioSearchModifyDob)
	ON_BN_CLICKED(IDC_RADIO_SEARCH_MODIFY_CONTACT_NUMBER, &CModifyDataDlg::OnBnClickedRadioSearchModifyContactNumber)
	ON_BN_CLICKED(IDC_RADIO_SEARCH_MODIFY_NAME, &CModifyDataDlg::OnBnClickedRadioSearchModifyName)
	ON_BN_CLICKED(IDC_RADIO_SEARCH_MODIFY_ALL, &CModifyDataDlg::OnBnClickedRadioSearchModifyAll)
END_MESSAGE_MAP()


// CModifyDataDlg message handlers


void CModifyDataDlg::OnBnClickedButtonSearchModifyReset()
{
	// TODO: Add your control notification handler code here
	m_EditSearchModifyDOB.SetWindowText(_T(""));
	m_EditSearchModifyCountryCode.SetWindowText(_T(""));
	m_EditSearchModifyContactNumber.SetWindowText(_T(""));
	m_EditSearchModifyPatientName.SetWindowText(_T(""));
	ResetListControl();
}


void CModifyDataDlg::OnBnClickedButtonSearchModifySearch()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDatabase database;
	CString sSqlString;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sDSN = L"BookAppointAccess";
	CString sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	CString sPID, sPatientName, sContactNumber, sAddress, sGender, sDOB;
	// You must change above path if it's different
	int iRec = 0;

	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", sDriver, sDSN, sFile);
	TRY
	{	
	// Open the database
	database.Open(NULL,false,false,sDsn);
	CRecordset recset(&database);
	CString sDate = m_SearchModifyDOB.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	COleDateTime currDate,DOB;
	currDate = COleDateTime::GetCurrentTime();
	switch (iRadioButtonOption)
	{
	case 0:
		sSqlString.Format(L"SELECT * FROM patients;");
		break;
	case 1:
		if (m_SearchModifyDOB > currDate)
		{
			AfxMessageBox(L"You must provide a valid date , Date of birth cannot be in the future");
			this->m_EditSearchModifyDOB.SetFocus();
			return;
		}
		sSqlString.Format(L"SELECT * FROM patients WHERE DOB = #%s#;", sDate);
		break;
	case 2:	
		if (m_sSearchModifyCountryCode == "")
		{
			AfxMessageBox(L"You must provide a Country code");
			this->m_EditSearchModifyCountryCode.SetFocus();
			return;
		}
		if (m_sSearchModifyContactNumber == "")
		{
			AfxMessageBox(L"You must provide a contact number or click Cancel");
			this->m_EditSearchModifyContactNumber.SetFocus();
			return;
		}
		sSqlString.Format(L"SELECT * FROM patients WHERE Contact_Number = '%s';", (m_sSearchModifyCountryCode + L"-" + m_sSearchModifyContactNumber));
		break;
	case 3:
		if (m_sSearchModifyPatientName == "")
		{
			AfxMessageBox(L"You must provide a patient name or click Cancel");
			this->m_EditSearchModifyPatientName.SetFocus();
			return;
		}
		sSqlString.Format(L"SELECT * FROM patients WHERE Patient_Name = '%s';", m_sSearchModifyPatientName);
		break;
	default:
		sSqlString.Format(L"SELECT * FROM patients;");
	}

	recset.Open(CRecordset::forwardOnly, sSqlString, CRecordset::readOnly);
	ResetListControl();


	ListView_SetExtendedListViewStyle(m_EditSearchModifyListControl, LVS_EX_GRIDLINES);
	m_EditSearchModifyListControl.InsertColumn(0, L"ID", LVCFMT_LEFT, -1, 0);
	m_EditSearchModifyListControl.InsertColumn(1, L"Patient Name", LVCFMT_LEFT, -1, 1);
	m_EditSearchModifyListControl.InsertColumn(2, L"Contact Number", LVCFMT_LEFT, -1, 1);
	m_EditSearchModifyListControl.InsertColumn(3, L"Address", LVCFMT_LEFT, -1, 1);
	m_EditSearchModifyListControl.InsertColumn(4, L"Gender", LVCFMT_LEFT, -1, 1);
	m_EditSearchModifyListControl.InsertColumn(5, L"DOB", LVCFMT_LEFT, -1, 1);
	m_EditSearchModifyListControl.SetColumnWidth(0, 100);
	m_EditSearchModifyListControl.SetColumnWidth(1, 200);
	m_EditSearchModifyListControl.SetColumnWidth(2, 200);
	m_EditSearchModifyListControl.SetColumnWidth(3, 400);
	m_EditSearchModifyListControl.SetColumnWidth(4, 50);
	m_EditSearchModifyListControl.SetColumnWidth(5, 100);

	while (!recset.IsEOF()) {
		// Copy each column into a variable
		recset.GetFieldValue(L"Patient_ID", sPID);
		recset.GetFieldValue(L"Patient_Name", sPatientName);
		recset.GetFieldValue(L"Contact_Number", sContactNumber);
		recset.GetFieldValue(L"Address", sAddress);
		recset.GetFieldValue(L"Gender", sGender);
		recset.GetFieldValue(L"DOB", sDOB);

		DOB.ParseDateTime(sDOB);
		sDOB = DOB.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);

		// Insert values into the list control
		iRec = m_EditSearchModifyListControl.InsertItem(0, sPID, 0);
		m_EditSearchModifyListControl.SetItemText(0, 1, sPatientName);
		m_EditSearchModifyListControl.SetItemText(0, 2, sContactNumber);
		m_EditSearchModifyListControl.SetItemText(0, 3, sAddress);
		m_EditSearchModifyListControl.SetItemText(0, 4, sGender);
		m_EditSearchModifyListControl.SetItemText(0, 5, sDOB);


		// goto next record
		recset.MoveNext();
	}
	m_EditSearchModifyListControl.ModifyStyle(0, LVS_REPORT, NULL);
	m_EditSearchModifyListControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

	// Close the database
	database.Close();
	}
	CATCH(CDBException, e) 
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;

	UpdateData(FALSE);	// TODO: Add your control notification handler code here
}

void CModifyDataDlg::ResetListControl() 
{
	m_EditSearchModifyListControl.DeleteAllItems();
	int iNbrOfColumns;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_EditSearchModifyListControl.GetDlgItem(0);
	if (pHeader) 
	{
		iNbrOfColumns = pHeader->GetItemCount();
	}
	for (int i = iNbrOfColumns; i >= 0; i--) 
	{
		m_EditSearchModifyListControl.DeleteColumn(i);
	}
}


void CModifyDataDlg::OnBnClickedButtonSearchModifyEdit()
{
	// TODO: Add your control notification handler code here
	int selectedRow = ((CListCtrl*)GetDlgItem(IDC_LIST_MODIFY_SEARCH_OUTPUT))->GetSelectionMark();
	if (selectedRow == -1)
	{
		AfxMessageBox(L"You must select a patient to edit details");
		return;
	}
	else
	{
		CString sPatientID = m_EditSearchModifyListControl.GetItemText(selectedRow, 0);
		CString sPatientName = m_EditSearchModifyListControl.GetItemText(selectedRow, 1);
		CString sPatientDOB = m_EditSearchModifyListControl.GetItemText(selectedRow, 5);
		CString sPatientGender = m_EditSearchModifyListControl.GetItemText(selectedRow, 4);
		CString sPatientContactNumber = m_EditSearchModifyListControl.GetItemText(selectedRow, 2);
		CString sPatientAddress = m_EditSearchModifyListControl.GetItemText(selectedRow, 3);
		CEditSelectedPatientDlg editData(sPatientID, sPatientName, sPatientDOB, sPatientContactNumber, sPatientGender, sPatientAddress);
		editData.DoModal();
		CDialogEx::OnOK();
	}

}


void CModifyDataDlg::OnBnClickedRadioSearchModifyDob()
{
	// TODO: Add your control notification handler code here
	iRadioButtonOption = 1;

}


void CModifyDataDlg::OnBnClickedRadioSearchModifyContactNumber()
{
	// TODO: Add your control notification handler code here
	iRadioButtonOption = 2;

}


void CModifyDataDlg::OnBnClickedRadioSearchModifyName()
{
	// TODO: Add your control notification handler code here
	iRadioButtonOption = 3;

}


void CModifyDataDlg::OnBnClickedRadioSearchModifyAll()
{
	// TODO: Add your control notification handler code here
	iRadioButtonOption = 0;
}


BOOL CModifyDataDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_EditSearchModifyCountryCode.AddString(L"+91");
	m_EditSearchModifyCountryCode.AddString(L"+1");
	m_EditSearchModifyCountryCode.AddString(L"+44");
	m_EditSearchModifyCountryCode.AddString(L"+86");
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


