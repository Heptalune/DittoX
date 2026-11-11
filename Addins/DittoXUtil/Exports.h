#pragma once

#include "..\..\Shared\DittoXDefines.h"
#include "..\..\Shared\IClip.h"
#include <vector>

extern "C" 
{
	bool __declspec(dllexport) DittoXAddin(const CDittoXInfo &DittoXInfo, CDittoXAddinInfo &info);
	bool __declspec(dllexport) SupportedFunctions(const CDittoXInfo &DittoXInfo, FunctionType type, std::vector<CFunction> &Functions);
	bool __declspec(dllexport) PasteAnyAsText(const CDittoXInfo &DittoXInfo, IClip *pClip);	
	bool __declspec(dllexport) ConvertPathToHtmlImageTag(const CDittoXInfo &DittoXInfo, IClip *pClip);
	bool __declspec(dllexport) ClearReadOnlyFlag(const CDittoXInfo &DittoXInfo, IClip *pClip);
	bool __declspec(dllexport) SetReadOnlyFlag(const CDittoXInfo &DittoXInfo, IClip *pClip);
	bool __declspec(dllexport) RemoveLineFeeds(const CDittoXInfo &DittoXInfo, IClip *pClip);
}