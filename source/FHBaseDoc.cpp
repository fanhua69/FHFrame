// .\include\FHBaseDoc.cpp : implementation file
//

#include "stdafx.h"
#include "..\include\fhbasedoc.h"


// CFHBaseDoc

IMPLEMENT_DYNCREATE(CFHBaseDoc, CDocument)

CFHBaseDoc::CFHBaseDoc()
{
	m_b2D=true;
}

BOOL CFHBaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFHBaseDoc::~CFHBaseDoc()
{
}


BEGIN_MESSAGE_MAP(CFHBaseDoc, CDocument)
END_MESSAGE_MAP()


// CFHBaseDoc diagnostics

#ifdef _DEBUG
void CFHBaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFHBaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFHBaseDoc serialization

void CFHBaseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CFHBaseDoc commands
void CFHBaseDoc::SetFileMark(char sMarkTag[])
{
	strcpy(m_sMark,sMarkTag);
}

void CFHBaseDoc::SetFileName(CString sFileName)
{
	m_sFile=sFileName;
}

bool CFHBaseDoc::SetFileDiscription(CString sDis)
{
	m_sFileDiscription=sDis;

	return true;
}

void CFHBaseDoc::SetFileExt(CString sExt)
{
	m_sExt=sExt;
}

char * CFHBaseDoc::GetFileMark()
{
	return m_sMark;
}

CString CFHBaseDoc::GetFileDiscription()
{
	return m_sFileDiscription;
}

CString CFHBaseDoc::GetFileExt()
{
	return m_sExt;
}

CString  CFHBaseDoc::GetFileName()
{
	return m_sFile;
}

enumDimension CFHBaseDoc::GetDimension(CString sFile)
{
	OnOpenDocument(sFile);
	return Seismic_2D;  
}



BOOL CFHBaseDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	return TRUE;
}

void CFHBaseDoc::Set2D(bool b2D)
{ 
    m_b2D=b2D;
}

bool CFHBaseDoc::Is2D()
{ 
    return m_b2D;
}
