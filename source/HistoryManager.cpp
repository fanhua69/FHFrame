
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
        return CString ("�򿪳���");
    case    eStartModule:
        return CString ("��ģ��");
    case    eOpenFile:
        return CString ("���ļ�");
    case    eSaveFile:
        return CString ("�洢�ļ�");
    case    eExportReport:
        return CString ("�������");
    case    eEndSoftware:
        return CString ("��������");
    case    eDrawAreaMap:
        return CString ("���ƹ���ͼ");
    case    eDrawLineData:
        return CString ("��ʾ��������");
    case    eDrawLineMap:
        return CString ("���Ʋ���ͼ��");
    case    eAddArea:
        return CString ("���ӹ���");
    case    eAddLine:
        return CString ("���Ӳ���");
    case    eRemoveArea:
        return CString ("ɾ������");
    case    eRemoveLine:
        return CString ("ɾ������");
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
