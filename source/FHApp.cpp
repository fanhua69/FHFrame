// FHApp.cpp : implementation file
//

#include "stdafx.h"
#include "FHFrame.h"
#include "FHApp.h"
#include "splash.h"
#include "direct.h"
#include "FHDocManager.h"
#include "FHMainFrm.h"
#include "childfrm.h"



// CFHApp


IMPLEMENT_DYNCREATE(CFHApp, CWinApp)

CFHApp::CFHApp()
{
    CSplashWnd::SetCover (IDB_JLGS_BK);

    m_sCurrentBlock="";

    if( _getcwd(m_sWorkDir.GetBuffer(128), 128) == NULL )
    {
        AfxMessageBox("_getcwd error: Can NOT get current working directory!" );
    }
    m_sWorkDir.ReleaseBuffer();

    m_bHelpIsShowing=false;
}

CFHApp::~CFHApp()
{
    RemoveAllTemplates();
    RemoveTempFiles();
}

CParManager		parManager;

bool CFHApp::RemoveTemplate(CString regFileTypeId)
{
    ///////////////////////////////////////////////
    // 删除DocManager中的模版
    ///////////////////////////////////////////////
    CFHDocManager *pManager=(CFHDocManager *)m_pDocManager;
    (CMultiDocTemplate *)pManager->RemoveTemplate (regFileTypeId);

    ///////////////////////////////////////////////
    // 删除m_templateString中的字符串
    ///////////////////////////////////////////////
    POSITION pos=m_templateString.GetHeadPosition ();
    POSITION posBak;
    CString s;
    while(pos)
    {
        posBak=pos;
        s=m_templateString.GetNext (pos);
        if(s==regFileTypeId)
        {
            m_templateString.RemoveAt (posBak);
        }
    }

    return true;
}

BOOL CFHApp::InitInstance()
{

    __super::InitInstance();

    // TODO:  perform and per-thread initialization here
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Get the started directory:
    parManager.ReadPar();

    // CG: The following block was added by the Splash Screen component.
    \
    {
        \
            CCommandLineInfo cmdInfo;
        \
            ParseCommandLine(cmdInfo);
        \

            \
            CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
        \

    }

    // 安装模板
    if(m_pDocManager )
    {
        delete m_pDocManager ;
        m_pDocManager =NULL;
    }
    else
    {
        m_pDocManager =new CFHDocManager;
    }

    return TRUE;
}

int CFHApp::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    return CWinApp::ExitInstance();
}

_declspec(dllexport) char AfxCurrentBlock[];

BEGIN_MESSAGE_MAP(CFHApp, CWinApp)
END_MESSAGE_MAP()


// CFHApp message handlers
CMultiDocTemplate * CFHApp::GetTemplate(CString regFileTypeId)
{
    CFHDocManager *pManager=(CFHDocManager *)m_pDocManager;
    return (CMultiDocTemplate *)pManager->GetTemplate (regFileTypeId);
}

bool CFHApp::RemoveAllTemplates()
{
    CloseAllDocuments(false);
    CFHDocManager *pManager=(CFHDocManager *)m_pDocManager;
    pManager->RemoveAllTemplates ();
    m_templateString.RemoveAll ();

    return true;
}

bool CFHApp::AddTree(CTreeView *pTree,CString sType)
{
    return m_treeManager.AddTree (pTree,sType);
}

CTreeView * CFHApp::GetTree(CString sType)
{
    return m_treeManager.GetTree (sType);
}

bool CFHApp::RemoveTree(CString sType)
{
    return m_treeManager.RemoveTree (sType);
}

bool CFHApp::ExistTree(CString sType)
{
    return m_treeManager.ExistTree (sType);
}

void CFHApp::EmptyTree()
{
    return m_treeManager.EmptyTree ();
}

bool  CFHApp::SetParFile(CString sFile)
{
    return parManager.SetParFile(sFile);
}

CString GetWorkDir()
{
    CFHApp *pApp=(CFHApp*)AfxGetApp();
    if(pApp->IsKindOf (RUNTIME_CLASS(CFHApp)))
    {
        return pApp->GetWorkDir ();
    }
    else
    {
        return CString("");
    }
}

void AddMyTempPath(CString sPath)
{
    if(sPath.Right (1)!="\\")
    {
        sPath+="\\";
    }

    AddPar("GLOBAL_TEMP_PATH",sPath);
}

CString GetMyTempPath()
{
    CString sValue;
    char sPath[1000];
    memset(sPath,0,1000);
    GetPar("GLOBAL_TEMP_PATH",sValue);
    if(sValue=="" || sValue.Mid(1,2)!=":\\")
    {
        GetTempPath(1000,sPath);
        if(strcmp(sPath,"")==0)
        {
            strcpy(sPath,"c:\\");
        }
        sValue=sPath;
    }

    if(sValue.Right (1)!="\\")
    {
        sValue+="\\";
    }

    return sValue;
}


void AddUserNameShort(CString sUserNameShort)
{
    AddPar("QCDB_USERNAME_SHORT",sUserNameShort);
}

CString GetUserNameShort()
{
    CString sUserNameShort;
    GetPar("QCDB_USERNAME_SHORT",sUserNameShort);
    return sUserNameShort;
}

int iTempFileOrder=1000;

void SetTempFileStartNo(int iNo)
{
    iTempFileOrder=iNo;
}

CString GetTempFile()
{
    char s[100];
    memset(s,0,100);
    sprintf(s,"%stemp%d",GetUserNameShort(),iTempFileOrder);
    CString sFile=GetMyTempPath()+s;
    iTempFileOrder++;

    return sFile;
}

void RemoveTempFiles()
{
    char s[100];
    CString sFile;
    CString sPath=GetMyTempPath();

    int n=iTempFileOrder;
    if(n<1100)
    {
        n=1100;
    }
    for(int i=1000;i<n;i++)
    {
        sprintf(s,"temp%d",i);
        sFile=sPath+s;
        RemoveFile(sFile);
    }
}

CMultiDocTemplate * GetTemplate(CString sTemplate)
{
    CFHApp *pApp=(CFHApp *)AfxGetApp();
    CMultiDocTemplate *pTemplate=(CMultiDocTemplate*)pApp->GetTemplate (sTemplate);
    return pTemplate;
}

bool OpenTextFile(CString sFile)
{
    if(sFile=="")return false;

    CMultiDocTemplate *pTemplate=GetTemplate("TextEdit");
    if(!pTemplate)return false;
    return pTemplate->OpenDocumentFile (sFile);
}


bool GetPar(CString sName, CString &sValue)
{
    return parManager.GetPar (sName,sValue);
}

bool GetPar(char sName[], int &iValue)
{
    return parManager.GetPar (sName,iValue);
}

bool GetPar(char sName[], UINT &iValue)
{
    return parManager.GetPar (sName,iValue);
}

bool GetPar(char sName[], long &lValue)
{
    return parManager.GetPar (sName,lValue);
}
bool GetPar(char sName[], double &dValue)
{
    return parManager.GetPar (sName,dValue);
}
bool GetPar(char sName[], float &fValue)
{
    return parManager.GetPar (sName,fValue);
}

bool GetPar(CString sName, int &iValue)
{
    return parManager.GetPar (sName,iValue);
}

bool GetPar(CString sName, UINT  &iValue)
{
    return parManager.GetPar (sName,iValue);
}

bool GetPar(CString sName, long &lValue)
{
    return parManager.GetPar (sName,lValue);
}

bool GetPar(CString sName, double &dValue)
{
    return parManager.GetPar (sName,dValue);
}

bool GetPar(CString sName, float &fValue)
{
    return parManager.GetPar (sName,fValue);
}

bool GetPar(CString sName, bool &bValue)
{
    int iValue=0;
    if(!parManager.GetPar(sName,iValue))return false;
    if(iValue>0)
    {
        bValue=true;
    }
    else
    {
        bValue=false;
    }

    return true;
}

int GetParQty(CString sName)
{
    return parManager.GetParQty(sName);
}

int GetParQty(char sName[])
{
    return parManager.GetParQty(sName);
}

char * GetMorePar(int iOrder)
{
    return parManager.GetMorePar (iOrder);
}

char *  GetPar(char sName[])
{
    return parManager.GetPar (sName);
}


void AddPar(CString sName, CString sValue)
{
    parManager.AddPar (sName,sValue);
}

void AddPar(char sName[], int iValue)
{
    parManager.AddPar (sName,iValue);
}

void AddPar(char sName[], CString sValue)
{
    AddPar(CString(sName),sValue);
}

void AddPar(char sName[], bool bValue)
{
    AddPar(CString(sName),bValue);
}

void AddPar(CString sName, bool bValue)
{
    int iValue=0;
    if(bValue)
    {
        iValue=1;
    }
    else
    {
        iValue=0;
    }
    return parManager.AddPar(sName,iValue);
}

void AddPar(char sName[], long lValue)
{
    parManager.AddPar (sName,lValue);
}

void AddPar(char sName[], double dValue)
{
    parManager.AddPar (sName,dValue);
}

void AddPar(char sName[], float fValue)
{
    parManager.AddPar (sName,fValue);
}

void AddPar(CString sName, int iValue)
{
    parManager.AddPar (sName,iValue);
}

void AddPar(char sName[], UINT iValue)
{
    parManager.AddPar (sName,iValue);
}


void AddPar(CString sName, UINT iValue)
{
    parManager.AddPar (sName,iValue);
}

void AddPar(CString sName, long lValue)
{
    parManager.AddPar (sName,lValue);
}
void AddPar(CString sName, double dValue)
{
    parManager.AddPar (sName,dValue);
}
void AddPar(CString sName, float fValue)
{
    parManager.AddPar (sName,fValue);
}

void AddPar(char sName[], char sValue[])
{
    parManager.AddPar (sName,sValue);
}

void AddMorePar(CString sName, CString sValue)
{
    parManager.AddMorePar (sName.GetBuffer(),sValue.GetBuffer());
}

void AddMorePar(char sName[], int iValue)
{
    parManager.AddMorePar (sName,iValue);
}

void AddMorePar(char sName[], CString sValue)
{
    AddMorePar(CString(sName),sValue);
}

void AddMorePar(CString sName,CStringArray &arrayValues)
{
    CString sValue;
    for(int i=0;i<arrayValues.GetCount();i++)
    {
        sValue=arrayValues.GetAt(i);
        AddMorePar(sName,sValue);
    }
}

void AddMorePar(char sName[], bool bValue)
{
    AddMorePar(CString(sName),bValue);
}

void AddMorePar(CString sName, bool bValue)
{
    int iValue=0;
    if(bValue)
    {
        iValue=1;
    }
    else
    {
        iValue=0;
    }
    return parManager.AddMorePar(sName,iValue);
}

void AddMorePar(char sName[], long lValue)
{
    parManager.AddMorePar (sName,lValue);
}

void AddMorePar(char sName[], double dValue)
{
    parManager.AddMorePar (sName,dValue);
}

void AddMorePar(char sName[], float fValue)
{
    parManager.AddMorePar (sName,fValue);
}

void AddMorePar(CString sName, int iValue)
{
    parManager.AddMorePar (sName,iValue);
}
void AddMorePar(CString sName, long lValue)
{
    parManager.AddMorePar (sName,lValue);
}

void AddMorePar(CString sName, double dValue)
{
    parManager.AddMorePar (sName,dValue);
}
void AddMorePar(CString sName, float fValue)
{
    parManager.AddMorePar (sName,fValue);
}

void AddMorePar(char sName[], char sValue[])
{
    parManager.AddMorePar (sName,sValue);
}
void RemovePar(CString sName)
{
    parManager.RemovePar (sName.GetBuffer ());
    sName.ReleaseBuffer ();
}

void RemovePar(char sName[])
{
    parManager.RemovePar (sName);
}

CString GetString(UINT nIDString)
{
    CString s;
    s.LoadString (nIDString);
    return s;
}

bool IsValid(const CObject* pOb)
{
    if (pOb == NULL)
    {
        return false;     // quick escape
    }
    if (!AfxIsValidAddress(pOb, sizeof(CObject)))
    {
        return false;     // quick escape
    }

    // check to make sure the VTable pointer is valid
    if(sizeof(CObject) != sizeof(void*))return false;
    if (!AfxIsValidAddress(*(void**)pOb, sizeof(void*), FALSE))
    {
        return false;     // quick escape
    }

    if (!AfxIsValidAddress(pOb, pOb->GetRuntimeClass()->m_nObjectSize, FALSE))
    {
        return false;     // quick escape
    }

    return true;
    // pOb->AssertValid();  removed this by fanhua
}

bool CFHApp::AddTemplate(CMultiDocTemplate *pTemplate)
{
    ///////////////////////////////////
    // 检查该模版是否已经在列表中
    ///////////////////////////////////
    CString sString;
    pTemplate->GetDocString (sString,CDocTemplate::regFileTypeId);
    if(sString=="")return false;

    CString s;
    POSITION pos=m_templateString.GetHeadPosition ();
    while(pos)
    {
        s=m_templateString.GetNext (pos);
        if(s==sString)
        {
            return true;
        }
    }

    ///////////////////////////////////
    // 加入该模版到列表中
    ///////////////////////////////////
    m_templateString.AddTail (sString);
    AddDocTemplate (pTemplate);

    return true;
}

bool GetMorePar(CString sName,CStringArray &array)
{
    int iQty=GetParQty(sName);
    if(iQty==0)
    {
        return false;
    }

    array.RemoveAll ();
    array.SetSize(iQty);
    char *pPar=NULL;
    CString sValue;
    for(int i=0;i<iQty;i++)
    {
        pPar=GetMorePar(i);
        sValue=pPar;
        if(sValue =="")
        {
            continue;
        }
        array.SetAt (i,sValue);
    }

    return true;
};


void AddInstance(CString sInstanceName,HINSTANCE hInstance)
{
    AddPar(sInstanceName+"_DLL",(UINT)hInstance);
}

void GetInstance(CString sInstanceName,HINSTANCE &hInstance)
{
    UINT n=0;
    GetPar(sInstanceName+"_DLL",n);
    hInstance=(HINSTANCE)n;
}



void AddUserName(CString sUser)
{
    if(sUser=="")
    {
        TRACEERROR("用户名为空,无法记录!");
        return;
    }

    AddPar("QC_MAIN_USERNAME",sUser);

}

CString GetUserName()
{
    CString sUser="";
    GetPar("QC_MAIN_USERNAME",sUser);
    if(sUser=="")
    {
        TRACEERROR("用户名尚未填写!");
    }

    return sUser;
}

void AddProjectName(CString sProjectName)
{
    if(sProjectName=="")
    {
        TRACEERROR("项目名为空,无法记录!");
        return;
    }

    AddPar("QC_MAIN_PROJECTNAME",sProjectName);
}

CString GetProjectName()
{
    CString sProjectName="";
    GetPar("QC_MAIN_PROJECTNAME",sProjectName);
    if(sProjectName=="")
    {
        TRACEERROR("项目名尚未填写!");
    }

    return sProjectName;
}


int giUniIDOrder=0;
CString gsProjectID="";
CString gsUiIDTemp="";


CString GetUniID()
{
    static CString sCurrID;
    static CTime time;
    static CString sYear;
    static CString sMonth;
    static CString sDay;
    static CString sHour;
    static CString sMinute;
    //static long long nnTime;
    //static CString sSecond;

    CString sUserNameShort;
    sUserNameShort=GetUserName();

    sCurrID=sUserNameShort;
    sCurrID.MakeUpper ();

    time=CTime::GetCurrentTime ();

    /*unsigned long long nnTime=0;
    int iYear=time.GetYear ();
    nnTime=time.GetYear ()*(long long)100000000;
    nnTime+=time.GetMonth ()*1000000;
    nnTime+=time.GetDay ()*10000;
    nnTime+=time.GetHour ()*100;
    nnTime+=time.GetMinute ();

    sCurrID.Format ("%s%ulld",sUserNameShort,nnTime);
    */
    int iYear		=time.GetYear ();
    int iMonth		=time.GetMonth ();
    int iDay		=time.GetDay ();
    int iHour		=time.GetHour ();
    int iMinute		=time.GetMinute ();
    CString s;
    s.Format ("%d",iYear);
    sCurrID+=s;

    s.Format ("%d",iMonth);
    while(s.GetLength ()<2)
    {
        s="0"+s;
    }
    sCurrID+=s;

    s.Format ("%d",iDay);
    while(s.GetLength ()<2)
    {
        s="0"+s;
    }
    sCurrID+=s;

    s.Format ("%d",iHour);
    while(s.GetLength ()<2)
    {
        s="0"+s;
    }
    sCurrID+=s;

    s.Format ("%d",iMinute);
    while(s.GetLength ()<2)
    {
        s="0"+s;
    }
    sCurrID+=s;

    if(sCurrID.Left(12)==gsUiIDTemp.Left(12))
    {
        giUniIDOrder++;
    }
    else
    {
        giUniIDOrder=0;
    }
    sCurrID+=vtos(giUniIDOrder);

    gsUiIDTemp=sCurrID;


    return sCurrID;
}


void CopyCStringList(CStringList &ListFrom, CStringList &listTo)
{
    listTo.RemoveAll();
    CString sName;
    POSITION pos=ListFrom.GetHeadPosition();
    while(pos)
    {
        sName=ListFrom.GetNext(pos);
        listTo.AddTail(sName);
    }
}
