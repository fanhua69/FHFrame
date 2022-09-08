
#include "StdAfx.h"
#include "dbsolution.h"
#include "fhtreeviewbase.h"
#include "fhcglobal.h"
#include "CJTabCtrlBar.h"
#include "direct.h"
#include "fhapp.h"


CDBSolution::CDBSolution(void)
{
    m_pCtrlBar=NULL;
    m_sSolutionMark="DataBaseSolution";
    m_sSolutionFile="";
    m_pCtrlBar=NULL;
}


CDBSolution::~CDBSolution(void)
{
    ClearDB();
}

void CDBSolution::ClearDB()
{
    for(int i=0;i<m_vecDBNames.size();i++)
    {
        CDBSolutionLeaf *p=m_vecDBNames.at(i);
        delete p;
    }
    m_vecDBNames.clear();
}

void CDBSolution::SetWorkSpace(CCJTabCtrlBar *pCtrlBar)
{
    m_pCtrlBar=pCtrlBar;
}

CString CDBSolution::OpenSolution(CString sFile)
{
    if(!m_pCtrlBar)
    {
        return CString("");
    }

    //
    if(sFile=="")
    {
        sFile=GetFileForOpen("数据库解决方案(*.sol)","sol");
        if(sFile=="")
        {
            return CString("");
        }
    }

    //
    FILE *fp=fopen(sFile,"rt");
    if(!fp)
    {
        AfxMessageBox("无法打开数据库方案:"+sFile);
        return CString("");
    }
    
    //
    char sLine[1000];
    fscanf(fp,"%s\n",sLine);
    CString s=sLine;
    if(s!=m_sSolutionMark)
    {
        fclose(fp);
        return CString("");
    }

    //
    ClearDB();

    int nProjectQty=0;
    fscanf(fp,"%d\n",&nProjectQty);


    CDBSolutionLeaf *pDB=NULL;
    char sDBExt[1000];
    char sDBPath[1000];
    for(int i=0;i<nProjectQty;i++)
    {
        memset(sDBExt,0,1000);
        fscanf(fp,"%s\n",sDBExt);

        memset(sDBPath,0,1000);
        fgets(sDBPath,1000,fp);
        ::RemoveInvalidChar(sDBPath);

        pDB=new CDBSolutionLeaf;
        pDB->sDBNameExt=sDBExt;
        pDB->sDBPathName=sDBPath;

        m_vecDBNames.push_back(pDB);
    }

    fclose(fp);

    m_sSolutionFile=sFile;

    CreateTrees();

    return m_sSolutionFile;
}

CString CDBSolution::GetTreePathName(CFHTreeViewBase *pTree)
{
    CString sDBNameExt=pTree->GetDataBaseNameExt();

    CDBSolutionLeaf *pDB=NULL;
    for(int i=0;i<m_vecDBNames.size();i++)
    {
        pDB=m_vecDBNames.at(i);
        if(pDB->sDBNameExt==sDBNameExt)
        {
            return pDB->sDBPathName;
        }
    }
    return CString("");
}


CString CDBSolution::CreateSolution()
{
    CString sFile=GetFileForSave("新物探监理数据库方案(*.sol)","sol");
    if(sFile=="")
    {
        return CString("");
    }

    m_sSolutionFile=sFile;

    //
    _mkdir((LPCTSTR)GetSolutionPath());

    //
    ClearDB();

    CreateTrees();

    return m_sSolutionFile;
}

CString CDBSolution::GetSolutionName()
{
    return m_sSolutionFile;
}

CString CDBSolution::GetSolutionPath()
{
    return ::SeperatePathName(m_sSolutionFile);
}

void  CDBSolution::CreateTrees()
{
    if(!m_pCtrlBar)
    {
        return;
    }

    CDBSolutionLeaf *pDB=NULL;

    CString sSolutionPath=GetSolutionPath();
    int iViewOrder=1;
    while(true)
    {
        CFHTreeViewBase *pTree=(CFHTreeViewBase*)(m_pCtrlBar->GetView(iViewOrder));
        if(!pTree)
        {
            break;
        }

        CString sTreePathName=GetTreePathName(pTree);
        if(sTreePathName=="")
        {
            pDB=new CDBSolutionLeaf;
            pDB->sDBNameExt=pTree->GetDataBaseNameExt();
            pDB->sDBPathName=pTree->CreateDataBase(sSolutionPath);
            m_vecDBNames.push_back(pDB);
        }
        else
        {
            pTree->OpenDataBase(sTreePathName);
        }

        iViewOrder++;
    }

    //
    SaveSolution(GetSolutionName());

    return;
}

bool CDBSolution::SaveSolution(CString sFile)
{
    if(sFile=="")
    {
        sFile=m_sSolutionFile;
        if(sFile=="")
        {
            sFile=GetFileForSave("数据库解决方案(*.sol)","sol");
            if(sFile=="")
            {
                return false ;
            }
        }
    }

    FILE *fp=fopen(sFile,"wt");
    if(!fp)
    {
        return false;
    }

    fprintf(fp,"%s\n",(LPCTSTR)m_sSolutionMark);

    int nProjectQty=m_vecDBNames.size();
    fprintf(fp,"%d\n",nProjectQty);
    for(int i=0;i<nProjectQty;i++)
    {
        CDBSolutionLeaf *pDB=m_vecDBNames.at(i);
        fprintf(fp,"%s\n",(LPCTSTR)pDB->sDBNameExt);
        fprintf(fp,"%s\n",(LPCTSTR)pDB->sDBPathName);
    }

    fclose(fp);

    m_sSolutionFile=sFile;

    return true;

}

void  CDBSolution::InsertArea(CString sAreaName)
{
    int iViewOrder=0;
    while(true)
    {
        CView *pView=m_pCtrlBar->GetView(iViewOrder);
        if(!pView)
        {
            break;
        }
        if(pView->IsKindOf(RUNTIME_CLASS(CFHTreeViewBase)))
        {
            CFHTreeViewBase *pTreeView=(CFHTreeViewBase*)(m_pCtrlBar->GetView(iViewOrder));
            if(!pTreeView)
            {
                break;
            }
            pTreeView->InsertArea(sAreaName);
        }
        iViewOrder++;
    }

    SaveSolution();

}
void  CDBSolution::RemoveArea(CString sArea)
{
    int iViewOrder=1;
    while(true)
    {
        CFHTreeViewBase *pView=(CFHTreeViewBase*)(m_pCtrlBar->GetView(iViewOrder));
        if(!pView)
        {
            break;
        }
        pView->RemoveArea(sArea);
        iViewOrder++;
    }

    SaveSolution();


    return ;
}

void  CDBSolution::UpdateDB(CFHTreeViewBase *pTree)
{
    CString sDBNameExt=pTree->GetDataBaseNameExt();

    bool bUpdated=false;
    CDBSolutionLeaf *pDB=NULL;
    int nQty=m_vecDBNames.size();
    for(int i=0;i<nQty;i++)
    {
        pDB=m_vecDBNames.at(i);
        if(pDB->sDBNameExt==sDBNameExt)
        {
            pDB->sDBPathName=pTree->ConstructDataBaseName();
            bUpdated=true;
            break;
        }
    }

    if(!bUpdated)
    {
        pDB=new CDBSolutionLeaf;
        pDB->sDBNameExt=pTree->GetDataBaseNameExt();
        pDB->sDBPathName=pTree->ConstructDataBaseName();
        m_vecDBNames.push_back(pDB);
    }

    SaveSolution();

    return;
}