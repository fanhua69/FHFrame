

#include "stdafx.h"
#include "stringextractor.h"

CStringExtractor::CStringExtractor(void)
{
	m_nLineCharLimit=5000;
	m_lSubStringQty=0;

	m_lSepStringLimit=20;

	memset(m_sLine,0,m_nLineCharLimit);
	memset(m_sSubString,0,m_nLineCharLimit);

	memset(m_lStartPos,0,m_nLineCharLimit);
	memset(m_lEndPos,0,m_nLineCharLimit);
	
	SetDefaultChar();
}

CStringExtractor::~CStringExtractor(void)
{
}

void CStringExtractor::SetSepChar(CString s)
{
	memset(m_sSepString,0,m_lSepStringLimit);

	for(int i=0;i<s.GetLength ();i++)
	{
		m_sSepString[i]=s.GetAt(i);
	}
}

void CStringExtractor::SetDefaultChar()
{
	memset(m_sSepString,0,m_lSepStringLimit);

	m_sSepString[0]=' ';
	m_sSepString[1]=',';
	m_sSepString[2]=9;
	m_sSepString[3]=';';
	m_sSepString[4]='\r';
	m_sSepString[5]=10;
	m_sSepString[6]=':';
	m_sSepString[7]='|';	
	m_sSepString[8]=0;

}


void CStringExtractor::Empty()
{
	memset(m_sSepString,0,m_lSepStringLimit);
	m_sSepString[0]='\r';
	m_sSepString[1]=10;
}


void CStringExtractor::AppendChar(char c)
{
	if(strlen(m_sSepString)>=m_lSepStringLimit-1)
	{
		return ;
	}

	char s[2];
	s[0]=c;
	s[1]=0;

	strcat(m_sSepString,s);
}

char * CStringExtractor::GetSepChar()
{
	return m_sSepString;
}

void CStringExtractor::SetString (CString sLine)
{
	SetString(sLine.GetBuffer());
	sLine.ReleaseBuffer();
}


/////////////////////////////////////////////////////////
// Check how many sub strings are there in the string,
// and get the starting and ending position of the
// sub strings:
/////////////////////////////////////////////////////////
void CStringExtractor::SetString(char sLine[])
{
	if(sLine==NULL)return;

	if(strlen(sLine)>m_nLineCharLimit)
	{
		return ;
	}
	strcpy(m_sLine,sLine);

	memset(m_lStartPos,	0,m_nLineCharLimit);
	memset(m_lEndPos,	0,m_nLineCharLimit);

	long nNumber=0;
	int nPos=0;
	int nLen=strlen(m_sLine);

	int *piPos=new int[nLen+2];
	int iSepQty=0;
	int i;
	for( i=0;i<nLen;i++)
	{
		if(strchr(m_sSepString,m_sLine[i]))
		{
			piPos[iSepQty]=i;
			iSepQty++;
		}
	}
	if(iSepQty>=m_nLineCharLimit)
	{
		iSepQty=m_nLineCharLimit;
	}

	m_lStartPos[0]=0;
	for(i=0;i<iSepQty;i++)
	{
		m_lEndPos[i]=piPos[i]-1;
		m_lStartPos[i+1]=piPos[i]+1;
	}
	m_lEndPos[iSepQty]=nLen;

	m_lSubStringQty=iSepQty+1;

	delete []piPos;
}

/////////////////////////////////////////////////////////
// Pick out the wanted sub string:
/////////////////////////////////////////////////////////
char * CStringExtractor::GetSubString(int iOrder)
{
	memset(m_sSubString,0,1000);
	if(iOrder>=0&&iOrder<m_lSubStringQty)
	{
		int nLen=m_lEndPos[iOrder]-m_lStartPos[iOrder]+1;
		if(nLen>0)
		{
			strncpy(m_sSubString,&m_sLine[m_lStartPos[iOrder]],nLen);
		}
	}

	return m_sSubString;
}

int CStringExtractor::GetSubStringQty()
{
	return m_lSubStringQty;
}
