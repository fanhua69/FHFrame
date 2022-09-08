// FHTreeViewBase.cpp : implementation file
//

#include "stdafx.h"
#include "fhframe.h"
#include "FHTreeViewBase.h"


// CFHTreeViewBase

IMPLEMENT_DYNCREATE(CFHTreeViewBase, CTreeView)

CFHTreeViewBase::CFHTreeViewBase()
{
}

CFHTreeViewBase::~CFHTreeViewBase()
{
}

BEGIN_MESSAGE_MAP(CFHTreeViewBase, CTreeView)
END_MESSAGE_MAP()


// CFHTreeViewBase diagnostics

#ifdef _DEBUG
void CFHTreeViewBase::AssertValid() const
{
	CTreeView::AssertValid();
}

void CFHTreeViewBase::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG


CString CFHTreeViewBase::GetDataBaseDiscription()
{
    return "DataBase";
}

CString CFHTreeViewBase::GetDataBaseName()
{
    return "DataBaseName";
}

CString CFHTreeViewBase::ConstructDataBaseName()
{
    return CString("");
}


CString CFHTreeViewBase::GetDataBaseNameExt()
{
    return "DataBaseExt";
}

CString CFHTreeViewBase::CreateDataBase(CString sDataBasePath)
{
    return CString("");
}

bool CFHTreeViewBase::OpenDataBase(CString sDataBaseName,bool bUpdateSolution)
{
    return false;
}

bool CFHTreeViewBase::InsertArea(CString sArea)
{
    return false;
}

bool CFHTreeViewBase::RemoveArea(CString sArea)
{
    return false;
}

void CFHTreeViewBase::ClearTree()
{
    return;
}