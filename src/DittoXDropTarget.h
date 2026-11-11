
#pragma once


// CDittoXDropTarget command target

class IDittoXDropTarget
{
public:
	virtual DROPEFFECT DragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) = 0;
	virtual DROPEFFECT DragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) = 0;
	virtual void DragLeave(void) = 0;
	virtual BOOL Drop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) = 0;
};


class CDittoXDropTarget : public COleDropTarget
{
	DECLARE_DYNAMIC(CDittoXDropTarget)

public:
	CDittoXDropTarget();
	virtual ~CDittoXDropTarget();

	void SetDropTarget(IDittoXDropTarget *pTarget) { m_pTarget = pTarget; }

protected:
	IDittoXDropTarget *m_pTarget;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual void OnDragLeave(CWnd* pWnd);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
};


