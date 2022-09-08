#include "StdAfx.h"
#include "fhimage.h"

CFHImage::CFHImage(void)
{
	m_pDC=NULL;
	m_hDC=0;
}

CFHImage::~CFHImage(void)
{
}

bool CFHImage::Save(CString sFile,
					int iTarWidth,
					int iTarHeight,
					bool  bKeepRatio)
{
	CRect rectTar(0,0,iTarWidth,iTarHeight);
	int iResultWidth=0;
	int iResultHeight=0;
	CalcExt(rectTar,
			bKeepRatio,
			0,
			iResultWidth,
			iResultHeight);

	if(iResultWidth<1||iResultHeight<1||iResultWidth>10000||iResultHeight>10000)
	{
		return false;
	}
	CImage image;
	int nBPP=GetBPP();
	image.Create (iResultWidth,iResultHeight,nBPP);
	HDC hDC=image.GetDC ();

	Show(hDC,CRect(0,0,iResultWidth,iResultHeight),bKeepRatio);

	image.ReleaseDC ();

	HRESULT hr=image.Save (sFile);
	if(hr!=S_OK)
	{
		return false;
	}

	return true;
}

bool CFHImage::Show(	HDC hDC,
						CRect rectTar,
						bool  bKeepRatio)
{
	m_hDC=hDC;
	m_pDC=NULL;

	return Show(rectTar,
				CRect(0,0,this->GetWidth (),GetHeight()),
				bKeepRatio);
}

bool CFHImage::Show(	CDC *pDC,
						CRect rectTar,
						bool  bKeepRatio)
{
	if(IsNull())return false;
	m_hDC=0;
	m_pDC=pDC;

	return Show(rectTar,
				CRect(0,0,this->GetWidth (),GetHeight()),
				bKeepRatio);
}

bool CFHImage::Show(	HDC hDC,
						CRect rectTar,
						CRect rectSrc,
						bool  bKeepRatio)
{
	m_hDC=hDC;
	m_pDC=NULL;

	return Show(rectTar,
				CRect(0,0,this->GetWidth (),GetHeight()),
				bKeepRatio);
}


bool CFHImage::Show(	CDC *pDC,
						CRect rectTar,
						CRect rectSrc,
						bool  bKeepRatio)
{
	m_hDC=0;
	m_pDC=pDC;

	return Show(rectTar,
				CRect(0,0,this->GetWidth (),GetHeight()),
				bKeepRatio);
}

void CFHImage::CalcExt(CRect	rectTar,
					   bool		bKeepRatio,
					   int		iDeflate,
					   int		&iResultWidth,
					   int		&iResultHeight)
{
	int iSrcWidth=GetWidth();
	int iSrcHeight=GetHeight();
	int iTarWidth=rectTar.Width ()-iDeflate*2;
	int iTarHeight=rectTar.Height()-iDeflate*2;

	if(iSrcWidth==0||iSrcHeight==0||iTarWidth<=0||iTarHeight<=0)return;

	////////////////////////////////
	//
	////////////////////////////////
	float fScaleTar=(float)iTarWidth/iTarHeight;
	float fScaleSrc=(float)iSrcWidth/iSrcHeight;

	if(bKeepRatio)
	{
		if(fScaleTar>fScaleSrc)
		{
			iTarHeight=iTarHeight;
			iTarWidth=iTarHeight*fScaleSrc;
		}
		else
		{
			iTarWidth=iTarWidth;
			iTarHeight=iTarWidth/fScaleSrc;
		}
	}

	while(iTarWidth%4!=0)
	{
		iTarWidth++;
	}

	iResultWidth=iTarWidth;
	iResultHeight=iTarHeight;

	return;
}

bool CFHImage::Show(	CRect rectTar,
						CRect rectSrc,
						bool  bKeepRatio)
{
	if(m_pDC==NULL&&m_hDC==0)return false;
	if(	rectSrc.Width ()	<=0
		||rectSrc.Height ()	<=0
		||rectTar.Width()	<=0
		||rectTar.Height ()	<=0)
	{
		return false;
	}

	if(m_pDC!=NULL)
	{
		m_hDC=m_pDC->m_hDC ;
	}

	int iDeflate=0;		
	if(m_pDC!=NULL)
	{
		iDeflate=4;
	}

	int iTarWidth=0;
	int iTarHeight=0;

	CalcExt(rectTar,
			bKeepRatio,
			iDeflate,
			iTarWidth,
			iTarHeight);
	if(iTarWidth<1||iTarHeight<1)
	{
		return false;
	}


	///////////////////////////////////
	// 不处理增大的情况：
	///////////////////////////////////
	if(rectTar.Width () >rectSrc.Width ()&&rectTar.Height()>rectSrc.Height())
	{
		StretchBlt(m_hDC ,CRect(CPoint(rectTar.left ,rectTar.top ),CSize(iTarWidth,iTarHeight)),rectSrc);
		return true;
	}

	if(rectSrc.right >GetWidth()||rectSrc.bottom >GetHeight())
	{
		rectSrc.right =GetWidth();
		rectSrc.bottom =GetHeight();
	}

	int nBPP=GetBPP ();
	CImage image;
	if(!image.Create (iTarWidth,iTarHeight,nBPP))
	{
		return false;
	};

	int iDistance=this->GetPitch ();
	int iDirection=1;
	if(iDistance<0)iDirection=-1;
	int nCharQtyPerPix=nBPP/8;
	int nCharQtyWithDir=nCharQtyPerPix*iDirection;


	char *pSrcPix=(char*)this->GetBits();
	char *pTarPix=(char*)image.GetBits();

	int iSrcWidth=rectSrc.Width ();
	int iSrcHeight=rectSrc.Height ();

	float fWStep=iSrcWidth/(float)iTarWidth;
	float fHStep=iSrcHeight/(float)iTarHeight;

	int i=0,j=0;
	float fX=0;
	float fY=0;
	long iTarPos=0;
	long iSrcPos=0;
	long iLinePos=0;

	int iLineChar=iSrcWidth*nCharQtyPerPix;
	while(iLineChar%4!=0)
	{
		iLineChar++;
	}
	iLineChar*=iDirection;

	int iTarLineWidth=iTarWidth*nCharQtyPerPix;
	while(iTarLineWidth%4!=0)
	{
		iTarLineWidth++;
	}
	iTarLineWidth*=iDirection;

	for(i=0;i<iTarHeight;i++)
	{
		iLinePos=(int)fY*iLineChar;
		fX=0;
		if(fY>=iSrcHeight)
		{
			break;
		}
		for(j=0;j<iTarWidth;j++)
		{
			if(fX>=iSrcWidth)
			{
				break;
			}

			iSrcPos=iLinePos+(int)fX*nCharQtyPerPix;
			iTarPos=i*iTarLineWidth+j*nCharQtyPerPix;

			memcpy(&pTarPix[iTarPos],&pSrcPix[iSrcPos],nCharQtyPerPix);
			fX+=fWStep;
		}
		fY+=fHStep;
	}

	CRect rectRealTar(CPoint(rectTar.left +iDeflate,rectTar.top +iDeflate),CSize(iTarWidth,iTarHeight));

	/*if(m_pDC!=NULL)
	{
		rectRealTar.InflateRect (iDeflate,iDeflate,iDeflate,iDeflate);
		m_pDC->Draw3dRect (rectRealTar,RGB(255,255,255),RGB(0,0,0));
		rectRealTar.DeflateRect (iDeflate,iDeflate,iDeflate,iDeflate);
		m_hDC=m_pDC->m_hDC ;
	}*/
	image.Draw(m_hDC ,rectRealTar);
	
	return true;
}
