#pragma once
#include "..\..\Shared\DittoXDefines.h"
#include "..\..\Shared\IClip.h"

#include <afxcoll.h>

class CReadOnlyFlag
{
public:
	CReadOnlyFlag(void);
	~CReadOnlyFlag(void);

	bool ResetReadOnlyFlag(const CDittoXInfo &DittoXInfo, IClip *pClip, bool resetFlag);

protected:
	bool LoadUnicodeFiles(CStringArray &lines, IClipFormats *pFormats);
	bool LoadTextFiles(CStringArray &lines, IClipFormats *pFormats);
	bool LoadHDropFiles(CStringArray &lines, IClipFormats *pFormats);
};

