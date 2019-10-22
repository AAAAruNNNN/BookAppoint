#include "stdafx.h"
#include "CppUnitTest.h"
#include "afxdialogex.h"
#include "odbcinst.h"
#include "afxdb.h"
#include<ATLComTime.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
bool OnBnClickedConfirm(CString m_sPatientName,CString m_DOB,CString m_sGender,CString m_sCountryCode,CString m_sContactNumber,CString m_sAddress)
{
	// TODO: Add your control notification handler code here
	//UpdateData();

	CDatabase database;
	CString sSqlString;
	CString sDriver = L"MICROSOFT ACCESS DRIVER (*.mdb)";
	CString sDsn;
	CString sDSN = L"BookAppointAccess";
	CString sFile = L"C:\\Users\\AK068047\\Documents\\Test_BookAppoint.mdb";
	// You must change above path if it's different
	bool bSucessRegistration = true;

	if (m_sPatientName == "")
	{
		AfxMessageBox(L"You must provide a patient name or click Cancel");
		//this->m_EditPatientName.SetFocus();
		bSucessRegistration = false;
		return bSucessRegistration;
	}
	COleDateTime currDate;
	currDate = COleDateTime::GetCurrentTime();
	COleDateTime date;
	date.ParseDateTime(m_DOB);
	if (date > currDate)
	{
		AfxMessageBox(L"You must provide a valid date , Date of birth cannot be in the future");
		//this->m_EditDOB.SetFocus();
		bSucessRegistration = false;
		return bSucessRegistration;
	}
	if (m_sGender == "")
	{
		AfxMessageBox(L"You must provide a gender or click Cancel");
		//this->m_EditGender.SetFocus();
		bSucessRegistration = false;
		return bSucessRegistration;
	}
	if (m_sContactNumber == "")
	{
		AfxMessageBox(L"You must provide a contact number or click Cancel");
		//this->m_EditContactNumber.SetFocus();
		bSucessRegistration = false;
		return bSucessRegistration;
	}
	if (m_sAddress == "")
	{
		AfxMessageBox(L"You must provide an Address or click Cancel");
		//this->m_EditContactNumber.SetFocus();
		bSucessRegistration = false;
		return bSucessRegistration;
	}


	// Build ODBC connection string
	sDsn.Format(L"ODBC;DRIVER={%s};DSN=%s;DBQ=%s", sDriver, sDSN, sFile);
	TRY
	{
		// Open the database
	database.Open(NULL,false,false,sDsn);
	CString sDate = date.Format(LOCALE_NOUSEROVERRIDE | VAR_DATEVALUEONLY);
	sSqlString.Format(L"INSERT INTO Patients (Patient_Name,Contact_Number,Address,Gender,DOB) VALUES ('%s','%s','%s','%s','%s')",m_sPatientName,(m_sCountryCode + L"-" + m_sContactNumber),m_sAddress,m_sGender,sDate);
	database.ExecuteSQL(sSqlString);
	// Close the database
	database.Close();
	AfxMessageBox(L"Patient is Successfully Registered");
	//OnCancel();

	}
		CATCH(CDBException, e)
	{
		// If a database exception occured, show error mgs
		AfxMessageBox(L"Database error: " + e->m_strError);
	}
	END_CATCH;
	//UpdateData(FALSE);
	return bSucessRegistration;
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg1)
	{
	public:

		TEST_METHOD(AllBlank)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("", "","","","","");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg2)
	{
	public:

		TEST_METHOD(DOBinFuture)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", "3/12/2025", "M", "+91", "8877996578", "Address of the patient");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg3)
	{
	public:

		TEST_METHOD(ValidDOB)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", L"3/12/1997", "M", "+91", "8877996578", "Address of the patient");
			bool bExpected = true;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg4)
	{
	public:

		TEST_METHOD(BlankGender)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", "3/12/1997", "", "+91", "8877996578", "Address of the patient");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg5)
	{
	public:

		TEST_METHOD(BlankContactNumber)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", "3/12/1997", "M", "+91", "", "Address of the patient");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg6)
	{
	public:

		TEST_METHOD(BlankAddress)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", "3/12/1997", "M", "+91", "8877996578", "");
			bool bExpected = false;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}

namespace UnitTest_BookAppoint
{
	TEST_CLASS(UnitTestRegistratonDlg7)
	{
	public:

		TEST_METHOD(ValidRegistration)
		{
			// TODO: Your test code here
			bool bResult = OnBnClickedConfirm("Patient Name", "3/12/1997", "M", "+91", "8877996578", "Address of the patient");
			bool bExpected = true;
			Assert::AreEqual(bResult, bExpected);
		}

	};
}