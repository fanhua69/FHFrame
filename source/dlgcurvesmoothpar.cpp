// .\source\DlgDrawChoice.cpp : implementation file
//

#include "stdafx.h"
#include "fhframe.h"
#include "DlgCurveSmoothPar.h"
#include "..\resource.h"
#include "fhapp.h"

IMPLEMENT_DYNAMIC(CDlgCurveSmoothPar, CPropertyPage)

CDlgCurveSmoothPar::CDlgCurveSmoothPar(CWnd* pParent /*=NULL*/)
:CPropertyPage(IDD_DlgCurveSmoothPar)
{
    m_iPointQty =0;
    m_iPower    =0;
    m_iLoop     =0;
    m_iRedundantPointCheckQty=10;
    m_fRedundantPointMigrationRate=0.1;
    
    ::GetPar("FHMATH_CDlgCurveSmoothPar_iRedundantPointCheckQty",m_iRedundantPointCheckQty);
    ::GetPar("FHMATH_CDlgCurveSmoothPar_fRedundantPointMigrationRate",m_fRedundantPointMigrationRate);
    ::GetPar("FHMATH_CDlgCurveSmoothPar_iPointQty",m_iPointQty);
    ::GetPar("FHMATH_CDlgCurveSmoothPar_iPower",m_iPower);
    ::GetPar("FHMATH_CDlgCurveSmoothPar_iLoop",m_iLoop);

}

CDlgCurveSmoothPar::~CDlgCurveSmoothPar()
{
    ::AddPar("FHMATH_CDlgCurveSmoothPar_iRedundantPointCheckQty",m_iRedundantPointCheckQty);
    ::AddPar("FHMATH_CDlgCurveSmoothPar_fRedundantPointMigrationRate",m_fRedundantPointMigrationRate);
    ::AddPar("FHMATH_CDlgCurveSmoothPar_iPointQty",m_iPointQty);
    ::AddPar("FHMATH_CDlgCurveSmoothPar_iPower",m_iPower);
    ::AddPar("FHMATH_CDlgCurveSmoothPar_iLoop",m_iLoop);
}

void CDlgCurveSmoothPar::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_PointNumber, m_iPointQty);
    DDX_Text(pDX, IDC_Power, m_iPower);
    DDX_Text(pDX, IDC_SmoothLoop, m_iLoop);
    DDX_Text(pDX, IDC_CheckPointNumber, m_iRedundantPointCheckQty);
    DDX_Text(pDX, IDC_RedundantPointMigrationRate, m_fRedundantPointMigrationRate);
}


BEGIN_MESSAGE_MAP(CDlgCurveSmoothPar, CPropertyPage)
END_MESSAGE_MAP()


// CDlgCurveSmoothPar message handlers

