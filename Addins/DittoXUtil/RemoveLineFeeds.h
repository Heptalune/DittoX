#pragma once

#include "..\..\Shared\DittoXDefines.h"
#include "..\..\Shared\IClip.h"

class CRemoveLineFeeds
{
public:
	CRemoveLineFeeds(void);
	~CRemoveLineFeeds(void);

	bool RemoveLineFeeds(const CDittoXInfo &DittoXInfo, IClip *pClip);


private:
	bool Handle_CF_TEXT(IClipFormats *pFormats);
	bool Handle_CF_UNICODETEXT(IClipFormats *pFormats);
	bool Handle_RichText(IClipFormats *pFormats);
};

