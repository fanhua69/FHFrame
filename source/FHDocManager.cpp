// FHDocManager.cpp : implementation file
//

#include "stdafx.h"
#include "FHDocManager.h"
#include "FHMainFrm.h"


// CFHDocManager

CFHDocManager::CFHDocManager()
{
}

CFHDocManager::~CFHDocManager()
{
}


// CFHDocManager member functions
void CFHDocManager::RemoveAllTemplates()
{		
	POSITION pos = m_templateList.GetHeadPosition();
	while (pos != NULL)
	{
		POSITION posTemplate = pos;
		CDocTemplate* pTemplate = (CDocTemplate* )m_templateList.GetNext(pos);
		if (pTemplate->m_bAutoDelete)
		{
			m_templateList.RemoveAt(posTemplate);
			delete (CDocTemplate*)pTemplate;
		}
	}
}

CDocTemplate * CFHDocManager::GetTemplate(CString sExt)
{
	POSITION pos=Exist(sExt);
	if(pos!=NULL)
	{
		return GetNextDocTemplate (pos);
	}
	else
	{
		TRACEERROR("����:�޷�����ĵ�ģ��:"+sExt);
		return NULL;
	}
}

bool CFHDocManager::RemoveTemplate(CString sExt)
{
	POSITION pos=Exist(sExt);
	if(pos!=0)
	{
		m_templateList.RemoveAt (pos);
		return true;
	}
	else
	{
		return false;
	}
}


POSITION CFHDocManager::Exist(CString sExt)
{
	sExt.MakeUpper ();
	POSITION posLast;
	POSITION pos=GetFirstDocTemplatePosition ();
	CString sTestExt;
	while (pos)
	{
		posLast=pos;
        CDocTemplate * pTemplate=GetNextDocTemplate (pos);
		pTemplate->GetDocString (sTestExt,CDocTemplate::regFileTypeId);
		sTestExt.MakeUpper ();
		if(sTestExt==sExt)
		{
			return posLast;
		}
	}

	return NULL;
}