#pragma once

#include "..\Shared\DittoXDefines.h"
#include "..\Shared\IClip.h"
#include <vector>

class CDittoXAddin
{
public:
	CDittoXAddin();
	virtual ~CDittoXAddin();

	bool DoLoad(LPCTSTR lpszDllName, CDittoXInfo DittoXInfo);

	std::vector<CFunction> m_PrePasteFunctions;
	bool PrePasteFunction(const CDittoXInfo &DittoXInfo, CStringA Function, IClip *pClip);

	CString DisplayName() { return m_DittoXAddinInfo.m_Name; }
	int Version() { return m_DittoXAddinInfo.m_AddinVersion; }
	int PrivateVersion() { return m_DittoXAddinInfo.PrivateVersion(); }
	CString LastError()	{ return m_csLastError; }

protected:
	TCHAR m_DllName[MAX_PATH];
	HMODULE m_hModule;
	CDittoXAddinInfo m_DittoXAddinInfo;
	CString m_csLastError;
	
protected:
	void Cleanup();

	bool (__cdecl *m_SupportedFunctions)(const CDittoXInfo&, FunctionType,std::vector<CFunction>&);
	bool SupportedFunctions(const CDittoXInfo &DittoXInfo, FunctionType type, std::vector<CFunction> &Functions);	
};
