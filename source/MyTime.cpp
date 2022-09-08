#include "StdAfx.h"
#include "mytime.h"
#include "StringExtractor.h"
#include "fhcglobal.h"
#include "FHMainFrm.h"

CMyTime::CMyTime(void)
{
	m_iHour=0;
	m_iMinute=0;
	m_iSecond=0;
}

CMyTime::~CMyTime(void)
{
}

CMyTime::CMyTime(int iHour,int iMinute,int iSecond)
{
	Set(iHour,iMinute,iSecond);
}

bool CMyTime::Set(int iHour,int iMinute,int iSecond)
{
	if(iHour<0||iHour>24)
	{
		TRACEERROR("小时数不对:"+vtos(iHour));
		return false;
	}

	if(iMinute<0||iMinute>60)
	{
		TRACEERROR("分钟数不对:"+vtos(iMinute));
		return false;
	}

	if(iSecond<0||iSecond>60)
	{
		TRACEERROR("秒数不对:"+vtos(iSecond));
		return false;
	}

	m_iHour=iHour;
	m_iMinute=iMinute;
	m_iSecond=iSecond;

	return true;
}

CMyTime CMyTime::operator - (CMyTime &other)
{
	int iSeconds1=m_iHour*3600+m_iMinute*60+m_iSecond;
	int iSeconds2=other.GetHour ()*3600+other.GetMinute ()*60+other.GetSecond ();
	int iMinusSecond=iSeconds1-iSeconds2;

	int iHour=iMinusSecond/3600;

	iMinusSecond-=iHour*3600;
	int iMinute=iMinusSecond/60;

	iMinusSecond-=iMinute*60;
	int iSecond=iMinusSecond;


	return CMyTime(iHour,iMinute,iSecond);
}

CString CMyTime::GetTimeString ()
{
	CString s="",s2="";
    if(m_iHour!=0)
    {
		s.Format ("%d时",m_iHour);
	}
	
	if(m_iMinute!=0)
	{
		s2.Format ("%d分",m_iMinute);
		s+=s2;
	}

	if(m_iSecond!=0)
	{
		s2.Format ("%d秒",m_iSecond);
		s+=s2;
	}

	return s;
}

bool CMyTime::SetTimeString (CString sTime)
{
	CStringExtractor ex;
	ex.SetSepChar (":-., _");
	ex.SetString (sTime);
	int n=ex.GetSubStringQty ();

	int iHour,iMinute,iSecond;

	if(n==3)
	{
		iHour=stovf(ex.GetSubString (0));
		iMinute=stovf(ex.GetSubString (1));
		iSecond=stovf(ex.GetSubString (2));
	}

	else if(n==2)
	{
		iHour=stovf(ex.GetSubString (0));
		iMinute=stovf(ex.GetSubString (1));
		iSecond=0;
	}

	else if(n==1)
	{
		iHour=stovf(ex.GetSubString (0));
		iMinute=0;
		iSecond=0;
	}

	return Set(iHour,iMinute,iSecond);
}
