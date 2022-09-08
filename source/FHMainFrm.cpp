
// FHMainFrm.cpp : implementation file
//

#include "stdafx.h"
#include "FHApp.h"
#include "FHFrame.h"
#include "FHMainFrm.h"
#include "Splash.h"
#include <direct.h>
#include "MdiClient.h"

#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>
#include <string.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFHMainFrm

IMPLEMENT_DYNAMIC(CFHMainFrm, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CFHMainFrm, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CFHMainFrm)
    ON_WM_CREATE()
    ON_COMMAND(ID_WORKSPACEBAR, OnWorkspacebar)
    ON_UPDATE_COMMAND_UI(ID_WORKSPACEBAR, OnUpdateWorkspacebar)
    ON_WM_DESTROY()
    ON_COMMAND(ID_OutputBar, OnOutputBar)
    ON_UPDATE_COMMAND_UI(ID_OutputBar, OnUpdateOutputBar)
    ON_COMMAND(ID_WINDOW_NEXT_PANE, OnWindowNextPane)
    ON_COMMAND(ID_WINDOW_PREV_PANE, OnWindowPrevPane)

    //}}AFX_MSG_MAP
    // Global help commands
    //ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
    //ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
    //ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
    //ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
};

CString             gsHistoryFile;
CHistoryManager     gHistoryManager;


/////////////////////////////////////////////////////////////////////////////
// CFHMainFrm construction/destruction

CFHMainFrm::CFHMainFrm()
{
    m_nSoftICON         =IDR_ICON_JLGS;	
    m_nWorkSpace_TAB    =IDB_WorkSpace_TAB;
    m_sBackGroundPic    ="back.jpg";
    m_iMainToolBar      =IDR_FHMAINFRAME;

    gsHistoryFile       ="qchistory.his";
    gHistoryManager.Read(gsHistoryFile);


    CSoftDog dog;
    long n=dog.GetSerNumber();

    // my dog number 136
    if(n!=(int)stovf(gHistoryManager.GetSoftwareNumber()))
    {
        gHistoryManager.RemoveAll();
        gHistoryManager.SetUserName(::GetUserName());
        gHistoryManager.SetSoftwareNumber(vtos(n));
    }

}

CFHMainFrm::~CFHMainFrm()
{
    gHistoryManager.Save(gsHistoryFile);
}

void CFHMainFrm::SetClientBack(CString sPic)
{
    m_sBackGroundPic=sPic;	
}

void CFHMainFrm::SetTitleIcon(UINT nICON)
{
    m_nSoftICON=nICON;
}

void CFHMainFrm::SetMainToolBar(int iToolBar)
{
    m_iMainToolBar=iToolBar;
}

void CFHMainFrm::RemoveMultiDocument    (CDocument *pDocument)
{
    m_listMultiDocument.remove(pDocument);
}

void CFHMainFrm::AddMultiDocument       (CDocument *pDocument)
{
    m_listMultiDocument.push_front(pDocument);
}

void CFHMainFrm::SetActiveMultiDocument (CDocument *pDocument)
{
    std::list<CDocument*>::iterator it = m_listMultiDocument.begin();
    bool bExist = false;
    while(it!=m_listMultiDocument.end())
    {
        if(*it == pDocument)
        {
            bExist = true;
            break;
        }
        it++;
    }
    
    if(!bExist)
    {
        return;
    }

    RemoveMultiDocument(pDocument);
    AddMultiDocument(pDocument);
}

CDocument * CFHMainFrm::GetActiveMultiDocument ()
{
    if(m_listMultiDocument.size()==0)
    {
        return NULL;
    }
    return m_listMultiDocument.front();
}


int CFHMainFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (!m_wndToolBar.Create(this) ||
        !m_wndToolBar.LoadToolBar(m_iMainToolBar))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_toolbar.Create(this) ||
        !m_toolbar.LoadToolBar(IDR_toolbarPowerSectionView))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }    

    if (!m_FHZoomViewToolBar.Create(this))
    {
        TRACE0("Failed to create ZoomView toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Remove this if you don't want tool tips or a resizeable toolbar
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    m_toolbar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    m_FHZoomViewToolBar.SetBarStyle(m_FHZoomViewToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);


    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable

    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

    m_toolbar.EnableDocking(CBRS_ALIGN_ANY);

    m_FHZoomViewToolBar.EnableDocking(CBRS_ALIGN_ANY);
    //m_FHZoomViewToolBar.SetButtonStyle(2,TBBS_CHECKBOX);

    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);
    DockControlBar(&m_FHZoomViewToolBar);	
    DockControlBar(&m_toolbar);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////    The work space like toolbar
    if (!m_wndWorkspace.Create(this, ID_VIEW_WORKSPACE,
        _T("工作区"), CSize(200,100), CBRS_LEFT))
    {
        TRACE0("Failed to create dialog bar m_wndWorkspace\n");
        return -1;  // fail to create
    }

    // Define the image list to use with the tab control	 
    m_TabImages.Create (m_nWorkSpace_TAB, 16, 1, RGB(0,255,0));
    m_wndWorkspace.SetTabImageList(&m_TabImages);	


    // 可以在此设置菜单
    //m_wndWorkspace.SetMenuID(IDR_DataBase_Popup);

    m_wndWorkspace.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);

    EnableDockingSizeBar(CBRS_ALIGN_ANY);
    DockSizeBar(&m_wndWorkspace);

    m_dbSolution.SetWorkSpace(&m_wndWorkspace);


    // The output bar:
    if (!m_wndOutput.Create(this, ID_VIEW_OUTPUTBAR,
        _T("输出信息"), CSize(300,100),m_wndOutput.GetDockSide()))
    {
        TRACE0("Failed to create dialog bar m_wndOutput\n");
        return -1;		// fail to create
    }
    //m_wndOutput.SetMenuID(IDR_POPUP);
    m_wndOutput.EnableDockingOnSizeBar(CBRS_ALIGN_ANY);
    DockSizeBar(&m_wndOutput);

    m_menuManager.Install(this);
    m_menuManager.LoadToolbar(IDR_FHMAINFRAME); 

    if (CreateTabs() == -1)
        return -1;

    CFHApp *pApp=(CFHApp*)AfxGetApp();
    m_iconFhSoft=pApp->LoadIcon(m_nSoftICON);
    SetIcon(m_iconFhSoft,false);

    // CG: The following line was added by the Splash Screen component.
    CSplashWnd::ShowSplashScreen(this);

    //
    return 0;
}

BOOL CFHMainFrm::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CFHMainFrm diagnostics

#ifdef _DEBUG
void CFHMainFrm::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CFHMainFrm::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFHMainFrm message handlers

void CFHMainFrm::OnWorkspacebar() 
{
    // TODO: Add your command handler code here
    OnBarCheck(ID_VIEW_WORKSPACE);
}

void CFHMainFrm::OnUpdateWorkspacebar(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->m_nID = ID_VIEW_WORKSPACE;
    pCmdUI->Enable();
    OnUpdateControlBarMenu(pCmdUI);	
}

void CFHMainFrm::OnDestroy() 
{
    CMDIFrameWnd::OnDestroy();
}

void CFHMainFrm::OnOutputBar() 
{
    // TODO: Add your command handler code here
    OnBarCheck(ID_VIEW_OUTPUTBAR);
}

void CFHMainFrm::OnUpdateOutputBar(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->m_nID = ID_VIEW_OUTPUTBAR;
    pCmdUI->Enable();
    OnUpdateControlBarMenu(pCmdUI);	
}

BOOL CFHMainFrm::CreateTabs(void) 
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

    m_wndMdiClient.SetPic(m_sBackGroundPic); //(Bitmap(m_nClientBkGround);
    return 0;
}

void CFHMainFrm::ShowBar(int nID,bool bShow)
{
    CControlBar* pBar = GetControlBar(nID);
    if (pBar != NULL)
    {
        ShowControlBar(pBar, bShow, FALSE);
    }
    return ;
}

/////////////////////////////////////////////////////////////////////////////
// CMDITabFrameWnd message handlers
//

void CFHMainFrm::OnWindowNextPane() 
{
    MDINext();	
}

void CFHMainFrm::OnWindowPrevPane() 
{
    ASSERT(::IsWindow(m_hWnd));
    ::SendMessage(m_hWndMDIClient, WM_MDINEXT, 0, 1); 
}

void CFHMainFrm::SetWorkSpaceTab(UINT nNewTabID)
{
    m_nWorkSpace_TAB=nNewTabID;
}


void CFHMainFrm::NewSolution()
{
    CString sSolutionValue=m_dbSolution.CreateSolution();
    if(sSolutionValue!="")
    {
        CString sSolutionName=GetSolutionName();
        ::AddPar(sSolutionName,sSolutionValue);
    }
}

void CFHMainFrm::OpenSolution()
{
    CString sSolutionValue=m_dbSolution.OpenSolution();

    if(sSolutionValue!="")
    {
        CString sSolutionName=GetSolutionName();
        ::AddPar(sSolutionName,sSolutionValue);
    }
}

void CFHMainFrm::SaveSolution()
{
    m_dbSolution.SaveSolution();
}

void CFHMainFrm::UpdateDBSolution(CFHTreeViewBase *pView)
{
    m_dbSolution.UpdateDB(pView);
}

void CFHMainFrm::InsertArea(CString sAreaName)
{
    m_dbSolution.InsertArea(sAreaName);
}

void  CFHMainFrm::RemoveArea(CString sAreaName)
{
    m_dbSolution.RemoveArea(sAreaName);
}


//CWnd * AddToolBarCtrl( )/* ECtrlType     eType,
//                      UINT          iID,
//                      int           iWidth, 
//                      CWnd      *   pUserWnd,
//                      CString       sText)*/
//{
//    //CFHToolBar &bar= ((CFHMainFrm*)AfxGetMainWnd())->m_toolbar;
//    //return bar.AddCtrl( eType,
//    //    iID,
//    //    iWidth, 
//    //    pUserWnd,
//    //    sText);
//    return 0;
//}
void AddToolBarCtrl(    CWnd **       ppAddedCtrl,         
                    ECtrlType     eType,
                    UINT          iID,
                    int           iWidth, 
                    CWnd      *   pUserWnd,
                    CString       sText)
{
    CFHToolBar &bar= ((CFHMainFrm*)AfxGetMainWnd())->m_toolbar;
    *ppAddedCtrl=bar.AddCtrl( eType,
        iID,
        iWidth, 
        pUserWnd,
        sText);

    ((CFHMainFrm*)AfxGetMainWnd())->RecalcLayout();//CFrameWnd::RecalcLayout 

    /*void CFrameWnd::*/
    ((CFHMainFrm*)AfxGetMainWnd())->ShowControlBar(&bar, true, false);



    return;
}

void RemoveToolBarCtrl(UINT          iID,
                       CWnd      *   pUserWnd)
{
    CFHToolBar &bar= ((CFHMainFrm*)AfxGetMainWnd())->m_toolbar;
    bar.RemoveCtrl(iID,pUserWnd);
    ((CFHMainFrm*)AfxGetMainWnd())->RecalcLayout();//CFrameWnd::RecalcLayout

    return;
}



void TRACEERROR(CString sError)
{
#ifdef OUTPUTBAR_EXIST
    char tmpbuf[128];
    if(sError.Left (4)!="错误"&&sError.Left (4)!="警告")
    {
        sError="错误:"+sError;
    }

    _strtime( tmpbuf );
    sError=CString(tmpbuf)+"  "+sError;
    CFHMainFrm *pMainFrm=(CFHMainFrm*)AfxGetMainWnd();
    if(!pMainFrm)
    {
        return;
    }
    COutputBar &bar=pMainFrm->GetOutputBar() ;
    if(IsWindow(bar.m_ListCtrl .m_hWnd ))
    {
        bar.AddItem (0,sError);
    }
    else
    {
        //AfxMessageBox(sError);
    }
#endif
}

void TRACEERROR(CString sError,CString sError2)
{
    TRACEERROR(sError+sError2);
}

void TRACEERROR(CString sError,int nData)
{
    sError+=vtos(nData);
    TRACEERROR(sError);
}

void TRACEERROR(CString sError,long nData)
{
    sError+=vtos(nData);
    TRACEERROR(sError);
}

void TRACEERROR(CString sError,double  dData)
{
    sError+=vtos(dData);
    TRACEERROR(sError);
}

void TRACEERROR(CString sError,float fData)
{
    sError+=vtos(fData);
    TRACEERROR(sError);
}

void TRACEPROGRESS(CString sError,int nData)
{
    sError+=vtos(nData);
    TRACEPROGRESS(sError);
}
void TRACEPROGRESS(CString sError,CString sError2)
{
    TRACEPROGRESS(sError+sError2);
}

void TRACEPROGRESS(CString sError,long nData)
{
    sError+=vtos(nData);
    TRACEPROGRESS(sError);
}

void TRACEPROGRESS(CString sError,double  dData)
{
    sError+=vtos(dData);
    TRACEPROGRESS(sError);
}

void TRACEPROGRESS(CString sError,float fData)
{
    sError+=vtos(fData);
    TRACEERROR(sError);
}

void TRACEPROGRESS(CString sProgress)
{
#ifdef OUTPUTBAR_EXIST
    CFHMainFrm* pWnd=(CFHMainFrm*)AfxGetMainWnd();
    if(pWnd==NULL)
    {
        return;
    }

    if(sProgress.Left (4)!="恭喜")
    {
        sProgress="恭喜:"+sProgress;
    }

    char tmpbuf[128];
    _strtime( tmpbuf );
    sProgress=CString(tmpbuf)+"  "+sProgress;


    COutputBar &bar=((CFHMainFrm*)AfxGetMainWnd())->GetOutputBar() ;
    if(IsWindow(bar.m_ListCtrl .m_hWnd ))
    {
        bar.AddItem (0,sProgress);
    }
    else
    {
    }
#endif
}


void TRACEERROR(UINT nIDS)
{
    CString s;
    s.LoadString (nIDS);
    TRACEERROR(s);
}

void TRACEPROGRESS(UINT nIDS)
{
    CString s;
    s.LoadString (nIDS);
    TRACEPROGRESS(s);
}
void TRACEPROGRESS(UINT nIDS, CString sValue)
{
    CString s;
    s.LoadString (nIDS);
    if(s.Right (1)!=":")
    {
        s+=":";
    }

    TRACEPROGRESS(s+sValue);
}

void TRACEPROGRESS(UINT nIDS,float fValue)
{
    CString sValue=vtos(fValue);
    TRACEPROGRESS(nIDS,sValue);
}

void TRACEPROGRESS(UINT nIDS,long lValue)
{
    CString sValue=vtos(lValue);
    TRACEPROGRESS(nIDS,sValue);
}

void TRACEPROGRESS(UINT nIDS,int iValue)
{
    CString sValue=vtos(iValue);
    TRACEPROGRESS(nIDS,sValue);
}

void TRACEPROGRESS(UINT nIDS,double dValue)
{
    CString sValue=vtos(dValue);
    TRACEPROGRESS(nIDS,sValue);
}


void TRACEERROR(UINT nIDS,float fValue)
{
    CString sValue=vtos(fValue);
    TRACEERROR(nIDS,sValue);
}

void TRACEERROR(UINT nIDS,long lValue)
{
    CString sValue=vtos(lValue);
    TRACEERROR(nIDS,sValue);
}

void TRACEERROR(UINT nIDS,int iValue)
{
    CString sValue=vtos(iValue);
    TRACEERROR(nIDS,sValue);
}

void TRACEERROR(UINT nIDS,double dValue)
{
    CString sValue=vtos(dValue);
    TRACEERROR(nIDS,sValue);
}

void TRACEERROR(UINT nIDS,CString sValue)
{
    CString s;
    s.LoadString (nIDS);
    if(s.Right (1)!=":")
    {
        s+=":";
    }

    TRACEERROR(s+sValue);
}

CCJTabCtrlBar *CFHMainFrm::GetWorkSpace()
{
    return &m_wndWorkspace;
}


// Open recent solution or create a new one:
CString CFHMainFrm::OpenRecentSolution(CString sRecentSolution)
{
    if(sRecentSolution!="")
    {
        if(!m_dbSolution.OpenSolution(sRecentSolution))
        {
            if(!m_dbSolution.CreateSolution())
            {
                return CString("");
            };
        }
        return sRecentSolution;
    }
    else
    {
        CString sFile=m_dbSolution.CreateSolution();
        return sFile;
    }
}

void CFHMainFrm::AddLine(   CString sArea, 
                         CString sLineName,
                         CRuntimeClass *pRuntimeClass)
{
    return;
}

void  CFHMainFrm::AddReport(CString sArea, CString sReportName)
{
    return;
}

CString CFHMainFrm::GetSolutionName()
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    return "FHMAINFRAME_RecentSolution";
}

BOOL CFHMainFrm::PreTranslateMessage(MSG* pMsg)
{
    int n=TranslateAccelerator(m_hWnd,m_accel,pMsg);
    if(n)
    {
        return true;
    }

    return CCJMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CFHMainFrm::SetAccelerator(int nAcceleratorRes, HINSTANCE hInstance)
{
    m_accel=LoadAccelerators(hInstance,MAKEINTRESOURCE(nAcceleratorRes)); 
}

BOOL CFHMainFrm::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: Add your specialized code here and/or call the base class

    return CCJMDIFrameWnd::OnCommand(wParam, lParam);
}


void AddHistory(EHistory eAction, 
                CString sTargetName)
{
    CTime t     = CTime::GetCurrentTime();
    int iYear   = t.GetYear();
    int iMonth  = t.GetMonth();
    int iDay    = t.GetDay();
    int iHour   = t.GetHour();
    int iMinute = t.GetMinute();
    int iSecond = t.GetSecond();

    CHistory history;
    history.SetAction(  eAction,
                        sTargetName,
                        CMyDate(iYear,iMonth,iDay),
                        CMyTime(iHour,iMinute,iSecond));

    gHistoryManager.Add(history);

}
