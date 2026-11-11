// About.cpp : implementation file
// test test test

//Main branch test

#include "stdafx.h"
#include "cp_main.h"
#include "About.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbout property page

IMPLEMENT_DYNCREATE(CAbout, CPropertyPage)

CAbout::CAbout() : CPropertyPage(CAbout::IDD)
{
	m_csTitle = theApp.m_Language.GetString("AboutTitle", "关于");
	m_psp.pszTitle = m_csTitle;
	m_psp.dwFlags |= PSP_USETITLE;

	//{{AFX_DATA_INIT(CAbout)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAbout::~CAbout()
{
}

void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	DDX_Control(pDX, IDC_STATIC_LINK, m_Link);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CPropertyPage)
	//{{AFX_MSG_MAP(CAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers

BOOL CAbout::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_List.AddString(_T("DittoX"));

	auto runningVersion = GetRunningVersion();
	CString cs = GetVersionString(runningVersion);

	CString csText;
	csText = "    " + theApp.m_Language.GetString("About_Version", "版本") + " " + cs;
#if _WIN64
	csText += " 64bit";
#else
	csText += " 32bit";
#endif
	m_List.AddString(csText);

	const char *SqliteVersion = sqlite3_libversion();
	csText = "    SQLite " + theApp.m_Language.GetString("About_Version", "版本") + " ";
	csText += SqliteVersion;
	m_List.AddString(csText);

	cs = CGetSetOptions::GetExeFileName();
	csText = "    " + theApp.m_Language.GetString("About_ExePath", "程序路径") + " " + cs;
	m_List.AddString(csText);

	cs = CGetSetOptions::GetDBPath();
	csText = "    " + theApp.m_Language.GetString("About_DBPath", "数据库路径") + " " + cs;
	m_List.AddString(csText);

	m_List.AddString(_T(""));

	m_List.AddString(theApp.m_Language.GetString("About_Credits", "开发"));

	m_List.AddString(_T("    ") + theApp.m_Language.GetString("About_Author", "Heptalune (github.com/Heptalune/DittoX)"));

	m_List.AddString(_T(""));

	m_List.AddString(theApp.m_Language.GetString("Add_Ins", "加载项"));

	//Show what addins are loaded
	CStringArray arr;
	theApp.m_Addins.AboutScreenText(arr);
	INT_PTR count = arr.GetCount();
	for(int i = 0; i < count; i++)
	{
		m_List.AddString(_T("    ") + arr[i]);
	}

	CRect rect;
	GetClientRect(rect);

	rect.bottom -= 24;

	m_List.MoveWindow(rect);

	rect.top = rect.bottom + 8;
	rect.bottom = rect.top + 16;

	m_Link.MoveWindow(rect);
	m_Link.SetURL(_T("https://github.com/Heptalune/DittoX"));

	m_Link.SetWindowText(theApp.m_Language.GetString("About_HomePage", "DittoX 主页 - github.com/Heptalune/DittoX"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
