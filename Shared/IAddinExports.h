#pragma once

#include "DittoXDefines.h"
#include "IClip.h"
#include <vector>

extern "C" 
{
	//DittoXAddin returns infor about your addin, version, display name
	bool __declspec(dllexport) DittoXAddin(const CDittoXInfo &DittoXInfo, CDittoXAddinInfo &info);
	
	//Supported functions returns a list of available functions your addin supports based on the type passed in
	//For each of the functions you must have an exported function that matches the returned function name
	//CDittoXInfo contains info about dittoX. Included language code, to support multiple languages return the correct string based off of the language code currently used in dittoX (//http://www.loc.gov/standards/iso639-2/php/code_list.php)
	bool __declspec(dllexport) SupportedFunctions(const CDittoXInfo &DittoXInfo, FunctionType type, std::vector<CFunction> &Functions);

	//Each exported functions must follow the following format
	bool __declspec(dllexport) ConvertPathToHtmlImageTag(const CDittoXInfo &DittoXInfo, IClip *pClip);
}