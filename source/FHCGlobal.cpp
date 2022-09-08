

#include "stdafx.h"
#include "math.h"
#include "FHCGlobal.h"
#include "FHString.h"

#ifndef __AFX_H__

CFHString GetSubString(CFHString sString,int nOrder)
{		
	long nLen=sString.GetLength ();
	long *pStartPos=new long[nLen+10];
	memset(pStartPos,0,sizeof(long)*(nLen+10));

	pStartPos[0]=0;

	// Check how many sub strings are there in the string:
	long nNumber=0;
	int nPos=0;
	int nMinusChar=2;
	char *pChar=sString.GetBuffer();
	while(nPos<nLen+1){
		if(pChar[nPos]=='\r'&&pChar[nPos+1]=='\n'){
			nNumber++;
			pStartPos[nNumber]=nPos+2;
			nMinusChar=2;
		}
		else if(pChar[nPos]=='\0'){
			nNumber++;
			pStartPos[nNumber]=nPos+1;
			nMinusChar=1;
		}

		nPos++;
	}

	//
	CFHString sReturn;
	if(nOrder>=0&&nOrder<nNumber){
		int nLen=pStartPos[nOrder+1]-nMinusChar-pStartPos[nOrder];
		if(nLen>0)sReturn=sString.Mid(pStartPos[nOrder],nLen);
	}

	delete []pStartPos;

	return sReturn;

}

CFHString SeperateName(CFHString sLineName)
{
	int posStart=sLineName.ReverseFind('\\');
	int posEnd=sLineName.ReverseFind('.');
	if(posEnd==-1)posEnd=sLineName.GetLength();

	CFHString s=sLineName.Mid(posStart+1,(posEnd-posStart-1));
	s.MakeUpper ();
	return s;
}

CFHString SeperateExt(CFHString sLineName)
{
	int posEnd=sLineName.ReverseFind('.');
	if(posEnd==-1)return CFHString(" ");

	CFHString s=sLineName.Mid(posEnd+1);
	s.MakeUpper ();
	return s;
}

CFHString SeperateFileName(CFHString sLineName)
{
	int posBegin=sLineName.ReverseFind('\\');

	CFHString s=sLineName.Right (sLineName.GetLength ()-posBegin-1);
	s.MakeUpper ();
	return s;
}	
	
// delete the extension:
CFHString SeperatePathName(CFHString sLineName)
{
	int pos=sLineName.ReverseFind('.');
	if(pos==-1)
		return sLineName;
	else
		return sLineName.Left (pos);
}

// Delete the extension and the name 
CFHString SeperatePath(CFHString sLineName)
{
	int pos=sLineName.ReverseFind('\\');
	if(pos==-1)
		return CFHString("");
	else
		return sLineName.Left (pos);
}

CFHString vtos(long data)
{
	char s[200];
	sprintf(s,"%ld",data);
	return CFHString (s);
}

CFHString vtos(int data)
{
	char s[200];
	sprintf(s,"%d",data);
	return CFHString (s);
}

CFHString vtos(float data)
{
	char s[200];
	sprintf(s,"%f",data);
	return CFHString (s);
}

CFHString vtos(double data)
{
	char s[200];
	sprintf(s,"%1.5",data);
	return CFHString (s);
}

//	Find Line from a file
bool FindLineInFile(FILE *fp, CFHString &sLineReturn,CFHString sLeftHeadToFind,bool bRemoveBlank)
{	
	bool bFound=false;
	CFHString sLine,sLineLeft;

	if(bRemoveBlank)sLeftHeadToFind.Remove (' ');

	long nLen=sLeftHeadToFind.GetLength ();

	while(true){		 
		if(feof(fp))break;
		fgets(sLine.GetBuffer (500),500,fp);
		sLine.ReleaseBuffer ();
		if(bRemoveBlank)sLine.Remove (' ');
		sLineLeft=sLine.Left (nLen);

		if(!sLineLeft.Compare (sLeftHeadToFind)){
			bFound=true;
			sLineReturn=sLine;
			break;
		}
	}
	
	if(!bFound){
		return false;
	}
	else
		return true;
}

long GetFileLineNumber(CFHString sFile,int nDataInLine)
{
	if(sFile=="")return 0;
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		return 0;
	}

	int n=0,i,nData;
	CFHString s;
	double dData[20];
	bool bInvalid=false;
	char sLine[300];
	while(!feof(fp)){
		s="";
		fgets(s.GetBuffer(300),300,fp);
		s.ReleaseBuffer();
		s.TrimLeft ();		
		if(s=="")break;

		if(nDataInLine!=0){
			for(i=0;i<20;i++){
				dData[i]=-1;
			}

			strcpy(sLine,s.GetBuffer ());
			sscanf(sLine,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
				&dData[0],&dData[1],&dData[2],&dData[3],&dData[4],
				&dData[5],&dData[6],&dData[7],&dData[8],&dData[9],
				&dData[10],&dData[11],&dData[12],&dData[13],&dData[14],
				&dData[15],&dData[16],&dData[17],&dData[18],&dData[19]);
			for(i=0;i<20;i++){
				if(dData[i]==-1){
					nData=i;
					break;
				}
			}
			if(nData!=nDataInLine){
				bInvalid=true;
				break;
			}
		}

		n++;
	}
	fclose(fp);

	if(bInvalid)
		return -1;
	else
		return n;
}

void copyname(char sNameTar[],CFHString sNameSrc)
{
	long nChar=sNameSrc.GetLength ();
	memcpy(sNameTar,sNameSrc.GetBuffer (),nChar);
	memset(&sNameTar[nChar],0,1);
};


int TRACEERROR(CFHString sError,int nType)
{
	return TRACEERROR(sError.GetBuffer(),nType);
}

int TRACEERROR(char *sError,int nType)
{
	printf(sError);
	if(nType==MB_YESNO){
		char c;
		scanf("%c",&c);
		if(c=='y'||c=='Y'){
			return IDYES;
		}
		else{
			return IDNO;
		}
	}
	else
		return IDNO;
}

#endif


/***** Three Order Spline Function Interpolation *****/

void splint(double *xa, double *ya, double *y2a, int *nn, double *xx, double *yy)
{
	int klo, khi, k, khl;
	double x, y, h, a, b;
	int n;
	
	n = *nn;
	x = *xx;

	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(xa[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	
	/*printf("khi=%d, klo=%d\n", khi, klo);*/
	h = xa[khi]-xa[klo];
	if(h==0.0) printf("Bad Input XA\n");
	a = (xa[khi]-x)/h;
	b = (x-xa[klo])/h;
	y = a*ya[klo]+b*ya[khi]+
	    ((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
	*yy = y;
	return;
}

/*** Forming The Interpolating Spline Function *****/


void spline (int n, double x[], double y[], double yd[][4])
/*****************************************************************************
compute cubic spline interpolation coefficients for interpolation with
continuous second derivatives
******************************************************************************
Input:
n		number of samples
x  		array[n] of monotonically increasing or decreasing abscissae
y		array[n] of ordinates

Output:
yd		array[n][4] of cubic interpolation coefficients (see notes)
******************************************************************************
Notes:
The computed cubic spline coefficients are as follows:
yd[i][0] = y(x[i])    (the value of y at x = x[i])
yd[i][1] = y'(x[i])   (the 1st derivative of y at x = x[i])
yd[i][2] = y''(x[i])  (the 2nd derivative of y at x = x[i])
yd[i][3] = y'''(x[i]) (the 3rd derivative of y at x = x[i])

To evaluate y(x) for x between x[i] and x[i+1] and h = x-x[i],
use the computed coefficients as follows:
y(x) = yd[i][0]+h*(yd[i][1]+h*(yd[i][2]/2.0+h*yd[i][3]/6.0))
*****************************************************************************/
{
	int i;
	double h1,h2,del1,del2,dmax,hsum,w1,w2,divdf3,sleft,sright,alpha,t;

	/* if n=1, then use constant interpolation */
	if (n==1) {
		yd[0][0] = y[0];
		yd[0][1] = 0.0;
		yd[0][2] = 0.0;
		yd[0][3] = 0.0;
		return;

	/* else, if n=2, then use linear interpolation */
	} else if (n==2) {
		yd[0][0] = y[0];  yd[1][0] = y[1];
		yd[0][1] = yd[1][1] = (y[1]-y[0])/(x[1]-x[0]);
		yd[0][2] = yd[1][2] = 0.0;
		yd[0][3] = yd[1][3] = 0.0;
		return;
	}
	
	/* set left end derivative via shape-preserving 3-point formula */
	h1 = x[1]-x[0];
	h2 = x[2]-x[1];
	hsum = h1+h2;
	del1 = (y[1]-y[0])/h1;
	del2 = (y[2]-y[1])/h2;
	w1 = (h1+hsum)/hsum;
	w2 = -h1/hsum;
	sleft = w1*del1+w2*del2;
	if (sleft*del1<=0.0)
		sleft = 0.0;
	else if (del1*del2<0.0) {
		dmax = 3.0*del1;
		if (fabs(sleft)>fabs(dmax)) sleft = dmax;
	}

	/* set right end derivative via shape-preserving 3-point formula */
	h1 = x[n-2]-x[n-3];
	h2 = x[n-1]-x[n-2];
	hsum = h1+h2;
	del1 = (y[n-2]-y[n-3])/h1;
	del2 = (y[n-1]-y[n-2])/h2;
	w1 = -h2/hsum;
	w2 = (h2+hsum)/hsum;
	sright = w1*del1+w2*del2;
	if (sright*del2<=0.0)
		sright = 0.0;
	else if (del1*del2<0.0) {
		dmax = 3.0*del2;
		if (fabs(sright)>fabs(dmax)) sright = dmax;
	}
	
	/* compute tridiagonal system coefficients and right-hand-side */	
	yd[0][0] = 1.0;
	yd[0][2] = 2.0*sleft;
	for (i=1; i<n-1; i++) {
		h1 = x[i]-x[i-1];
		h2 = x[i+1]-x[i];
		del1 = (y[i]-y[i-1])/h1;
		del2 = (y[i+1]-y[i])/h2;
		alpha = h2/(h1+h2);
		yd[i][0] = alpha;
		yd[i][2] = 3.0*(alpha*del1+(1.0-alpha)*del2);
	}
	yd[n-1][0] = 0.0;
	yd[n-1][2] = 2.0*sright;
	
	/* solve tridiagonal system for slopes */
	t = 2.0;
	yd[0][1] = yd[0][2]/t;
	for (i=1; i<n; i++) {
		yd[i][3] = (1.0-yd[i-1][0])/t;
		t = 2.0-yd[i][0]*yd[i][3];
		yd[i][1] = (yd[i][2]-yd[i][0]*yd[i-1][1])/t;
	}
	for (i=n-2; i>=0; i--)
		yd[i][1] -= yd[i+1][3]*yd[i+1][1];

	/* copy ordinates into output array */
	for (i=0; i<n; i++)
		yd[i][0] = y[i];

	/* compute 2nd and 3rd derivatives of cubic polynomials */
	for (i=0; i<n-1; i++) {
		h2 = x[i+1]-x[i];
		del2 = (y[i+1]-y[i])/h2;
		divdf3 = yd[i][1]+yd[i+1][1]-2.0*del2;
		yd[i][2] = 2.0*(del2-yd[i][1]-divdf3)/h2;
		yd[i][3] = (divdf3/h2)*(6.0/h2);
	}
	yd[n-1][2] = yd[n-2][2]+(x[n-1]-x[n-2])*yd[n-2][3];
	yd[n-1][3] = yd[n-2][3];
}

/***** Compute One order Diffrential Coefficient *****/
void derivative1( double *xa, double *ya, double *y2a, int *nn, double *xx, double *yy)
{
	int klo, khi, k, khl;
	double x, dy, h, a, b;
	int n;
	
	n = *nn;
	x = *xx;

	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(xa[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	h = xa[khi]-xa[klo];
	if(h==0.0) printf("Bad Input XA\n");
	a = (xa[khi]-x)/h;
	b = (x-xa[klo])/h;
	dy = (ya[khi]-ya[klo])/h+
	     ((1.0-3.0*a*a)/h*y2a[klo]+(3.0*b*b-1.0)/h*y2a[khi])*h*h/6.0;

	*yy = dy;
	
	return;
}
void derivative2( double *xa, double *ya, double *y2a, int *nn, double *xx, double *yy)
{
	int klo, khi, k, khl;
	double x, dyy, h, a, b;
	int n;
	
	n = *nn;
	x = *xx;

	klo = 0;
	khi = n-1;
	khl = khi-klo;
	while(khl>1) {
	  k = (khi+klo)/2;
	  if(xa[k]>x) {
	     khi = k;
	  }
	  else {
	     klo = k;
	  }
	  khl = khi-klo;
	}
	h = xa[khi]-xa[klo];
	if(h==0.0) printf("Bad Input XA\n");
	a = (xa[khi]-x)/h;
	b = (x-xa[klo])/h;
	dyy = a*y2a[klo]+b*y2a[khi];

	*yy = dyy;
	
	return;
}




char * vtoa(int i,char *buffer)
{
	sprintf(buffer,"%d ",i);
	return buffer;
};

char * vtoa(long i,char *buffer)
{
	sprintf(buffer,"%ld ",i);
	return buffer;
};

char * vtoa(double i,char *buffer)
{
	sprintf(buffer,"%1.5lf ",i);
	return buffer;
};

char * vtoa(float i,char *buffer)
{
	sprintf(buffer,"%1.2f ",i);
	return buffer;
};


void pause()
{
	//if(_kbhit() ){      
	//	cDirection1=_getch();
	//while(!_kbhit());
	getchar();
	fflush(stdin);	
}



bool GetLineData(FILE *fp,CFHString &sString,bool bRemoveBlank)
{
	if(feof(fp))return false;
	
	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sString=sLine;

	if(bRemoveBlank)sString.Remove (' ');
	
	long n=sString.GetLength ();	
	if(n>0)sString.SetAt(n-1,'\0');

	return true;
}


bool GetLineData(FILE *fp,int &nData)
{
	if(feof(fp))return false;

	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sscanf(sLine,"%d",&nData);

	return true;	
}

bool GetLineData(FILE *fp,long &nData)
{
	if(feof(fp))return false;

	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sscanf(sLine,"%ld",&nData);

	return true;	
}

bool GetLineData(FILE *fp,unsigned int &nData)
{
	if(feof(fp))return false;

	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sscanf(sLine,"%u",&nData);

	return true;	
}

bool GetLineData(FILE *fp,float &nData)
{
	if(feof(fp))return false;

	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sscanf(sLine,"%f",&nData);

	return true;	
}


bool GetLineData(FILE *fp,double &nData)
{
	if(feof(fp))return false;

	char sLine[256];
	memset(sLine,0,256);
	fgets(sLine,256,fp);
	sscanf(sLine,"%lf",&nData);

	return true;	
}


void ErrorMsg(int fatal_type)
{
	char msg[64];
	sprintf (msg, "CStr fatal %d", fatal_type);
	printf(msg);
}

void ErrorMsg(char sMsg[])
{
	printf(sMsg);
}


CFHString GetFileForOpen(CFHString sType)
{
	// TODO: Add your command handler code here
	printf("Give a file name for open("+sType+"):");
	CFHString sFile;
	scanf("%s",sFile.GetBuffer(200));
	sFile.ReleaseBuffer ();

	return sFile;
}



CFHString GetFileForSave(CFHString sType)
{
	// TODO: Add your command handler code here
	printf("Give a file name for Save("+sType+"):");
	CFHString sFile;
	scanf("%s",sFile.GetBuffer(200));
	sFile.ReleaseBuffer ();

	return sFile;
}

void RemoveInvalidChar(CString &sLine)
{
    sLine.Remove(10);
    sLine.Remove(13);
}

void RemoveInvalidChar(char *pLine)
{
    int n=strlen(pLine);
    for(int i=0;i<n;i++)
    {
        if(pLine[i]==10 || pLine[i]==13)
        {
            pLine[i]=0;
        }
    }
}

CString GetDomainName(EDomain eDomain)
{
    if(eDomain==eTime)
    {
        return "时间";
    }
    else
    {
        return "深度";
    }
}