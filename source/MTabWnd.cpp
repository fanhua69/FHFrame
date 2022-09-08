// MTabWnd.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// Derive your mainframe window from this class to get support for the
// MDI tabs. Also adds support for next/previous MDI window selection.
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
#include <afxpriv.h>
#include "FHFrame.h"
#include "MTabWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDITabFrameWnd

IMPLEMENT_DYNCREATE(CMDITabFrameWnd, CMDIFrameWnd)

CMDITabFrameWnd::CMDITabFrameWnd()
{
}

CMDITabFrameWnd::~CMDITabFrameWnd()
{
}

BEGIN_MESSAGE_MAP(CMDITabFrameWnd, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMDITabFrameWnd)
	ON_COMMAND(ID_WINDOW_NEXT_PANE, OnWindowNextPane)
	ON_COMMAND(ID_WINDOW_PREV_PANE, OnWindowPrevPane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMDITabFrameWnd::CreateTabs(void) 
{
	if (!m_wndTabs.Create(this,WS_VISIBLE|WS_CHILD|CBRS_TOP|WS_EX_WINDOWEDGE, IDC_MDI_TAB_CTRL_BAR)  )
	{
		TRACE("Failed to create test status bar\n");
		return -1;      // fail to create
	}
	
	// The MDI Client needs to talk to the TabBar
	m_wndMdiClient.m_pWndTabs = &m_wndTabs;
	ASSERT(m_hWndMDIClient != NULL);

	// This is to get notifications for adding/relemoving tabs (automatically)
	if (!m_wndMdiClient.SubclassWindow(m_hWndMDIClient)  )
	{
		TRACE("Failed to subclass MDI client\n");
		return -1;      // fail to create
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabFrameWnd message handlers
//

void CMDITabFrameWnd::OnWindowNextPane() 
{
	MDINext();	
}

void CMDITabFrameWnd::OnWindowPrevPane() 
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 1); 
}


