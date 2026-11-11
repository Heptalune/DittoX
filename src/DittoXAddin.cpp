#include "stdafx.h"
#include "DittoXAddin.h"
#include "Misc.h"
#include "..\Shared\TextConvert.h"

CDittoXAddin::CDittoXAddin() :
	m_hModule(NULL)
{
}

CDittoXAddin::~CDittoXAddin()
{
	Cleanup();
}

bool CDittoXAddin::DoLoad(LPCTSTR lpszDllName, CDittoXInfo DittoXInfo)
{
	bool bLoaded = false;
	m_csLastError.Empty();
	Cleanup();

	if(lpszDllName)
	{
		_tcscpy(m_DllName, lpszDllName);

		m_hModule = ::LoadLibrary(lpszDllName);

		if( m_hModule )
		{
			bool (__cdecl *DittoXAddin)(const CDittoXInfo&, CDittoXAddinInfo&);
			DittoXAddin = (bool(__cdecl*)(const CDittoXInfo&, CDittoXAddinInfo&))GetProcAddress(m_hModule, "DittoXAddin");
			if(DittoXAddin)
			{
				bLoaded = DittoXAddin(DittoXInfo, m_DittoXAddinInfo);
				if(bLoaded)
				{
					m_SupportedFunctions = (bool(__cdecl*)(const CDittoXInfo&, FunctionType,std::vector<CFunction>&))GetProcAddress(m_hModule, "SupportedFunctions");
					SupportedFunctions(DittoXInfo, eFuncType_PRE_PASTE, m_PrePasteFunctions);
				}
				else
				{
					m_csLastError.Format((_T("DittoX Addin - DittoXAddin return false, not loading Addin")));
				}
			}
			else
			{
				m_csLastError.Format((_T("DittoX Addin - Failed to Get Function DittoXAddin")));
			}
		}
		else
		{
			m_csLastError.Format((_T("DittoX Addin - Failed to load library on Addin %s"), lpszDllName));
		}
	}

	return bLoaded;
}

void CDittoXAddin::Cleanup()
{
	if(m_hModule)
	{
		// release resources to the dll
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

bool CDittoXAddin::SupportedFunctions(const CDittoXInfo &DittoXInfo, FunctionType type, std::vector<CFunction> &Functions)
{
	bool bRet = false;
	m_csLastError.Empty();

	if(m_SupportedFunctions != NULL)
	{
		bRet = m_SupportedFunctions(DittoXInfo, type, Functions);
		if(bRet)
		{
			INT_PTR nCount = Functions.size();
			for(int i = 0; i < nCount; i++)
			{
				CFunction func = Functions[i];
				Log(StrF(_T("DittoX Addin - Supported Function Display: %s, Function: %s, Desc: %s"), func.m_csDisplayName, CTextConvert::AnsiToUnicode(func.m_csFunction), func.m_csDetailDescription));
			}
		}
		else
		{
			m_csLastError = _T("DittoX Addin - m_SupportedFunctions returned false");
		}
	}
	else
	{
		m_csLastError = _T("DittoX Addin - m_SupportedFunctions is null, not call function load supported functions");
	}

	return bRet;
}

bool CDittoXAddin::PrePasteFunction(const CDittoXInfo &DittoXInfo, CStringA Function, IClip *pClip)
{
	bool (__cdecl *PrePasteFunc)(const CDittoXInfo &, IClip*);

	PrePasteFunc = (bool(__cdecl*)(const CDittoXInfo &, IClip*))GetProcAddress(m_hModule, Function);
	if(PrePasteFunc)
	{
		return PrePasteFunc(DittoXInfo, pClip);
	}

	return false;
}
