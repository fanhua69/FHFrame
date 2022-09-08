// MdiClient.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// This class does subclass the MDI-CLIENT window.
// Subclassing means that all messages are first routed to this class, then 
// to the original window (in this case the MDI-CLIENT).
// We need this to get notifications of the creation and deletion of the 
// MDI child frames (contain views).
/////////////////////////////////////////////////////////////////////////////
//
// Copyright ?1998 Written by Dieter Fauth 
//		mailto:fauthd@zvw.de 
//  
// This code may be used in compiled form in any way you desire. This    
// file may be redistributed unmodified by any means PROVIDING it is     
// not sold for profit without the authors written consent, and     
// providing that this notice and the authors name and all copyright     
// notices remains intact. If the source code in this file is used in     
// any  commercial application then a statement along the lines of     
// "Portions Copyright ?1999 Dieter Fauth" must be included in     
// the startup banner, "About" box or printed documentation. An email     
// letting me know that you are using it would be nice as well. That's     
// not much to ask considering the amount of work that went into this.    
//    
// This file is provided "as is" with no expressed or implied warranty.    
// The author accepts no liability for any damage/loss of business that    
// this product may cause.    
//  
// ==========================================================================  
// HISTORY:	  
// ==========================================================================  
//			1.00	08 May 1999	- Initial release.  
// ==========================================================================  
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tabbar.h"
#include "MdiClient.h"
#include "FHImage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiClient

CMdiClient::CMdiClient()
{
	m_pWndTabs = NULL;

	//m_pBmp = NULL;
	m_nBackColor = CLR_INVALID;
	m_nBkType=BK_STRETCH;

	m_iWidth=0;
	m_iHeight=0;


}

CMdiClient::~CMdiClient()
{
	/*if (m_pBmp)
	{
		m_pBmp->DeleteObject();
		delete m_pBmp;
		m_pBmp = NULL;
	}*/
	m_image.Destroy ();
}


BEGIN_MESSAGE_MAP(CMdiClient, CWnd)
	//{{AFX_MSG_MAP(CMdiClient)
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CMdiClient::AddHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	m_pWndTabs->AddHandle(hWnd);
}

void CMdiClient::RemoveHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	m_pWndTabs->RemoveHandle(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiClient message handlers

LRESULT CMdiClient::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = (HWND) DefWindowProc(WM_MDICREATE,  wParam, lParam);
	AddHandle(hWnd);
	return (LRESULT) hWnd;
}

LRESULT CMdiClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	RemoveHandle((HWND) wParam);
	return DefWindowProc(WM_MDIDESTROY,  wParam, lParam);
}




BOOL CMdiClient::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);

	if (m_nBackColor != CLR_INVALID)
	{
		pDC->FillSolidRect(&rect, m_nBackColor);
	}
	else if(m_iWidth!=0&&m_iHeight!=0)
			/*if (m_pBmp != NULL)*/
	{
		/*BITMAP bm;
		CDC dcMem;
		VERIFY(m_pBmp->GetObject(sizeof(BITMAP), (LPVOID)&bm));
		dcMem.CreateCompatibleDC(NULL);
		
		CBitmap* pOldBitmap = dcMem.SelectObject(m_pBmp);

		// 拉伸位图以适应背景区
		if(m_nBkType==BK_STRETCH)
		{
			pDC->StretchBlt (0,0,rect.Width (),rect.Height (),&dcMem,0,0,bm.bmWidth,bm.bmHeight ,SRCCOPY  );
		}

		// 下面的代码是排列位图
		else
		{			
			for (register int nX = 0; nX < rect.Width(); nX += bm.bmWidth)
			{
				for (register int nY = 0; nY < rect.Height(); nY += bm.bmHeight)
				{
					pDC->BitBlt(nX, nY, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, SRCCOPY);
				}
			}
		}

		dcMem.SelectObject(pOldBitmap);
		*/
		m_image.Show (pDC->m_hDC ,rect,CRect(0,0,m_iWidth,m_iHeight),false);
	}
	else
	{
		CWnd::OnEraseBkgnd(pDC);
	}

	return TRUE;
}

COLORREF CMdiClient::SetBackColor(COLORREF nBackColor)
{
	LockWindowUpdate();

	COLORREF cr = m_nBackColor;
	m_nBackColor = nBackColor;

	UnlockWindowUpdate();

	return cr;
}

BOOL CMdiClient::SetPic(CString sPath)
{
	//m_image.Detach ();
	int i=m_image.Load(sPath);
	if(i==E_FAIL)
	{
		return false;
	}
	m_iWidth=m_image.GetWidth ();
	m_iHeight=m_image.GetHeight ();

	return true;
}

////////////////////////////////////
/* Obsolete codes.
// for learning, save here.

BOOL CMdiClient::SetBitmap(UINT nID)
{
	BOOL bResult = true;

	LockWindowUpdate();

	if (m_pBmp)
	{
		m_pBmp->DeleteObject();
		delete m_pBmp;
		m_pBmp = NULL;
	}

	m_pBmp = new CBitmap();
	bResult = m_pBmp->LoadBitmap(nID);

	UnlockWindowUpdate();

	return bResult;
}*/

void CMdiClient::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	//if (m_pBmp != NULL)
		Invalidate();
}

void CMdiClient::SetBkType(enumBkType type)
{
	m_nBkType=type;
}