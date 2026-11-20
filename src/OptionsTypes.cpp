// OptionsTypes.cpp : implementation file
//

#include "stdafx.h"
#include "CP_Main.h"
#include "OptionsTypes.h"
#include "..\Shared\ArrayEx.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsTypes property page

// Checkbox control ID <-> stored format name (stored names go into the Types table).
static const struct FormatCheckEntry
{
	UINT nCheckboxId;
	LPCTSTR csCode;
} g_formatChecks[] = {
	{ IDC_CHK_CF_TEXT,          _T("CF_TEXT") },
	{ IDC_CHK_CF_UNICODETEXT,   _T("CF_UNICODETEXT") },
	{ IDC_CHK_CF_OEMTEXT,       _T("CF_OEMTEXT") },
	{ IDC_CHK_CF_LOCALE,        _T("CF_LOCALE") },
	{ IDC_CHK_CF_DIB,           _T("CF_DIB") },
	{ IDC_CHK_CF_DIBV5,         _T("CF_DIBV5") },
	{ IDC_CHK_CF_BITMAP,        _T("CF_BITMAP") },
	{ IDC_CHK_PNG,              _T("PNG") },
	{ IDC_CHK_JPG,              _T("JPG") },
	{ IDC_CHK_JPEG,             _T("JPEG") },
	{ IDC_CHK_CF_HDROP,         _T("CF_HDROP") },
	{ IDC_CHK_CF_SYLK,          _T("CF_SYLK") },
	{ IDC_CHK_CF_DIF,           _T("CF_DIF") },
	{ IDC_CHK_RTF,              _T("Rich Text Format") },
	{ IDC_CHK_CF_RTFNOOBJS,     _T("CF_RTFNOOBJS") },
	{ IDC_CHK_CF_RETEXTOBJ,     _T("CF_RETEXTOBJ") },
	{ IDC_CHK_HTML,             _T("HTML Format") },
	{ IDC_CHK_CF_WAVE,          _T("CF_WAVE") },
	{ IDC_CHK_CF_RIFF,          _T("CF_RIFF") },
	{ IDC_CHK_CF_TIFF,          _T("CF_TIFF") },
	{ IDC_CHK_CF_PENDATA,       _T("CF_PENDATA") },
	{ IDC_CHK_CF_OWNERDISPLAY,  _T("CF_OWNERDISPLAY") },
	{ IDC_CHK_CF_DSPTEXT,       _T("CF_DSPTEXT") },
	{ IDC_CHK_CF_DSPBITMAP,     _T("CF_DSPBITMAP") },
	{ IDC_CHK_CF_METAFILEPICT,  _T("CF_METAFILEPICT") },
	{ IDC_CHK_CF_ENHMETAFILE,   _T("CF_ENHMETAFILE") },
	{ IDC_CHK_CF_DSPMETAFILEPICT, _T("CF_DSPMETAFILEPICT") },
	{ IDC_CHK_CF_DSPENHMETAFILE,  _T("CF_DSPENHMETAFILE") },
};

IMPLEMENT_DYNCREATE(COptionsTypes, CPropertyPage)

COptionsTypes::COptionsTypes() : CPropertyPage(COptionsTypes::IDD)
{
	m_csTitle = theApp.m_Language.GetString("SupportedTypesTitle", "支持的类型");
	m_psp.pszTitle = m_csTitle;
	m_psp.dwFlags |= PSP_USETITLE;

	//{{AFX_DATA_INIT(COptionsTypes)
	//}}AFX_DATA_INIT

	m_bSave = false;
}

COptionsTypes::~COptionsTypes()
{
}

void COptionsTypes::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsTypes)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionsTypes, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsTypes)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_CF_TEXT, IDC_CHK_CF_DSPENHMETAFILE, OnTypeCheckboxChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsTypes message handlers

BOOL COptionsTypes::OnApply()
{
	if(m_bSave)
	{
		try
		{
			theApp.m_db.execDML(_T("DELETE FROM Types;"));

			for (int i = 0; i < sizeof(g_formatChecks) / sizeof(g_formatChecks[0]); i++)
			{
				if (IsDlgButtonChecked(g_formatChecks[i].nCheckboxId) == BST_CHECKED)
					theApp.m_db.execDMLEx(_T("INSERT INTO Types VALUES(NULL, '%s');"), g_formatChecks[i].csCode);
			}
		}
		CATCH_SQLITE_EXCEPTION

		// refresh our local cache
		theApp.ReloadTypes();
	}

	return CPropertyPage::OnApply();
}

BOOL COptionsTypes::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Load the enabled formats from the database.
	CStringArray enabled;
	try
	{
		CppSQLite3Query q = theApp.m_db.execQuery(_T("SELECT TypeText FROM Types"));
		while(q.eof() == false)
		{
			enabled.Add(q.getStringField(0));
			q.nextRow();
		}
	}
	CATCH_SQLITE_EXCEPTION

	// Formats that should be on by default. Formats the user has never seen
	// (no DB row) get checked; explicit choices in the DB are respected.
	static const LPCTSTR commonFormats[] = {
		_T("CF_TEXT"), _T("CF_UNICODETEXT"), _T("CF_HDROP"), _T("CF_DIB"),
		_T("CF_DIBV5"), _T("CF_BITMAP"), _T("CF_TIFF"), _T("Rich Text Format"),
		_T("HTML Format"), _T("PNG"), _T("JPG"), _T("JPEG")
	};
	for (int i = 0; i < sizeof(commonFormats) / sizeof(commonFormats[0]); i++)
	{
		BOOL bFound = FALSE;
		for (int j = 0; j < enabled.GetSize(); j++)
		{
			if (enabled[j].CompareNoCase(commonFormats[i]) == 0)
			{
				bFound = TRUE;
				break;
			}
		}
		if (!bFound)
			enabled.Add(commonFormats[i]);
	}

	// Check the box of every enabled format.
	for (int i = 0; i < sizeof(g_formatChecks) / sizeof(g_formatChecks[0]); i++)
	{
		BOOL bChecked = FALSE;
		for (int j = 0; j < enabled.GetSize(); j++)
		{
			if (enabled[j].CompareNoCase(g_formatChecks[i].csCode) == 0)
			{
				bChecked = TRUE;
				break;
			}
		}
		CheckDlgButton(g_formatChecks[i].nCheckboxId, bChecked ? BST_CHECKED : BST_UNCHECKED);
	}

	theApp.m_Language.UpdateOptionSupportedTypes(this);

	return TRUE;
}

void COptionsTypes::OnTypeCheckboxChanged(UINT nID)
{
	m_bSave = true;
}
