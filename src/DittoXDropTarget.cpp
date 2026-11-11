// DittoXDropTarget.cpp : implementation file
//

#include "stdafx.h"
#include "CP_Main.h"
#include "DittoXDropTarget.h"
#include ".\dittoXdroptarget.h"


// CDittoXDropTarget

IMPLEMENT_DYNAMIC(CDittoXDropTarget, COleDropTarget)
CDittoXDropTarget::CDittoXDropTarget()
{
	m_pTarget = NULL;
}

CDittoXDropTarget::~CDittoXDropTarget()
{
}

BEGIN_MESSAGE_MAP(CDittoXDropTarget, COleDropTarget)
END_MESSAGE_MAP()

BOOL CDittoXDropTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if(m_pTarget)
		return m_pTarget->Drop(pDataObject, dropEffect, point);

	return COleDropTarget::OnDrop(pWnd, pDataObject, dropEffect, point);
}


DROPEFFECT CDittoXDropTarget::OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pTarget)
		return m_pTarget->DragEnter(pDataObject, dwKeyState, point);

	return COleDropTarget::OnDragEnter(pWnd, pDataObject, dwKeyState, point);
}

void CDittoXDropTarget::OnDragLeave(CWnd* pWnd)
{
	if(m_pTarget)
		return m_pTarget->DragLeave();

	COleDropTarget::OnDragLeave(pWnd);
}

DROPEFFECT CDittoXDropTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pTarget)
		return m_pTarget->DragOver(pDataObject, dwKeyState, point);

	return COleDropTarget::OnDragOver(pWnd, pDataObject, dwKeyState, point);
}
