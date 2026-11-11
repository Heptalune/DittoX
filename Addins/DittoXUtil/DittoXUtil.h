// DittoXUtil.h : main header file for the DittoXUtil DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CDittoXUtilApp
// See DittoXUtil.cpp for the implementation of this class
//

class CDittoXUtilApp : public CWinApp
{
public:
	CDittoXUtilApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL ExitInstance();

	DECLARE_MESSAGE_MAP()
};
