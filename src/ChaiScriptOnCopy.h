#pragma once

#include "DittoXChaiScript.h"
#include <string>

class ChaiScriptOnCopy
{
public:
	ChaiScriptOnCopy();
	~ChaiScriptOnCopy();

	bool ProcessScript(CDittoXChaiScript &clipData, std::string script);

	CString m_lastError;
};

