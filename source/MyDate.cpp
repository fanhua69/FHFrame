

#include "StdAfx.h"
#include "mydate.h"
#include "fhcglobal.h"

CMyDate::CMyDate(void)
{
	CTime time=CTime::GetCurrentTime ();
	m_iYear=time.GetYear();
	m_iMonth=time.GetMonth();
	m_iDay=time.GetDay();

	m_mt=MT_STATISTICS;
}

CMyDate::~CMyDate(void)
{
}

CMyDate::CMyDate(CString sDateString)
{
	m_iYear     =0;
	m_iMonth    =0;
	m_iDay      =0;

	Set(sDateString);
}

CMyDate::CMyDate(CString sYear,CString sMonth,CString sDay)
{
	Set( sYear, sMonth, sDay);
}


CMyDate::CMyDate(int iYear,int iMonth,int iDay)
{
	SetYear(iYear);
	SetMonth(iMonth);
	SetDay(iDay);
}

void CMyDate::Set(CString sYear,CString sMonth,CString sDay)
{
	SetYear(sYear);
	SetMonth(sMonth);
	SetDay(sDay);
}

void CMyDate::Set(int iYear,int iMonth,int iDay)
{
	SetYear(iYear);
	SetMonth(iMonth);
	SetDay(iDay);
}

void CMyDate::SetYear(int iYear)
{
    m_iYear=iYear;
}

void CMyDate::SetMonth(int iMonth)
{
    m_iMonth=iMonth;
}

void CMyDate::SetDay(int iDay)
{
    m_iDay=iDay;
}

void CMyDate::SetYear(CString sYear)
{
    SetYear(stovf(sYear));
}

void CMyDate::SetMonth(CString sMonth)
{
    SetMonth(stovf(sMonth));
}

void CMyDate::SetDay(CString sDay)
{
    SetDay(stovf(sDay));
}

void CMyDate::operator = (CMyDate &date)
{
	Copy(date);
}

void CMyDate::Copy(CMyDate &date)
{
	m_iYear=date.m_iYear ;
	m_iMonth=date.m_iMonth ;
	m_iDay=date.m_iDay;
	m_mt=date.m_mt ;
}

bool CMyDate::Set(CString sYearMonthDate)
{
    CString sDate=sYearMonthDate;
    CString sSeperate=".-_:,";
    CString sYear="",sMonth="",sDay="";
	int n=sDate.Find("年");
	if(n!=-1)
	{
		sYear=sDate.Left (n);
		n=n+2;
	}
	else 
	{
		n=sDate.FindOneOf (sSeperate);
		if(n!=-1)
		{
			sYear=sDate.Left (n);
			n=n+1;
		}
        else
        {
            sYear=sDate;
        }
	}
    SetYear(sYear);

	if(n==-1)
    {
        n=0;
    }

	if(n>=sDate.GetLength ())return false;
	sDate=sDate.Mid (n);

	n=sDate.Find("月");
	if(n!=-1)
	{
		sMonth=sDate.Left (n);
		n=n+2;
	}
	else 
	{
		n=sDate.FindOneOf (sSeperate);
		if(n!=-1)
		{
			sMonth=sDate.Left (n);
			n=n+1;
		}
        else
        {
            sMonth=sDate;
        }
	}
    SetMonth(sMonth);

    //
	if(n==-1)n=0;
	if(n>=sDate.GetLength ())
    {
        return false;
    }
	sDate=sDate.Mid (n);

	n=sDate.Find("日");
	if(n!=-1)
	{
		sDay=sDate.Left (n);
		n=n+2;
	}
	else 
	{
		n=sDate.FindOneOf (sSeperate);
		if(n!=-1)
		{
			sDay=sDate.Left (n);
			n=n+1;
		}
        else
        {
            sDay=sDate;
        }
	}
    SetDay(sDay);

	return true;
}

int CMyDate::GetYear()
{
	return m_iYear;
}

int CMyDate:: GetMonth()
{
	return m_iMonth;
}

int CMyDate::GetDay()
{
	return m_iDay;
}

CString CMyDate::GetDateString()
{
	CString s;
	s.Format ("%d年%d月%d日",m_iYear,m_iMonth,m_iDay);
	return s;
}

CString CMyDate::GetMonthDayString()
{
	CString s;
	s.Format ("%d月%d日",m_iMonth,m_iDay);
	return s;
}

int CMyDate::DayFrom(CMyDate &date)
{
	/*
	CTime time1(m_iYear,m_iMonth,m_iDay,0,0,0);
	CTime time2(date.GetYear (),
				date.GetMonth (),
				date.GetDay (),
				0,0,0);

	CTimeSpan span=time2-time1;
	return span.GetDays ();*/

	return 1234;

}

int CMyDate::MonthFrom(CMyDate &date)
{
	int iMonth=0;
	iMonth=(abs(m_iYear-date.m_iYear) +1)*12;

	if(*this>date)
	{
		iMonth-=date.m_iMonth;
		iMonth-=12-m_iMonth;
	}
	else
	{
		iMonth-=m_iMonth ;
		iMonth-=12-date.m_iMonth;
	}

	return iMonth;
}

bool CMyDate::operator >=(CMyDate &date)
{
	return (*this>date||*this==date);
}

bool CMyDate::operator <=(CMyDate &date)
{
	return *this<date || *this==date;
}

bool CMyDate::operator >(CMyDate &date)
{
	if(m_iYear<date.m_iYear)return false;
	if(m_iYear>date.m_iYear)return true;

	if(m_iMonth>date.m_iMonth)return true;
	if(m_iMonth<date.m_iMonth)return false;

	if(m_iDay>date.m_iDay)return true;
	return false;
}

bool CMyDate::operator <(CMyDate &date)
{
	if(m_iYear<date.m_iYear)return true;
	if(m_iYear>date.m_iYear)return false;

	if(m_iMonth<date.m_iMonth)return true;
	if(m_iMonth>date.m_iMonth)return false;

	if(m_iDay<date.m_iDay)return true;
	return false;
}

bool CMyDate::operator ==(CMyDate &date)
{
	return (m_iYear==date.m_iYear&&m_iMonth==date.m_iMonth&&m_iDay==date.m_iDay);
}


void CMyDate::DecreaseMonth()
{
	int iMonth=m_iMonth;
	iMonth--;
	m_iMonth=iMonth;

	if(iMonth<1)
	{
		int iYear=m_iYear;
		iYear--;
		m_iYear=iYear;
		m_iMonth=12;
	};
}

void CMyDate::IncreaseMonth()
{
	int iMonth=m_iMonth;
	iMonth++;
	m_iMonth=iMonth;

	if(iMonth>12)
	{
		int iYear=m_iYear;
		iYear++;
		m_iYear=iYear;
		m_iMonth=1;
	};
}

void CMyDate::SetMonthType(enumMonthType eMonthType)
{
	m_mt=eMonthType;
}

void CMyDate::GetMonthDate(CMyDate &dateStart, CMyDate &dateEnd)
{

	if(m_mt==MT_NATURAL)
	{
		dateStart.SetYear (m_iYear);
		dateStart.SetMonth (m_iMonth);
		dateStart.SetDay (1);
		dateStart.m_mt = m_mt;

		dateEnd.SetYear (m_iYear);
		dateEnd.SetMonth (m_iMonth);
		dateEnd.SetDay (GetMonthDayQty());
	}
	else
	{
		dateStart.SetYear (m_iYear);
		dateStart.SetMonth (m_iMonth);
		dateStart.SetDay (26);
		dateStart.DecreaseMonth ();

		dateEnd.SetYear (m_iYear);
		dateEnd.SetMonth (m_iMonth);
		dateEnd.SetDay (25);
	}
	dateStart.m_mt = m_mt;
	dateEnd.m_mt = m_mt;
}

bool CMyDate::IsLeapYear()
{
	int iYear=m_iYear;
	if(iYear%4==0 && iYear%100!=0)
	{
		return true;
	}

	if(iYear%400==0)
	{
		return true;
	}

	return false;
}

int CMyDate::GetMonthDayQty()
{
	int iMonth=m_iMonth;
	switch (iMonth)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		if(IsLeapYear())
		{
			return 29;
		}
		else
		{
			return 28;
		};
	default:
		ASSERT(false);
		return 0;
	}
}

void CMyDate::SetType(enumMonthType mt)
{
	m_mt=mt;
}

CArchive & CMyDate::Write(CArchive &ar)
{
    ar<<m_iYear;
    ar<<m_iMonth;
    ar<<m_iDay;
    ar<<(int)m_mt;	

    return ar;
}

CArchive & CMyDate::Read(CArchive &ar)
{
    ar>>m_iYear;
    ar>>m_iMonth;
    ar>>m_iDay;
    
    int n;
    ar>>n;
    m_mt=(enumMonthType)n;	

    return ar;
}
