// OptionsKeyBoard.cpp : implementation file
//

#include "stdafx.h"
#include "CP_Main.h"
#include "OptionsKeyBoard.h"
#include <afxrich.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsKeyBoard property page

IMPLEMENT_DYNCREATE(COptionsKeyBoard, CPropertyPage)

COptionsKeyBoard::COptionsKeyBoard() : CPropertyPage(COptionsKeyBoard::IDD)
{
	m_csTitle = theApp.m_Language.GetString("KeyboardShortcutsTitle", "键盘快捷键");
	m_psp.pszTitle = m_csTitle;
	m_psp.dwFlags |= PSP_USETITLE;
	
	//{{AFX_DATA_INIT(COptionsKeyBoard)
	//}}AFX_DATA_INIT
}

COptionsKeyBoard::~COptionsKeyBoard()
{
}

void COptionsKeyBoard::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionsKeyBoard)
	DDX_Control(pDX, IDC_HOTKEY, m_HotKey);
	DDX_Control(pDX, IDC_HOTKEY_ACTIVATE_2, m_HotKey2);
	DDX_Control(pDX, IDC_HOTKEY_ACTIVATE_3, m_HotKey3);
	DDX_Control(pDX, IDC_HOTKEY_TEXT_ONLY, m_TextOnlyKey);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_CUSTOM_KEYS, m_CustomeKeysHelp);
	DDX_Control(pDX, IDC_HOTKEY_SAVE_CLIPBOARD, m_saveClipboardHotKey);
	DDX_Control(pDX, IDC_HOTKEY_COPYSAVECLIPBOARD, m_copyAndSaveClipboardCtrl);
	DDX_Control(pDX, IDC_CHECK_WIN_V_TAKEOVER, m_btWinVTakeover);
}

BEGIN_MESSAGE_MAP(COptionsKeyBoard, CPropertyPage)
	//{{AFX_MSG_MAP(COptionsKeyBoard)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsKeyBoard message handlers

BOOL COptionsKeyBoard::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_CustomeKeysHelp.SetURL(_T("https://github.com/sabrogden/DittoX/wiki/Custom-Key-Strokes"));

	m_pParent = (COptionsSheet *)GetParent();

	theApp.m_pDittoXHotKey->CopyToCtrl(m_HotKey, m_hWnd, IDC_CHECK_WIN_DITTOX);
	theApp.m_pDittoXHotKey2->CopyToCtrl(m_HotKey2, m_hWnd, IDC_CHECK_WIN_DITTOX2);
	theApp.m_pDittoXHotKey3->CopyToCtrl(m_HotKey3, m_hWnd, IDC_CHECK_WIN_DITTOX3);

	theApp.m_pTextOnlyPaste->CopyToCtrl(m_TextOnlyKey, m_hWnd, IDC_CHECK_WIN_TEXT_ONLY);
	theApp.m_pSaveClipboard->CopyToCtrl(m_saveClipboardHotKey, m_hWnd, IDC_CHECK_WIN_SAVE_CLIPBOARD);
	theApp.m_pCopyAndSaveClipboard->CopyToCtrl(m_copyAndSaveClipboardCtrl, m_hWnd, IDC_CHECK_WIN_COPY_SAVE_CLIPBOARD);


	//Unregister hotkeys and Reregister them on cancel or ok
	g_HotKeys.UnregisterAll();

	m_btWinVTakeover.SetCheck(CGetSetOptions::GetWinVTakeover());
	m_btWinVTakeover.SetWindowText(theApp.m_Language.GetString("WinVTakeover", _T("Take over system Win+V shortcut")));
	CStatic* pHint = (CStatic*)GetDlgItem(IDC_STATIC_WINV_HINT);
	if(pHint)
	{
		pHint->SetWindowText(theApp.m_Language.GetString("WinVTakeoverHint",
			_T("Replaces the Windows Win+V clipboard history shortcut with DittoX.\nDisables system clipboard history and restarts Explorer, Ctrl+V still works.")));
	}

	m_HotKey.SetFocus();

	theApp.m_Language.UpdateOptionShortcuts(this);	
		
	return FALSE;
}

HBRUSH COptionsKeyBoard::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_WINV_HINT)
	{
		pDC->SetTextColor(RGB(192, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

LRESULT COptionsKeyBoard::OnWizardNext()
{
	return CPropertyPage::OnWizardNext();
}

BOOL COptionsKeyBoard::OnWizardFinish() 
{
	return CPropertyPage::OnWizardFinish();
}

BOOL COptionsKeyBoard::OnApply()
{
	INT_PTR x,y;
	CString str;
	ARRAY keys;
	
	g_HotKeys.GetKeys( keys ); // save old keys just in case new ones are invalid
	
	theApp.m_pDittoXHotKey->CopyFromCtrl(m_HotKey, m_hWnd, IDC_CHECK_WIN_DITTOX);
	theApp.m_pDittoXHotKey2->CopyFromCtrl(m_HotKey2, m_hWnd, IDC_CHECK_WIN_DITTOX2);
	theApp.m_pDittoXHotKey3->CopyFromCtrl(m_HotKey3, m_hWnd, IDC_CHECK_WIN_DITTOX3);
	
	theApp.m_pTextOnlyPaste->CopyFromCtrl(m_TextOnlyKey, m_hWnd, IDC_CHECK_WIN_TEXT_ONLY);
	theApp.m_pSaveClipboard->CopyFromCtrl(m_saveClipboardHotKey, m_hWnd, IDC_CHECK_WIN_SAVE_CLIPBOARD);
	theApp.m_pCopyAndSaveClipboard->CopyFromCtrl(m_copyAndSaveClipboardCtrl, m_hWnd, IDC_CHECK_WIN_COPY_SAVE_CLIPBOARD);

	// Take over system Win+V
	BOOL bWinVEnabled = m_btWinVTakeover.GetCheck() != FALSE;
	BOOL bWinVWasEnabled = CGetSetOptions::GetWinVTakeover();

	// Make sure no other DittoX hotkey already uses Win+V
	if(bWinVEnabled)
	{
		DWORD winVKey = MAKEWORD('V', HOTKEYF_EXT);
		bool bConflict = false;
		for(int i = 0; i < g_HotKeys.GetSize(); i++)
		{
			CHotKey* pKey = g_HotKeys.ElementAt(i);
			if(pKey && pKey != theApp.m_pWinVTakeover && pKey->m_Key == winVKey)
			{
				bConflict = true;
				break;
			}
		}
		if(bConflict)
		{
			MessageBox(_T("Win+V is already used by another DittoX hotkey.\nChange that hotkey first, then enable Win+V takeover."));
			bWinVEnabled = FALSE;
		}
	}

	if(bWinVEnabled)
	{
		if(!bWinVWasEnabled)
		{
			// First time enabling: disable the system Win+V hotkey in the registry
			// and restart Explorer to release it
			EnableWinVTakeover(TRUE);
			RestartExplorer();
		}

		// Explorer needs a moment to release Win+V, register the hotkey shortly after
		::SetTimer(theApp.m_MainhWnd, CMainFrame::TIMER_ID_WINV, 1500, NULL);
	}
	else if(bWinVWasEnabled)
	{
		// Unregister immediately and restore the system clipboard
		if(theApp.m_pWinVTakeover)
		{
			theApp.m_pWinVTakeover->Unregister();
			theApp.m_pWinVTakeover->SetKey(0, true);
		}

		EnableWinVTakeover(FALSE);
		RestartExplorer();
	}
	CGetSetOptions::SetWinVTakeover(bWinVEnabled);

	ARRAY NewKeys;
	g_HotKeys.GetKeys(NewKeys);
	
	if(g_HotKeys.FindFirstConflict(NewKeys, &x, &y))
	{
		str =  g_HotKeys.ElementAt(x)->GetName();
		str += " and ";
		str += g_HotKeys.ElementAt(y)->GetName();
		str += " cannot be the same.";
		MessageBox(str);
		g_HotKeys.SetKeys(keys); // restore the original values
		return FALSE;
	}
	
	g_HotKeys.SaveAllKeys();
	g_HotKeys.RegisterAll(true);
	
	return CPropertyPage::OnApply();
}

void COptionsKeyBoard::OnCancel() 
{
	g_HotKeys.RegisterAll( true );
	CPropertyPage::OnCancel();
}
