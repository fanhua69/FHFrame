
#include "stdafx.h"
#include "historymanager.h"

const int nWordLen=31;
const int nHistoryLen=128;

CHistory::CHistory()
{
    memset(this,0,sizeof(CHistory));
}

CHistory::CHistory(const CHistory &other)
{
    memcpy(this, &other, sizeof(CHistory));
}


void CHistory::operator = (const CHistory &other)
{
    memcpy(this, &other, sizeof(CHistory));
    return;
}

bool CHistory::operator == (CHistory &other)
{
    return memcpy(this, &other, sizeof(CHistory))==0;
}

void CHistory::SetAction(   EHistory    eAction,
                            CString     sActionTarget,
                            CMyDate     date,
                            CMyTime     time)
{
    m_eHistory=eAction;
    if(sActionTarget.GetLength()>nHistoryLen-1)
    {
        sActionTarget=sActionTarget.Right(nHistoryLen-1);
    }
    memcpy(m_sTargetName,(LPCTSTR)sActionTarget,sActionTarget.GetLength());
    m_date=date;
    m_time=time;

    return;
}

CString CHistory::GetHistoryName()
{
    switch(m_eHistory)
    {
    case    eStartSoftware:
        return CString ("打开程序");
    case    eStartModule:
        return CString ("打开模块");
    case    eOpenFile:
        return CString ("打开文件");
    case    eSaveFile:
        return CString ("存储文件");
    case    eExportReport:
        return CString ("输出报告");
    case    eEndSoftware:
        return CString ("结束程序");
    case    eDrawAreaMap:
        return CString ("绘制工区图");
    case    eDrawLineData:
        return CString ("显示测线数据");
    case    eDrawLineMap:
        return CString ("绘制测线图形");
    case    eAddArea:
        return CString ("增加工区");
    case    eAddLine:
        return CString ("增加测线");
    case    eRemoveArea:
        return CString ("删除工区");
    case    eRemoveLine:
        return CString ("删除测线");
    default:
        ASSERT(false);
    }
}


void CHistory::Encrypt()
{
    int i=0;
    int iSize=sizeof(CHistory);
    unsigned char c;
    unsigned char *pHistory=(unsigned char *)(this);

    for(i=0;i<iSize;i++)
    {
        c=pHistory[i];
        c-=27;
        if(c<0)
        {
            c+=256;
        }
        pHistory[i]=c;
    }
}

void CHistory::Decrypt()
{
    int i=0;
    int iSize=sizeof(CHistory);
    unsigned char c;
    unsigned char *pHistory=(unsigned char *)this;

    for(i=0;i<iSize;i++)
    {
        c=pHistory[i];
        c+=27;
        if(c>256)
        {
            c-=256;
        }
        pHistory[i]=c;
    }
}


CArchive  & CHistory::Write(CArchive &ar)
{
    ar.Write(this,sizeof(CHistory));
    return ar;
}

CArchive  & CHistory::Read(CArchive &ar)
{
    ar.Read(this,sizeof(CHistory));
    return ar;
}


CHistoryManager::CHistoryManager(void)
{
    memset(m_sUserName,0,nWordLen+1);
    memset(m_sSoftwareNumber,0,nWordLen+1);
}

CHistoryManager::~CHistoryManager(void)
{
}

void CHistoryManager::SetUserName(CString sUserName)
{
    if(sUserName.GetLength()>nWordLen)
    {
        sUserName=sUserName.Right(nWordLen);
    }

    memset(m_sUserName,0,nWordLen+1);
    memcpy(m_sUserName,(LPCTSTR)sUserName,sUserName.GetLength());
}

void CHistoryManager::SetSoftwareNumber   (CString sSoftwareNumber)
{
    if(sSoftwareNumber.GetLength()>nWordLen)
    {
        sSoftwareNumber=sSoftwareNumber.Right(nWordLen);
    }

    memset(m_sSoftwareNumber,0,nWordLen+1);
    memcpy(m_sSoftwareNumber,(LPCTSTR)sSoftwareNumber,sSoftwareNumber.GetLength());
}

bool CHistoryManager::Read(CString sFile)
{
	if(sFile=="")return false;

    CFile file;
    if(!file.Open(sFile,CFile::modeRead))
    {
        return false;
    };
    CArchive ar(&file,CArchive::load);

    try
    {
        long nMark=0;
        ar>>nMark;
        if(nMark!=9999)
        {
            return false;
        }


        CHistory mark;
        mark.Read(ar);
	    mark.Decrypt ();
	    if(strcmp((const char*)(mark.m_sTargetName),"FHHISTORY")!=0)
	    {
            ar.Close();
            file.Close();
		    return false;
	    }

        mark.Read(ar);
        mark.Decrypt ();
        strcpy((char*)m_sUserName,(char*)mark.m_sTargetName);


        mark.Read(ar);
        mark.Decrypt ();
        strcpy((char*)m_sSoftwareNumber,(char*)mark.m_sTargetName);

	    ///////////////////////////////
	    //
	    long i;
        int nHistoryQty;
        ar>>nHistoryQty;

        CHistory history;

        for(i=0;i<nHistoryQty;i++)
        {
            history.Read(ar);
		    history.Decrypt ();
            m_vecHistroy.push_back(history);
	    }
    }

    catch (CException* e)
    {
        file.Close();
        ar.Close();
        return false;
    }

	//////////////////////////////////
	//
	ar.Close();
    file.Close();

	return true;
}

bool  CHistoryManager::Save(CString sFile)
{
    CFile file;
    if(!file.Open(sFile,CFile::modeCreate | CFile::modeWrite))
    {
        return false;
    }

    CArchive ar(&file,CArchive::store);

    try
    {
        long nMark=9999;
        ar<<nMark;

	    CHistory mark;
	    strcpy((char*)mark.m_sTargetName,"FHHISTORY");
	    mark.Encrypt ();
        mark.Write(ar);
        
        strcpy((char*)mark.m_sTargetName,(char*)m_sUserName);
        mark.Encrypt ();
        mark.Write(ar);

        strcpy((char*)mark.m_sTargetName,(char*)m_sSoftwareNumber);
        mark.Encrypt ();
        mark.Write(ar);

        int nQty=m_vecHistroy.size();
        ar<<m_vecHistroy.size();

	    ///////////////////////////////
	    //
	    ///////////////////////////////
	    long i;
	    
	    for(i=0;i<nQty;i++)
	    {
		    CHistory &word = m_vecHistroy.at(i);
		    word.Encrypt();
            word.Write(ar);
	    };
    }

    catch (CException* e)
    {
        file.Close();
        ar.Close();
        return false;
    }

	//////////////////////////////////
	//
	ar.Close();
    file.Close();

	return true;
}

bool CHistoryManager::RemoveAll()
{
    m_vecHistroy.clear();
	return true;
}

long  CHistoryManager::GetHistoryQty()
{
	return m_vecHistroy.size();
}

CHistory & CHistoryManager::GetHistory(long nIndex)
{
    return m_vecHistroy.at(nIndex);
}


CString CHistoryManager::GetUserName()
{
    return CString(m_sUserName);
}

CString CHistoryManager::GetSoftwareNumber()
{
    return CString(m_sSoftwareNumber);
}

bool CHistoryManager::Add(CHistory history)
{
    m_vecHistroy.push_back(history);
    return true;
}
