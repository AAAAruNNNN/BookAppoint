// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BookAppoint.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include <afxdb.h>
#include "BookAppointDlg.h"

// CLoginDlg dialog

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_BOOKAPPOINT_LOGIN, pParent)
	, m_Username(_T(""))
	, m_Password(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USERNAME, m_EditUsername);
	DDX_Text(pDX, IDC_USERNAME, m_Username);
	DDX_Text(pDX, IDC_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_PASSWORD, m_EditPassword);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_BN_CLICKED(IDLOGIN, &CLoginDlg::OnBnClickedLogin)
END_MESSAGE_MAP()


// CLoginDlg message handlers


void CLoginDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CDatabase database;
	CString sSqlString;
	CString sUsername, sPassword;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sConnStr;
	CString sDSN = L"BookAppointAccess";
	CString sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	// Above is the path where db is stored
	bool bValidLogin = false;

	if (m_Username == "")
	{
		AfxMessageBox(L"You must provide a username and a password or click Cancel");
		this->m_EditUsername.SetFocus();
		return;
	}
	if (m_Password == "")
	{
		AfxMessageBox(L"Invalid Login");
		this->m_EditPassword.SetFocus();
		return;
	}
	// Build ODBC connection string
	sConnStr.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", sDriver, sDSN, sFile);
	TRY
	{
		// Open the database
		database.Open(NULL,false,false,sConnStr);

		// Allocate the recordset
		CRecordset recset(&database);

		// Build the SQL statement
		sSqlString = "SELECT Username, Password FROM Login";

		// Execute the query

		recset.Open(CRecordset::forwardOnly,sSqlString,CRecordset::readOnly);
		while (!recset.IsEOF()) 
		{
			// Copy each column into a variable
			recset.GetFieldValue(L"Username", sUsername);
			recset.GetFieldValue(L"Password", sPassword);
			if (m_Username == sUsername)
			{
				if (m_Password == sPassword)
				{
					bValidLogin = true;

				}

			}
			recset.MoveNext();
		}
		database.Close();
	}
	CATCH(CDBException, e) 
	{
		// If a database exception occured, show error msg
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;


	if (bValidLogin == true)
	{
		OnCancel();
		//OnOK();
		CBookAppointDlg mainDlg;
		mainDlg.DoModal();
	}
	else
	{
		AfxMessageBox(L"Invalid Credentials. Please try again");
		this->m_EditUsername.SetFocus();
	}

	UpdateData(FALSE);
}


BOOL CLoginDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	this->m_EditUsername.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
