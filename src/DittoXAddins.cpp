#include "stdafx.h"
#include ".\dittoXaddins.h"
#include "misc.h"
#include "CP_Main.h"

CDittoXAddins::CDittoXAddins(void)
{
}

CDittoXAddins::~CDittoXAddins(void)
{
	UnloadAll();
}

bool CDittoXAddins::UnloadAll()
{
	Log(StrF(_T("DittoX Addin - Unloading all addins Count: %d"), m_Addins.size()));

	INT_PTR count = m_Addins.size();
	for(int i = 0; i < count; i++)
	{
		CDittoXAddin *pAddin = m_Addins[i];
		if(pAddin)
		{
			delete pAddin;
			pAddin = NULL;
		}
	}

	m_Addins.clear();

	return true;
}

bool CDittoXAddins::LoadAll()
{
	CDittoXInfo DittoXInfo;
	LoadDittoXInfo(DittoXInfo);

	CString csDir = CGetSetOptions::GetPath(PATH_ADDINS);

	CFileFind find;
	BOOL bCont = find.FindFile(csDir + _T("*.dll"));

	while(bCont)
	{
		bCont = find.FindNextFile();

		Log(StrF(_T("DittoX Addin - Trying to load addin file %s"), find.GetFilePath()));

		CDittoXAddin *pAddin = new CDittoXAddin;
		if(pAddin->DoLoad(find.GetFilePath(), DittoXInfo))
		{
			Log(StrF(_T("DittoX Addin - Success, loaded addin: %s"), find.GetFilePath()));
			m_Addins.push_back(pAddin);
		}
		else
		{
			Log(StrF(_T("DittoX Addin - Failed loading Adding Error: %s"), pAddin->LastError()));

			delete pAddin;
			pAddin = NULL;
		}
	}

	return m_Addins.size() > 0;
}

bool CDittoXAddins::AddPrePasteAddinsToMenu(CMenu *pMenu)
{
	bool bRet = false;

	m_FunctionMap.RemoveAll();
	int nMenuId = 3000;

	HMENU AllAddinsMenu = ::CreateMenu();

	INT_PTR count = m_Addins.size();
	for(int i = 0; i < count; i++)
	{
		CDittoXAddin *pAddin = m_Addins[i];
		if(pAddin)
		{
			INT_PTR subCount = pAddin->m_PrePasteFunctions.size();
			if(subCount > 1)
			{
				HMENU AddinMenu = ::CreateMenu();
				for(int x = 0; x < subCount; x++)
				{
					::AppendMenu(AddinMenu, MF_ENABLED, nMenuId, LocalizedAddinText(pAddin->m_PrePasteFunctions[x].m_csDisplayName));

					CFunctionLookup lookup;
					lookup.m_csFunctionName = pAddin->m_PrePasteFunctions[x].m_csFunction;
					lookup.m_pAddin = pAddin;
					m_FunctionMap.SetAt(nMenuId, lookup);
					nMenuId++;
				}

				::AppendMenu(AllAddinsMenu, MF_ENABLED|MF_POPUP, (UINT_PTR)AddinMenu, LocalizedAddinText(pAddin->DisplayName()));
				bRet = true;
			}
			else if(subCount == 1)
			{
				//If there is only 1 function for this add in then just show one menu with addin name - function
				CFunctionLookup lookup;
				lookup.m_csFunctionName = pAddin->m_PrePasteFunctions[0].m_csFunction;
				lookup.m_pAddin = pAddin;
				m_FunctionMap.SetAt(nMenuId, lookup);

				CString menuName;
				menuName.Format(_T("%s - %s"), LocalizedAddinText(pAddin->DisplayName()), LocalizedAddinText(pAddin->m_PrePasteFunctions[0].m_csDisplayName));

				::AppendMenu(AllAddinsMenu, MF_ENABLED, nMenuId, menuName);
				bRet = true;
				nMenuId++;
			}
		}
	}

	if(bRet)
	{
		int nPos = pMenu->GetMenuItemCount();
		pMenu->InsertMenu(nPos, MF_BYPOSITION | MF_SEPARATOR);
		pMenu->InsertMenu(nPos + 1, MF_BYPOSITION|MF_ENABLED|MF_STRING|MF_POPUP, (UINT_PTR)AllAddinsMenu, theApp.m_Language.GetString("Add_Ins", "插件"));
	}

	return bRet;
}

bool CDittoXAddins::CallPrePasteFunction(int Id, IClip *pClip)
{
	bool bRet = false;
	CFunctionLookup func;
	if(m_FunctionMap.Lookup(Id, func))
	{
		CDittoXInfo DittoXInfo;
		LoadDittoXInfo(DittoXInfo);

		bRet = func.m_pAddin->PrePasteFunction(DittoXInfo, func.m_csFunctionName, pClip);
	}

	return bRet;
}

CString CDittoXAddins::LocalizedAddinText(const CString &csEnglish)
{
	// Addin display names come from the addin DLLs as hard coded English.
	// Map the known names to language file keys so they follow the selected language.
	CString csKey;

	if (csEnglish.CompareNoCase(_T("DittoX Utils")) == 0)
		csKey = _T("Addin_DittoXUtil");
	else if (csEnglish.CompareNoCase(_T("Paste Any Clip As Text")) == 0)
		csKey = _T("Addin_PasteAnyClipAsText");
	else if (csEnglish.CompareNoCase(_T("Paste As html image link")) == 0)
		csKey = _T("Addin_PasteAsHtmlImageLink");
	else if (csEnglish.CompareNoCase(_T("Clear read only flag")) == 0)
		csKey = _T("Addin_ClearReadOnly");
	else if (csEnglish.CompareNoCase(_T("Set read only flag")) == 0)
		csKey = _T("Addin_SetReadOnly");
	else if (csEnglish.CompareNoCase(_T("Paste Removing Line Feeds")) == 0)
		csKey = _T("Addin_RemoveLineFeeds");
	else if (csEnglish.CompareNoCase(_T("Displays a list of clip formats allowing you to select one and paste the contents as text")) == 0)
		csKey = _T("Addin_Desc_PasteAnyClipAsText");
	else if (csEnglish.CompareNoCase(_T("Converts a CF_DIB or CF_HDROP to a html format for pasting into outlook express")) == 0)
		csKey = _T("Addin_Desc_PasteAsHtmlImageLink");
	else if (csEnglish.CompareNoCase(_T("Clears read only flag on the types CF_HDROP, or files paths in text")) == 0)
		csKey = _T("Addin_Desc_ClearReadOnly");
	else if (csEnglish.CompareNoCase(_T("Sets the read only flag on the types CF_HDROP, or files paths in text")) == 0)
		csKey = _T("Addin_Desc_SetReadOnly");

	if (csKey.IsEmpty())
	{
		return csEnglish;
	}

	return theApp.m_Language.GetString(csKey, csEnglish);
}

void CDittoXAddins::LoadDittoXInfo(CDittoXInfo &DittoXInfo)
{
	DittoXInfo.m_csDatabasePath = CGetSetOptions::GetDBPath();
	DittoXInfo.m_csLanguageCode = theApp.m_Language.GetLangCode();	
	DittoXInfo.m_csSqliteVersion = sqlite3_libversion();
	DittoXInfo.m_hWndDittoX = theApp.QPastehWnd();
}

void CDittoXAddins::AboutScreenText(CStringArray &arr)
{
	INT_PTR count = m_Addins.size();
	for(int i = 0; i < count; i++)
	{
		CDittoXAddin *pAddin = m_Addins[i];
		if(pAddin)
		{
			CString csLine;
			csLine.Format(_T("%s %s: %d, %s2: %d"), LocalizedAddinText(pAddin->DisplayName()),
				theApp.m_Language.GetString("Addin_Ver", "版本"), pAddin->Version(),
				theApp.m_Language.GetString("Addin_Ver", "版本"), pAddin->PrivateVersion());
			arr.Add(csLine);
			INT_PTR subCount = pAddin->m_PrePasteFunctions.size();
			for(int x = 0; x < subCount; x++)
			{
				CString csLine2;
				csLine2.Format(_T("    %s (%s)"), LocalizedAddinText(pAddin->m_PrePasteFunctions[x].m_csDisplayName), LocalizedAddinText(pAddin->m_PrePasteFunctions[x].m_csDetailDescription));
				arr.Add(csLine2);
			}
			arr.Add("");
		}
	}
}