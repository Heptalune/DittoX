#pragma once

#include "DittoXAddin.h"
#include <vector>
#include <afxtempl.h>

class CDittoXAddins
{
public:
	CDittoXAddins(void);
	~CDittoXAddins(void);

	bool LoadAll();
	bool UnloadAll();

	bool Loaded()	{ return m_Addins.size() > 0; }

	bool AddPrePasteAddinsToMenu(CMenu *pMenu);
	bool CallPrePasteFunction(int Id, IClip *pClip);
	void AboutScreenText(CStringArray &arr);

protected:
	std::vector<CDittoXAddin*> m_Addins;

	class CFunctionLookup
	{
	public:
		CDittoXAddin *m_pAddin;
		CStringA m_csFunctionName;
	};

	CMap<int, int, CFunctionLookup, CFunctionLookup> m_FunctionMap;

protected:
	void LoadDittoXInfo(CDittoXInfo &DittoXInfo);
	CString LocalizedAddinText(const CString &csEnglish);
};
