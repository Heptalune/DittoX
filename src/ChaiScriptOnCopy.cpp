#include "stdafx.h"
#include "ChaiScriptOnCopy.h"
#include "..\Shared\TextConvert.h"

#include "chaiscript/chaiscript.hpp"

using namespace chaiscript;


ChaiScriptOnCopy::ChaiScriptOnCopy()
{
}


ChaiScriptOnCopy::~ChaiScriptOnCopy()
{
}

std::string FormatCurrentTime(const std::string &format) {
	CTime t = CTime::GetCurrentTime();
	CStringA x = t.Format(format.c_str());
	return x;
}

bool ChaiScriptOnCopy::ProcessScript(CDittoXChaiScript &clipData, std::string script)
{
	m_lastError = _T("");
	bool continueCopy = true;
	 
	try
	{
		ChaiScript chai;

		chai.add(chaiscript::fun(&CDittoXChaiScript::GetClipMD5), "GetClipMD5");
		chai.add(chaiscript::fun(&CDittoXChaiScript::GetClipSize), "GetClipSize");
		chai.add(chaiscript::fun(&CDittoXChaiScript::GetAsciiString), "GetAsciiString");
		chai.add(chaiscript::fun(&CDittoXChaiScript::SetAsciiString), "SetAsciiString");
		chai.add(chaiscript::fun(&CDittoXChaiScript::GetActiveApp), "GetActiveApp");

		chai.add(chaiscript::fun(&CDittoXChaiScript::GetActiveAppTitle), "GetActiveAppTitle");

		chai.add(chaiscript::fun(&CDittoXChaiScript::SetMakeTopSticky), "SetMakeTopSticky");
		chai.add(chaiscript::fun(&CDittoXChaiScript::SetMakeLastSticky), "SetMakeLastSticky");
		chai.add(chaiscript::fun(&CDittoXChaiScript::SetReplaceTopSticky), "SetReplaceTopSticky");

		chai.add(chaiscript::fun(&CDittoXChaiScript::FormatExists), "FormatExists");
		chai.add(chaiscript::fun(&CDittoXChaiScript::RemoveFormat), "RemoveFormat");
		chai.add(chaiscript::fun(&CDittoXChaiScript::SetParentId), "SetParentId");
		chai.add(chaiscript::fun(&CDittoXChaiScript::AsciiTextMatchesRegex), "AsciiTextMatchesRegex");
		chai.add(chaiscript::fun(&CDittoXChaiScript::AsciiTextReplaceRegex), "AsciiTextReplaceRegex");

		chai.add(chaiscript::fun(&CDittoXChaiScript::DescriptionMatchesRegex), "DescriptionMatchesRegex");
		chai.add(chaiscript::fun(&CDittoXChaiScript::DescriptionReplaceRegex), "DescriptionReplaceRegex");

		chai.add(chaiscript::fun(&FormatCurrentTime), "FormatCurrentTime");

		chai.add(chaiscript::var(&clipData), "clip");

		//loop over all scripts
		/*std::string script = R""(	
									var md5 = clip.GetClipMD5(13)
									return md5 == "4FF8DB22A28559FB23AB9EB90523AE3E"
								)"";*/


		Boxed_Value bv = chai.eval(script);
		if (chaiscript::boxed_cast<bool> (bv) == true)
		{
			m_lastError = _T("Script returned true, canceling copy");
			continueCopy = false;
		}
	}
	catch (const chaiscript::exception::eval_error &ee)
	{
		std::string eString;
		eString += ee.pretty_print();

		m_lastError = CTextConvert::AnsiToUnicode(eString.c_str());
	}
	catch (std::exception &e)
	{
		std::string eString;
		eString += e.what();

		m_lastError = CTextConvert::AnsiToUnicode(eString.c_str());
	}
	catch (CException *ex)
	{
		TCHAR szCause[255];
		ex->GetErrorMessage(szCause, 255);
		m_lastError.Format(_T("ProcessScript exception: %s"), szCause);
	}
	catch (...)
	{
		m_lastError.Format(_T("ProcessScript exception"));
	}

	return continueCopy;
}
