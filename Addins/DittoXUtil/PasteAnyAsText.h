#pragma once
#include "..\..\Shared\DittoXDefines.h"
#include "..\..\Shared\IClip.h"


class PasteAnyAsText
{
public:
	PasteAnyAsText(void);
	~PasteAnyAsText(void);

	static bool SelectClipToPasteAsText(const CDittoXInfo &DittoXInfo, IClip *pClip);
};
