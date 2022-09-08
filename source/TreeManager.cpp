#include "stdafx.h"
#include "treemanager.h"

CTreeManager::CTreeManager(void)
{
	m_pList=new CObList;
}

CTreeManager::~CTreeManager(void)
{
	m_pList->RemoveAll ();
	delete m_pList;
}

bool CTreeManager::AddTree(CTreeView *pTree, CString sType)
{
	RemoveTree(sType);

	CTreeProperty *pNewTree=new CTreeProperty ;
	pNewTree->pTree =pTree;
	strcpy(pNewTree->sType,sType);

	m_pList->AddTail(pNewTree);

	return true;

}

bool CTreeManager::RemoveTree(CString sTree)
{
	while(true)
	{
		POSITION pos=ExistTree(sTree);
		if(pos!=0)
		{
			m_pList->RemoveAt (pos);
		}
		else
		{
			break;
		}
	}

	return true;
}

CTreeView * CTreeManager::GetTree(CString sType)
{
	POSITION pos=ExistTree(sType);
	if(pos!=0)
	{
		CTreeProperty *pProperty=(CTreeProperty *)m_pList->GetAt (pos);
		return (CTreeView*)pProperty->pTree ;
	}

	return NULL;
}

CObList * CTreeManager::GetTreeList()
{
	return m_pList;
}

POSITION CTreeManager::ExistTree(CString sType)
{
	sType.MakeUpper ();
	POSITION pos=m_pList->GetHeadPosition ();
	CTreeProperty *pTreeInList=NULL;
	CString sThis;
	POSITION posLast;
	while(pos)
	{
		posLast=pos;
		pTreeInList=(CTreeProperty *)m_pList->GetNext (pos);
		sThis=pTreeInList->sType ;
		sThis.MakeUpper();
		if(sThis == sType)
		{
			return posLast;
		}
	}
	return 0;
}

void  CTreeManager::EmptyTree()
{
	m_pList->RemoveAll ();
}
