#include "stdafx.h"
#include "CppUnitTest.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

bool OnBnClickedLogin(CString m_Username, CString m_Password)
{
	// TODO: Add your control notification handler code here
	//UpdateData();

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
		//this->m_EditUsername.SetFocus();
		bValidLogin = false;
		return bValidLogin;
	}
	if (m_Password == "")
	{
		AfxMessageBox(L"Invalid Login");
		//this->m_EditPassword.SetFocus();
		bValidLogin = false;
		return bValidLogin;
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


	/*if (bValidLogin == true)
	{
		OnCancel();
		OnOK();
	}
	else
	{
		AfxMessageBox(L"Invalid Credentials. Please try again");
		this->m_EditUsername.SetFocus();
	}

	UpdateData(FALSE);*/
	return bValidLogin;
}


namespace UnitTest_BookAppoint
{		
	TEST_CLASS(UnitTestLoginPage1)
	{
	public:
		
		TEST_METHOD(BlankUsernameandPassword)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedLogin("", "");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestLoginPage2)
	{
	public:

		TEST_METHOD(BlankUsername)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedLogin("", "newclear");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestLoginPage3)
	{
	public:

		TEST_METHOD(BlankPassword)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedLogin("arun", "");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestLoginPage4)
	{
	public:

		TEST_METHOD(WrongUsernameandPassword)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedLogin("alvin", "chipmunk");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestLoginPage5)
	{
	public:

		TEST_METHOD(ValidUsernameandPassword)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedLogin("arun", "newclear");
			bool bExpected = true;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}