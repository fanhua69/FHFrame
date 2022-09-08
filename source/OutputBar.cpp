// OutputBar.cpp : implementation file
//

#include "stdafx.h"
#include "FHFrame.h"
#include "OutputBar.h"
#include "FHApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar
#include "fhcglobal.h"

COutputBar::COutputBar()
{
	m_nRowNumber=0;
	m_nMaxRow=1000;
	m_bEnable=true;

	m_iDockSide=CBRS_BOTTOM;

	m_nColNumber=1;
	m_arrayColName.Add("过程信息");

	m_iConditionCol=-1;
	m_sConditionString="";

	m_sTmpFile=GetTempFile();
}

void COutputBar::SetDockSide(int iDockSide)
{
	m_iDockSide=iDockSide;
}

void COutputBar::SetColProperty(CStringArray &array)
{
	m_nColNumber=array.GetCount ();
	m_arrayColName.Copy (array);
}

COutputBar::~COutputBar()
{
	RemoveFile(m_sTmpFile);
}


BEGIN_MESSAGE_MAP(COutputBar, CCJControlBar)
	//{{AFX_MSG_MAP(COutputBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutputBar message handlers

int COutputBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	// TODO: Add your specialized creation code here
	if (CCJControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if (!m_ListCtrl.Create (WS_TABSTOP|WS_CHILD|WS_VISIBLE|LVS_REPORT,
		CRect(0,0,0,0), this, IDC_OutputList))
	{
		TRACE0("Failed to create view for CMyBarLeft\n");
		return -1;
	}
	SetChild(&m_ListCtrl);

	m_ListCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	int *pnSize=new int [ m_nColNumber+2];
	for(int i=0;i<m_nColNumber;i++)
	{
		pnSize[i]=1000/m_nColNumber;
	}

	BuildColumns(m_nColNumber, pnSize);

	delete []pnSize;

	CStringArray array;
	array.Add (" ");
	for(int i=0;i<10;i++)
	{
		AddRow (array);
	}

	return 0;
}

void COutputBar::BuildColumns(int nCols, int * nWidth)
{
	ASSERT(nCols);
	ASSERT(nWidth);
	ASSERT(m_ListCtrl);

	//insert columns
	int i;
	LV_COLUMN lvc;

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CString	strTemp;
	for(i = 0; i < nCols; i++)
	{
		lvc.iSubItem = i;
		if(i>=m_arrayColName.GetCount ())
		{
			ASSERT(false);
			break;
		}
		strTemp=m_arrayColName.GetAt(i);
		lvc.pszText = (char*)(LPCTSTR)strTemp;
		lvc.cx = nWidth[i];
		lvc.fmt = LVCFMT_LEFT;
		m_ListCtrl.InsertColumn(i,&lvc);
	}
}

void COutputBar::AddItem(int nCol, CString sText)
{
	if(!m_bEnable)return;

	if(nCol==0)
	{	
		m_ListCtrl.InsertItem(m_nRowNumber, sText);
	}
	else if (nCol>0)
	{
		m_ListCtrl.InsertItem(m_nRowNumber, m_arrayColName.GetAt(nCol));		
		m_ListCtrl.SetItem(m_nRowNumber,1,LVIF_TEXT,sText,0,LVIF_TEXT,LVIF_TEXT,0);
	}

	m_nRowNumber++;

	if(m_nRowNumber>m_nMaxRow)
	{
		m_ListCtrl.DeleteItem (0);
		m_nRowNumber--;
	}

	m_ListCtrl.Scroll (CSize(0,100));
}

void COutputBar::AddRow(CStringArray &arrayRow)
{
	if(!m_bEnable)return;

	m_ListCtrl.InsertItem(m_nRowNumber, arrayRow.GetAt(0));		
	for(int i=1;i<arrayRow.GetCount ();i++)
	{
		if(i>=m_nColNumber)break;
		m_ListCtrl.SetItem(m_nRowNumber,i,LVIF_TEXT,arrayRow.GetAt (i),0,LVIF_TEXT,LVIF_TEXT,0);
	}

	m_nRowNumber++;

	if(m_nRowNumber>m_nMaxRow)
	{
		m_ListCtrl.DeleteItem (0);
		m_nRowNumber--;
	}

	m_ListCtrl.Scroll (CSize(0,100));
}

void COutputBar::ModifyItem(int iColCheck, CString sItemCheck, int iColModify, CString sItemNew)
{
	int iRow=SearchRow(iColCheck,sItemCheck);
	if(iRow!=-1)
	{
		ModifyItem(iRow,iColModify,sItemNew);
	}
}

void COutputBar::ModifyItem(int iRow, int iCol, CString sItem)
{
	if(!m_bEnable)return;

	if(iRow<0||iRow>=m_nRowNumber)
	{
		return ;
	}
	m_ListCtrl.SetItem(iRow,iCol,LVIF_TEXT,sItem,0,LVIF_TEXT,LVIF_TEXT,0);
}

void COutputBar::DeleteRow(int iCol, CString sItemValue)
{
	int i= SearchRow(iCol,sItemValue);
	if(i==-1)
	{
		return;
	}
	DeleteRow(i);
}


void COutputBar::DeleteRow(int iRow)
{
	if(!m_bEnable)return;

	if(iRow<0||iRow>=m_nRowNumber)
	{
		return ;
	}

	m_ListCtrl.DeleteItem (iRow);
}

int COutputBar::SearchRow(int iCol,CString sItem)
{
	if(!m_bEnable)return -1;

	char sSearchItem[1000];
	strcpy(sSearchItem,(LPCTSTR)sItem);
	char sText[1000];
	LVITEM vi;
	vi.mask			= LVIF_TEXT;
	vi.pszText		= sText;
	vi.cchTextMax	= 1000;
	vi.iSubItem		= iCol;

	for(int i=0;i<m_nRowNumber;i++)
	{
		vi.iItem = i;
		
		m_ListCtrl.GetItem (&vi);

		if(strcmp(sText,sSearchItem)==0)
		{
			return i;
		}
	}

	return -1;
}


void COutputBar::Clear()
{
	m_ListCtrl.DeleteAllItems ();
	m_nRowNumber=0;

}

void COutputBar::Enable(bool bEnable)
{
	m_bEnable=bEnable;
}
