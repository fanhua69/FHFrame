// FHToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "fhframe.h"
#include "FHToolBar.h"


// CFHToolBar

IMPLEMENT_DYNAMIC(CFHToolBar, CToolBar)
CFHToolBar::CFHToolBar()
{
}

CFHToolBar::~CFHToolBar()
{
    CToolProperty p;
    int i;
    for(i=0;i<m_vecToolProperty.size();i++)
    {
        p=m_vecToolProperty.at(i);
        if(p.m_pCtrl!=NULL)
        {
            delete p.m_pCtrl;
            p.m_pCtrl=NULL;
        }
    }
}


BEGIN_MESSAGE_MAP(CFHToolBar, CToolBar)
END_MESSAGE_MAP()



// CFHToolBar message handlers
void CFHToolBar::RemoveCtrl(UINT iID, CWnd *pUserWnd)
{
    // Get the control:
    int iPos=GetCtrl(iID);
    if(iPos==-1)
    {
        return;
    }

    // remove the window from the recording list:
    CToolProperty &p=m_vecToolProperty.at(iPos);

    std::vector <CWnd *>::iterator Iter;
    for ( Iter = p.m_vecWnds.begin( ) ; Iter != p.m_vecWnds.end( ) ; Iter++ )
    {
        CWnd *pWndIn=*Iter;
        if(pWndIn==pUserWnd)
        {
            p.m_vecWnds.erase(Iter);
            break;
        }       
    }

    // No windows is using this button:
    if(p.m_vecWnds.empty())
    {
        p.m_iCtrlID=-1;
        delete p.m_pCtrl;
        p.m_pCtrl=NULL;
        SetButtonInfo(iPos,iID,TBBS_SEPARATOR,0);
    }

    //
    RecalcPos();


    // NO buttons existed in the toolbar already:
    int nCount=GetCount();

    bool bExist=false;
    for(int i=0;i<nCount;i++)
    {
        CToolProperty &p=m_vecToolProperty.at(i);
        if(p.m_pCtrl!=NULL)
        {
            bExist=true;
            break;
        }
    }

    if(!bExist)
    {
        ShowWindow(SW_HIDE);
    }
}


BOOL CFHToolBar::RecalcPos()
{
    int nCount=GetCount();

    CToolProperty p;
    int i,j;
    int nPos;
    for(i=0;i<nCount;i++)
    {
        CToolProperty &p=m_vecToolProperty.at(i);
        if(p.m_pCtrl!=NULL)
        {
            //
            nPos=RequestPos();
            if(nPos>=i)
            {
                continue;
            }

            // Move the Ctrl to the new position:
            PasteCtrl(p.m_eType,p.m_pCtrl,p.m_iCtrlID,p.m_iWidth,p.m_vecWnds[0]);
            
            // Attach the user windows to the moved ctrl:
            for(j=1;j<p.m_vecWnds.size();j++)
            {
                AddCtrl(p.m_eType,p.m_iCtrlID,p.m_iWidth,p.m_vecWnds.at(j));
            }

            //
            p.Reset();
        }
    }

    return true;
}


BOOL CFHToolBar::LoadToolBar(UINT nIDResource)
{
    if(!CToolBar::LoadToolBar(nIDResource))
    {
        return false;
    }

    int nCount=GetCount();
    
    CToolProperty p;
    int i;
    for(i=0;i<nCount;i++)
    {
        p.m_iToolOrder=i;
        p.m_iCtrlID=-1;
        p.m_pCtrl=NULL;
        p.m_vecWnds.clear();
        
        m_vecToolProperty.push_back(p);
    }

    for(i=0;i<nCount;i++)
    {
        SetButtonInfo(i,0,TBBS_SEPARATOR,10);
    }

    SetButtonInfo(0,0,TBBS_BUTTON,0);
    SetSizes(CSize(8,23),CSize(8-7,16));


    this->ShowWindow(SW_HIDE);

    return true;
}

int  CFHToolBar::RequestPos()
{
    int nCount=GetCount();

    CToolProperty p;
    for(int i=1;i<nCount;i++)
    {
        p=m_vecToolProperty.at(i);
        if(p.m_pCtrl==NULL)
        {
            return i;
        }
    }

    return -1;
}

int CFHToolBar::GetCtrl(UINT iID)
{
    CToolProperty p;
    for(int i=0;i<m_vecToolProperty.size();i++)
    {
        p=m_vecToolProperty.at(i);
        if(p.m_iCtrlID==iID)
        {
            return i;
        }
    }
    return -1;
}

CWnd *CFHToolBar::AddCtrl(ECtrlType     eType,
                          UINT          iID,
                          int           iWidth, 
                          CWnd      *   pUserWnd,
                          CString       sText)
{
    int nPos=GetCtrl(iID);
    if(nPos!=-1)
    {
        ShowWindow(SW_SHOW);
        m_vecToolProperty.at(nPos).m_vecWnds.push_back(pUserWnd);
        return m_vecToolProperty.at(nPos).m_pCtrl;
    }

    CWnd *pCtrl=NULL;

    if(eType==eComboBox)
    {
        pCtrl= new CComboBox;
        ((CComboBox*)pCtrl)->Create(  WS_CHILD
                                    |WS_VISIBLE
                                    |WS_VSCROLL 
                                    |CBS_DROPDOWNLIST
                                    |CBS_AUTOHSCROLL,
                                    CRect(0,0,10,10),
                                    this,
                                    iID);
    }

    else if(eType==eEdit)
    {
        pCtrl= new CEdit;
        ((CEdit*)pCtrl)->Create(    WS_CHILD
                                    |WS_VISIBLE
                                    |CBS_AUTOHSCROLL
                                    |WS_BORDER,
                                    CRect(0,0,10,10),
                                    this,
                                    iID);

    }

    else if(eType==eStatic)
    {
        //
        pCtrl=new CStatic;
        ((CStatic*)pCtrl)->Create(sText,
                                            WS_CHILD
                                            |WS_VISIBLE
                                            |CBS_AUTOHSCROLL
                                            |SS_RIGHT,
                                            CRect(0,0,100,100),
                                            this,
                                            iID);
        CDC *pDC=pCtrl->GetDC();
        CSize size=pDC->GetTextExtent(sText);
        iWidth=size.cx+2;
    }

    else if(eType==eCheckBox)
    {
        //
        if(!AddCtrl(eStatic,0,0,pUserWnd, sText))
        {
            return NULL;
        };

        //
        pCtrl= new CButton;
        ((CButton*)pCtrl)->Create(          sText,
                                            WS_CHILD
                                            |WS_VISIBLE
                                            |CBS_AUTOHSCROLL
                                            |BS_LEFTTEXT 
                                            |BS_AUTOCHECKBOX ,
                                            CRect(0,0,100,100),
                                            this,
                                            iID);
        iWidth=20;
    }
    
    if(!PasteCtrl(eType, pCtrl, iID, iWidth,pUserWnd))
    {
        delete pCtrl;
        pCtrl=NULL;
    };

    return pCtrl;
}

bool CFHToolBar::PasteCtrl(ECtrlType    eType, 
                           CWnd         *pWnd, 
                           int          iID, 
                           int          iWidth, 
                           CWnd         *pUserWnd)
{
    int nPos=RequestPos();
    if(nPos==-1)
    {
        return false;
    }

    if(iWidth<8)
    {
        iWidth=8;
    }

    //if(nPos==0)
    //{
    //    SetButtonInfo(nPos,iID,TBBS_BUTTON,iWidth);
    //    SetSizes(CSize(iWidth,23),CSize(iWidth-7,16));
    //}
    //else
    {
        SetButtonInfo(nPos,iID,TBBS_SEPARATOR,iWidth);
    }

    CRect rect,rectHead;
    GetItemRect(nPos,&rectHead);
    rect=rectHead;

    if(pWnd->IsKindOf(RUNTIME_CLASS(CComboBox)))
    {
        rect.top=1;
        rect.bottom=200;

        rect.left+=1;
        rect.right+=1;
    }

    else
    {
        rect.top+=1;
        rect.bottom-=1;

        rect.left+=1;
        rect.right-=1;
    }



    pWnd->SetParent(this);
    pWnd->MoveWindow(&rect);


    CToolProperty &p=m_vecToolProperty.at(nPos);
    p.m_pCtrl       =pWnd;
    p.m_iCtrlID     =iID;
    p.m_iToolOrder  =nPos;
    p.m_eType       =eType;
    p.m_iWidth      =iWidth;//rect.Width();

    p.m_vecWnds.push_back(pUserWnd);

    ShowWindow(SW_SHOW);


    return true;
}
