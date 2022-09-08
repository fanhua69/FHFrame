#include "stdafx.h"
#include "parmanager.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>


CPar::CPar()
{
    m_psName=NULL;
    m_psValue=NULL;
}

CPar::CPar(char *psNameTag, char *psValueTag)
{
    m_psName=NULL;
    m_psValue=NULL;
    Set(psNameTag,psValueTag);
}

CPar::~CPar()
{
    Reset();
};

void CPar::Reset()
{
    if(m_psName)
    {
        delete []m_psName;
        m_psName=NULL;
    }
    if(m_psValue)
    {
        delete []m_psValue;
        m_psValue=NULL;
    }
}

char * CPar::GetName()
{
    return m_psName;
};

char * CPar::GetValue()
{
    return m_psValue;
};


void CPar::operator =(CPar &other)
{
    Set(other.GetName(),other.GetValue());
};

void CPar::SetString(CString sNameTag, CString sValueTag)
{
    Set(sNameTag.GetBuffer(),sValueTag.GetBuffer());
    sNameTag.ReleaseBuffer();
    sValueTag.ReleaseBuffer();
}

void CPar::SetName(char *psNameTag)
{
    char *psValue=GetValue();
    char *pNewValue=new char [strlen(psValue)+2];
    strcpy(pNewValue,psValue);

    Set(psNameTag,pNewValue);

    delete []pNewValue;
}

void CPar::SetValue(char *psValueTag)
{
    char *psName=GetName();

    char *pNewName=new char [strlen(psName)+2];

    strcpy(pNewName,psName);

    Set(pNewName,psValueTag);

    delete []pNewName;
}

// This is the main Set function:
void CPar::Set(char *psNameTag, char *psValueTag)
{
    ::Trim(psNameTag);
    ::Trim(psValueTag);

    int nLenName=strlen(psNameTag);
    int nLenValue=strlen(psValueTag);

    Reset();

    m_psName=new char [nLenName+2];
    strcpy(m_psName,psNameTag);

    m_psValue=new char [nLenValue+2];
    strcpy(m_psValue,psValueTag);
}


CParManager::CParManager(char *pFileName)
{
    if(!pFileName)
    {
        memset(m_sParFile,0,sizeof(m_sParFile));
        _getcwd(m_sParFile, sizeof(m_sParFile));
        strcat(m_sParFile,"par.txt");
    }    
    else
    {
        strcpy(m_sParFile,pFileName);
    }

    m_psValue       =new char [102];
    m_iValueMemLen  =100;

    m_psName        = new char [102];
    m_iNameMemLen   =100;
}

CParManager::~CParManager(void)
{
    SavePar();
    RemoveAllPars();

    if(m_psName)
    {
        delete []m_psName;
        m_psName=NULL;
        m_iNameMemLen   =0;
    }

    if(m_psValue)
    {
        delete []m_psValue;
        m_psValue=NULL;
        m_iValueMemLen  =0;
    }  

}

bool  CParManager::_AllocateNameMem(int iLen)
{
    if(m_iNameMemLen>=iLen)return true;

    if(iLen>1000)
    {
        int mm=0;
    }

    delete []m_psName;
    m_psName=new char[iLen+2];
    memset(m_psName,0,iLen+2);
    m_iNameMemLen=iLen;

    return m_psName!=NULL;
}

bool  CParManager::_AllocateValueMem(int iLen)
{
    if(m_iValueMemLen<iLen)
    {
        delete []m_psValue;
        m_psValue=new char[iLen+2];
        m_iValueMemLen=iLen;
    }

    if(m_psValue)
    {
        memset(m_psValue,0,m_iValueMemLen+2);
    }

    return m_psValue!=NULL;
}

void  CParManager::AddMorePar(char sName[], int iValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%d",iValue);
    AddMorePar(sName,m_psValue);
};

void  CParManager::AddMorePar(char sName[], long lValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%ld",lValue);
    AddMorePar(sName,m_psValue);
};

void  CParManager::AddMorePar(char sName[], double dValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lf",dValue);
    AddMorePar(sName,m_psValue);
};

void  CParManager::AddMorePar(char sName[], float fValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%f",fValue);
    AddMorePar(sName,m_psValue);
};

void  CParManager::AddMorePar(CString sName, int iValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%d",iValue);
    AddMorePar(sName.GetBuffer (),m_psValue);
};

void  CParManager::AddMorePar(CString sName, long lValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%ld",lValue);
    AddMorePar(sName.GetBuffer (),m_psValue);
};

void  CParManager::AddMorePar(CString sName, double dValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lf",dValue);
    AddMorePar(sName.GetBuffer (),m_psValue);
};

void  CParManager::AddMorePar(CString sName, float fValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%f",fValue);
    AddMorePar(sName.GetBuffer (),m_psValue);
};


void CParManager::AddMorePar(char sName[],char sValue[])
{
    CPar *pPar=new CPar(sName,sValue);
    m_list.push_back(pPar);
}

int CParManager::GetParQty(CString sName)
{
    char *psName=new char [sName.GetLength()+2];
    strcpy(psName,(LPCTSTR)sName);
    int iQty=GetParQty(psName);
    delete []psName;
    return iQty;
}


int CParManager::GetParQty(char sName[])
{
    std::list <CPar*>::iterator p;
    ::Trim(sName);

    m_vecParOfOneName.clear();
    CPar *pPar=NULL;
    for ( p = m_list.begin(); p!= m_list.end(); ++p )
    {
        pPar=*p;
        if(strcmp(sName,pPar->GetName())==0)
        {
            CPar *pParNew=new CPar(pPar->GetName(),pPar->GetValue());
            m_vecParOfOneName.push_back(pParNew);
        }
    }

    return m_vecParOfOneName.size();
}

char * CParManager::GetMorePar(int iOrder)
{
    if(iOrder<0||iOrder>=m_vecParOfOneName.size())
    {
        return NULL;
    }
    return m_vecParOfOneName.at(iOrder)->GetValue();
}

char * CParManager::GetPar(char sName[])
{
    std::list <CPar*>::iterator p;
    if(_SearchPar(sName,p))
    {
        if(strlen((*p)->GetValue())<1)return NULL;

        _AllocateValueMem(strlen((*p)->GetValue()));
        strcpy(m_psValue,(*p)->GetValue());
        return m_psValue;
    }
    return NULL;
}

bool CParManager::SetPar(char sName[], char sValue[])
{
    ::Trim(sName);
    ::Trim(sValue);

    std::list <CPar*>::iterator p;
    if(_SearchPar(sName,p))
    {
        (*p)->SetValue(sValue);
        return true;
    }
    return false;
}

bool CParManager::_SetPar(std::list <CPar*>::iterator p,char sValue[])
{
    (*p)->SetValue(sValue);
    return true;
}


bool CParManager::RemovePar(char sName[])
{
    ::Trim(sName);
    std::list <CPar*>::iterator p;
    while(true)
    {
        if(_SearchPar(sName,p))
        {
            CPar *pPar=*p;
            m_list.erase (p);
            delete pPar;
        }
        else
        {
            break;
        }
    }

    return false;
}

bool CParManager::_SearchPar(char sName[],std::list <CPar*>::iterator &p)
{
    ::Trim(sName);

    int iOrder=0;
    for ( p = m_list.begin(); p!= m_list.end(); ++p )
    {
        CPar *pPar=*p;
        if(strcmp(sName,pPar->GetName())==0)
        {
            return true;
        }
        iOrder++;// just for debugging.
    }

    return false;
}


bool CParManager::ReadPar()
{
    FILE *fp=fopen(m_sParFile,"rt");
    if(!fp)
    {
        return false;
    }

    m_list.clear ();

    int nNameLen=0;
    int nValueLen=0;
    char *pChar=NULL;
    char *pValueStart=NULL;
    char *pNameEnd=NULL;
    char *pNameStart=NULL;
    char *psName=NULL;
    char *psValue=NULL;

    while(!feof(fp))
    {
        // Read a Line out from the file:
        memset(m_sLine,0,2000);
        fgets(m_sLine,2000,fp);

        // The Name:
        pNameStart=strchr(m_sLine,'[');
        if(!pNameStart)continue;
        pNameEnd=strchr(m_sLine,']');
        if(!pNameEnd)continue;
        nNameLen=pNameEnd-pNameStart-1;

        if(psName)delete []psName;
        psName=new char[nNameLen+2];
        memset(psName,0,nNameLen+2);
        strncpy(psName, &pNameStart[1], nNameLen);

        // The Value:
        pValueStart=strchr(m_sLine,'=');
        if(!pValueStart)continue;

        if(psValue)delete []psValue;
        nValueLen=strlen(&pNameStart[1]);
        psValue=new char[nValueLen+2];
        memset(psValue,0,nValueLen+2);
        strcpy(psValue, &pValueStart[1]);

        //
        CPar  *pPar=new CPar(psName,psValue);
        m_list.push_back(pPar);
    }

    if(psName)delete []psName;
    if(psValue)delete []psValue;

    fclose(fp);

    return true;

}

bool CParManager::SavePar()
{
    FILE *fp=fopen(m_sParFile,"wt");
    if(!fp)
    {
        return false;
    }

    CPar *pPar=NULL;
    std::list <CPar*>::iterator p;
    for ( p = m_list.begin(); p!= m_list.end(); ++p )
    {
        pPar=*p;
        fprintf(fp,"[%s]=%s\n",pPar->GetName(), pPar->GetValue());
    }

    fclose(fp);

    return true;

}

bool CParManager::RemoveAllPars()
{
    CPar *pPar=NULL;
    std::list <CPar*>::iterator p;
    for ( p = m_list.begin(); p!= m_list.end(); ++p )
    {
        pPar=*p;
        delete pPar;
    }

    m_list.clear ();
    return true;
}


bool CParManager::SetParFile(char sFile[])
{
    if(strcmp(sFile,"")==0)return false;
    strcpy(m_sParFile,sFile);
    return true;
}


bool  CParManager::GetPar(char sName[], int &iValue)
{
    char *psValue=GetPar(sName);
    if(!psValue)return false;
    sscanf(psValue,"%d",&iValue);
    return true;
};


bool  CParManager::GetPar(char sName[], long &lValue)
{
    char *psValue=GetPar(sName);
    if(!psValue)return false;
    sscanf(psValue,"%ld",&lValue);
    return true;
};
bool  CParManager::GetPar(char sName[], double &dValue)
{
    char *psValue=GetPar(sName);
    if(!psValue)return false;
    sscanf(psValue,"%lf",&dValue);
    return true;
};

bool  CParManager::GetPar(char sName[], float &fValue)
{
    char *psValue=GetPar(sName);
    if(!psValue)return false;
    sscanf(psValue,"%f",&fValue);
    return true;
};

bool  CParManager::GetPar(CString sName, int &iValue)
{
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;
    sscanf(psValue,"%d",&iValue);
    return true;
};

bool  CParManager::GetPar(CString sName, UINT &iValue)
{
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;
    sscanf(psValue,"%lu",&iValue);
    return true;
};

bool  CParManager::GetPar(CString sName, long &lValue)
{
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;
    sscanf(psValue,"%ld",&lValue);
    return true;
};
bool  CParManager::GetPar(CString sName, double &dValue)
{
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;
    sscanf(psValue,"%lf",&dValue);
    return true;
};

bool  CParManager::GetPar(CString sName, float &fValue)
{
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;
    sscanf(psValue,"%f",&fValue);
    return true;
};

void  CParManager::AddPar(char sName[], int iValue)
{
    _AllocateValueMem(100);

    sprintf(m_psValue,"%d",iValue);
    AddPar(sName,m_psValue);
};
void  CParManager::AddPar(char sName[], UINT iValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lu",iValue);
    AddPar(sName,m_psValue);
};
void  CParManager::AddPar(char sName[], long lValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%ld",lValue);
    AddPar(sName,m_psValue);
};

void  CParManager::AddPar(char sName[], double dValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lf",dValue);
    AddPar(sName,m_psValue);
};

void  CParManager::AddPar(char sName[], float fValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%f",fValue);
    AddPar(sName,m_psValue);
};

void  CParManager::AddPar(CString sName, int iValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%d",iValue);
    AddPar((LPCTSTR)sName,m_psValue);
};

void  CParManager::AddPar(CString sName, UINT iValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lu",iValue);
    AddPar((LPCTSTR)sName,m_psValue);
};

void  CParManager::AddPar(CString sName, long lValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%ld",lValue);
    AddPar((LPCTSTR)sName,m_psValue);
};

void  CParManager::AddPar(CString sName, double dValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%lf",dValue);
    AddPar((LPCTSTR)sName,m_psValue);
};

void  CParManager::AddPar(CString sName, float fValue)
{
    _AllocateValueMem(100);
    sprintf(m_psValue,"%f",fValue);
    AddPar((LPCTSTR)sName,m_psValue);
};

void CParManager::AddPar(char sName[], char sValue[])
{
    std::list <CPar*>::iterator p;
    if(_SearchPar(sName,p))
    {
        (*p)->SetValue(sValue);
    }
    else
    {
        CPar *pPar=new CPar(sName,sValue);
        m_list.push_back(pPar);
    }
}


////////////////////////////////////////
// Special functions for CString:
bool  CParManager::RemovePar(CString sName)
{
    sName.Trim ();

    _AllocateNameMem(sName.GetLength());

    strcpy(m_psName,(LPCTSTR)sName);
    return RemovePar(m_psName);
}

bool CParManager::GetPar(CString sName, CString &sValue)
{
    sName.Trim ();
    _AllocateNameMem(sName.GetLength());
    strcpy(m_psName,(LPCTSTR)sName);
    char *psValue=GetPar(m_psName);
    if(!psValue)return false;

    sValue=m_psValue;
    return true;
}

void  CParManager::AddPar(CString sName, CString sValue)
{
    _AllocateNameMem(sName.GetLength());
    _AllocateValueMem(sValue.GetLength());

    strcpy(m_psName,(LPCTSTR)sName);
    strcpy(m_psValue,(LPCTSTR)sValue);
    AddPar(m_psName,m_psValue);
}

bool  CParManager::SetPar(CString sName, CString sValue)
{
    _AllocateNameMem(sName.GetLength());
    _AllocateValueMem(sValue.GetLength());

    strcpy(m_psName,(LPCTSTR)sName);
    strcpy(m_psValue,(LPCTSTR)sValue);

    return SetPar(m_psName,m_psValue);
}

bool CParManager:: SetParFile(CString sFile)
{
    if(sFile=="")return false;
    strcpy(m_sParFile,(LPCTSTR)sFile);
    return true;
}

