

#include "stdafx.h"
#include "math.h"
#include "FHCGlobal.h"
#include "FHString.h"
#include "FHMainFrm.h"

#ifdef __AFX_H__


CString GetSubString(CString sString,int nOrder)
{		
	long nLen=sString.GetLength ();
	long *pStartPos=new long[nLen+10];
	memset(pStartPos,0,sizeof(long)*(nLen+10));

	pStartPos[0]=0;

	// Check how many sub strings are there in the string:
	long nNumber=0;
	int nPos=0;
	int nMinusChar=2;
	LPCTSTR pChar=(LPCTSTR)sString;
	while(nPos<nLen+1)
	{
		if(pChar[nPos]=='\r'&&pChar[nPos+1]=='\n')
		{
			nNumber++;
			pStartPos[nNumber]=nPos+2;
			nMinusChar=2;
		}
		else if(pChar[nPos]=='\0')
		{
			nNumber++;
			pStartPos[nNumber]=nPos+1;
			nMinusChar=1;
		}

		nPos++;
	}

	//
	CString sReturn;
	if(nOrder>=0&&nOrder<nNumber)
	{
		int nLen=pStartPos[nOrder+1]-nMinusChar-pStartPos[nOrder];
		if(nLen>0)sReturn=sString.Mid(pStartPos[nOrder],nLen);
	}

	delete []pStartPos;

	return sReturn;

}


CString SeperateName(CString sLineName)
{
	int posStart=sLineName.ReverseFind('\\');
	int posEnd=sLineName.ReverseFind('.');
	if(posEnd==-1)posEnd=sLineName.GetLength();

	CString s=sLineName.Mid(posStart+1,(posEnd-posStart-1));
	s.MakeUpper ();
	return s;
}

CString SeperateExt(CString sLineName)
{
    int posEnd=sLineName.ReverseFind('.');
    if(posEnd==-1)return CString(" ");

    CString s=sLineName.Mid(posEnd+1);
    s.MakeUpper ();
    s.Remove('\10');
    s.Remove(' ');
    s.Remove('\13');
    return s;
}

CString SeperateFileName(CString sLineName)
{
	int posBegin=sLineName.ReverseFind('\\');

	CString s=sLineName.Right (sLineName.GetLength ()-posBegin-1);
	s.MakeUpper ();
	return s;
}	
	
// delete the extension:
CString SeperatePathName(CString sLineName)
{
	int pos=sLineName.ReverseFind('.');
	if(pos==-1)
		return sLineName;
	else
		return sLineName.Left (pos);
}

// Delete the extension and the name 
CString SeperatePath(CString sLineName)
{
	int pos=sLineName.ReverseFind('\\');
	if(pos==-1)
		return CString("");
	else
		return sLineName.Left (pos);
}

double  stovd(CString s)
{
    double d;
    sscanf((LPCTSTR)s,"%lf",&d);
    return d;
}

int     stovi(CString s)
{
    int d;
    sscanf((LPCTSTR)s,"%i",&d);
    return d;
}

CString vtos(long data)
{
	char s[200];
	sprintf(s,"%ld",data);
	return CString (s);
}

CString vtos(int data)
{
	char s[200];
	sprintf(s,"%d",data);
	return CString (s);
}

CString vtos(__int64 nValue)
{
	char s[200];
	sprintf(s,"%I64d",nValue);
	return CString (s);
}

CString vtos(float fData,int nSmallNumberQty)
{
	CString s;

	long nLongData=(long )fData;

	double dSmallNumber=0.001;

	if(nSmallNumberQty==-1)
	{
		if(fData<0.0001)
			nSmallNumberQty=5;
		else if(fData<0.001)
			nSmallNumberQty=4;
		else if(fData<0.01)
			nSmallNumberQty=3;
		else if(fData<0.1)
			nSmallNumberQty=2;
		else if(fData<1)
			nSmallNumberQty=1;

		else if(fabs(fData-long(fData+0.5))<dSmallNumber)
			nSmallNumberQty=0;
		else if(fabs(fData*10-long(fData*10+0.5))<dSmallNumber)
			nSmallNumberQty=1;
		else if(fabs(fData*100-long(fData*100+0.5))<dSmallNumber)
			nSmallNumberQty=2;
		else if(fabs(fData*1000-long(fData*1000+0.5))<dSmallNumber)
			nSmallNumberQty=3;
		else 
			nSmallNumberQty=4;
	}

	if(nSmallNumberQty==0)
		s.Format("%1.0f",fData);
	else if(nSmallNumberQty==1)
		s.Format("%1.1f",fData);
	else if(nSmallNumberQty==2)
		s.Format("%1.2f",fData);
	else if(nSmallNumberQty==3)
		s.Format("%1.3f",fData);
	else if(nSmallNumberQty==4)
		s.Format("%1.4f",fData);
	else if(nSmallNumberQty==5)
		s.Format("%1.5f",fData);
	else if(nSmallNumberQty==6)
		s.Format("%1.6f",fData);
	else
		s.Format("%1.7f",fData);

	return s;
}

CString vtos(double dData,int nSmallNumberQty)
{
	CString s;

	long nLongData=(long )dData;

	double dSmallNumber=0.0000001;
	if(nSmallNumberQty==-1)
	{
		if(fabs(dData-long(dData))<dSmallNumber)
			nSmallNumberQty=0;
		else if(fabs(dData*10-long(dData*10))<dSmallNumber)
			nSmallNumberQty=1;
		else if(fabs(dData*100-long(dData*100))<dSmallNumber)
			nSmallNumberQty=2;
		else if(fabs(dData*1000-long(dData*1000))<dSmallNumber)
			nSmallNumberQty=3;
		else if(fabs(dData*10000-long(dData*10000))<dSmallNumber)
			nSmallNumberQty=4;
		else if(fabs(dData*100000-long(dData*10000))<dSmallNumber)
			nSmallNumberQty=5;
		else 
			nSmallNumberQty=6;
	}

	if(nSmallNumberQty==0)
		s.Format("%1.0lf",dData);
	else if(nSmallNumberQty==1)
		s.Format("%1.1lf",dData);
	else if(nSmallNumberQty==2)
		s.Format("%1.2lf",dData);
	else if(nSmallNumberQty==3)
		s.Format("%1.3lf",dData);
	else if(nSmallNumberQty==4)
		s.Format("%1.4lf",dData);
	else if(nSmallNumberQty==5)
		s.Format("%1.5lf",dData);
	else
		s.Format("%1.6lf",dData);

	return s;
}

float stovf(CString s)
{
	float f=0.0;
	sscanf(s.GetBuffer (),"%f",&f);
	s.ReleaseBuffer ();

	return f;
}

//	Find Line from a file
bool FindLineInFile(FILE *fp, CString &sLineReturn,CString sLeftHeadToFind,bool bRemoveBlank)
{	
	bool bFound=false;
	CString sLine,sLineLeft;

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

bool GetLineData(FILE *fp,CString &sString,bool bRemoveBlank)
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

void copyname(char sNameTar[],CString sNameSrc)
{
	long nChar=sNameSrc.GetLength ();
	memcpy(sNameTar,(LPCTSTR)sNameSrc,nChar);
	memset(&sNameTar[nChar],0,1);
};

long GetMaxCharQtyInLine(CString sFile,long &nLineNumber)
{
	if(sFile=="")return 0;
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		return 0;
	}

	int nMaxChar=0,nChar;
	nLineNumber=0;
	char sChar[300];
	while(!feof(fp))
	{
		fgets(sChar,300,fp);
		nChar=strlen(sChar);
		if(nChar>nMaxChar)
		{
			nMaxChar=nChar;
		}
		nLineNumber++;
	}
	fclose(fp);

	return nMaxChar;	

}


long GetFileLineNumber(CString sFile,int nDataInLine,bool bHasBlankLine)
{
	if(sFile=="")
    {
        return 0;
    }

	FILE *fp=fopen(sFile,"rt");
	if(!fp)
	{
		return 0;
	}

	int n=0,i,nData;
	CString s;
	static double dData[20];
	static bool bInvalid=false;
	static char sLine[1200];

    n=0;

	while(!feof(fp))
    {
        memset(sLine,0,1000);
        fgets(sLine,1000,fp);
		s=sLine;
		s.TrimLeft ();		
		if(!bHasBlankLine && s=="")
		{
			break;
		}
		

		if(nDataInLine!=0)
        {
			for(i=0;i<20;i++)
            {
				dData[i]=-99999;
			}

			strcpy(sLine,(LPCTSTR)s);
			sscanf(sLine,"%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf ",
				&dData[0],&dData[1],&dData[2],&dData[3],&dData[4],
				&dData[5],&dData[6],&dData[7],&dData[8],&dData[9],
				&dData[10],&dData[11],&dData[12],&dData[13],&dData[14],
				&dData[15],&dData[16],&dData[17],&dData[18],&dData[19]);

			for(i=0;i<20;i++)
            {
				if(dData[i]==-99999)
				{
					nData=i;
					break;
				}
			}

			if(nData!=nDataInLine)
            {
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

void scanvalue(char sString[],double &dReturn)
{
	char sReturn[200];
	SeperateValue(sString, sReturn);
	sscanf(sReturn,"%lf",&dReturn);
}

void scanvalue(char sString[],float &fReturn)
{
	char sReturn[200];
	SeperateValue(sString, sReturn);
	sscanf(sReturn,"%f",&fReturn);
}

void scanvalue(char sString[],int &iReturn)
{
	char sReturn[200];
	SeperateValue(sString, sReturn);
	sscanf(sReturn,"%d",&iReturn);
}

void scanvalue(char sString[],long &lReturn)
{
	char sReturn[200];
	SeperateValue(sString, sReturn);
	sscanf(sReturn,"%ld",&lReturn);
}



void SeperateValue(char sString[], char *psReturn)
{
	int n=strlen(sString);
	int i=0;
	while(i<n)
	{
		if(IsNumber(sString[i]))break;
		i++;
	}

	int nStart=i;

	i=n-1;
	while(i>=0)
	{
		if(IsNumber(sString[i]))break;
		i--;
	}
	int nEnd=i;
    if(nEnd<nStart)
    {
        return ;
    }

	int nReLen=nEnd-nStart+1;
	memcpy(psReturn,&sString[nStart],nReLen);

	memset(&psReturn[nReLen],0,2);

	return;

}

bool IsNumber(char c)
{
	return (c>=48&&c<=57);
}

bool ExtractNumber(char sIn[], char sOut[], bool bReverse)
{
	CString sInS(sIn);
	CString sOutS;
	long nStart=0;
	long nEnd=0;
	char c;

	// 逆序查找数字串：
	if(bReverse)
	{
		for(long i=sInS.GetLength ()-1;i>=0;i--)
		{
			c=sInS.GetAt (i);
			if(IsNumber(c))
			{
				nEnd=i;
				break;
			}
		}

		for(long i=nEnd;i>=0;i--)
		{
			c=sInS.GetAt (i);
			if(!IsNumber(c))
			{
				nStart=i+1;
				break;
			}
		}
	}

	// 正序查找数字串：
	else
	{
		for(long i=0;i<sInS.GetLength ();i++)
		{
			c=sInS.GetAt (i);
			if(IsNumber(c))
			{
				nStart=i;
				break;
			}
		}

		for(long i=nStart;i<sInS.GetLength ();i++)
		{
			c=sInS.GetAt (i);
			if(!IsNumber(c))
			{
				nEnd=i-1;
				break;
			}
		}
	}


	int nLen=nEnd-nStart+1;
	if(nLen<=0)return false;

	sOutS=sInS.Mid (nStart,nLen);
	strcpy(sOut,(LPCTSTR)sOutS);

	return true;
}

bool ExtractString(char sIn[], 
				   int nGroupOrder, 
				   int nSubOrder, 
				   char sOut[])
{
	////////////////////////
	// 寻找目标组
	int nCurGroup=0;
	int i=0;
	while(sIn[i]&&nCurGroup<nGroupOrder)
	{
		i++;
		if(sIn[i]==',')
		{
			nCurGroup++;
			i++;  // Jump over the ','
		}
	}

	////////////////////////////
	// 寻找目标子组开始点：
	bool bNumberPre=IsNumber(sIn[i]);
	bool bNumberCur;
	int nCurSubGroup=0;
	while(sIn[i]&&nCurSubGroup<nSubOrder)
	{
		i++;
		bNumberCur=IsNumber(sIn[i]);
		if(bNumberCur!=bNumberPre)
		{
			nCurSubGroup++;
			bNumberPre=bNumberCur;
		}
	}
	int nSubStart=i;

	///////////////////////////
	// 寻找子组结束点：
	int nSubEnd=strlen(sIn);
	while(sIn[i])
	{
		i++;
		bNumberCur=IsNumber(sIn[i]);
		if(bNumberCur!=bNumberPre||sIn[i]==';')
		{
			nSubEnd=i-1;
			bNumberPre=bNumberCur;
			break;
		}
	}

	////////////////////////////
	// 拷贝输出：
	int nLen=nSubEnd-nSubStart+1;
	if(nLen<=0)return false;
	memcpy(sOut,&sIn[nSubStart],nLen);
	sOut[nLen]='\0';

	return true;
}

unsigned long GetFileLen(FILE *fp)
{
	unsigned long nCurPos=ftell(fp);
	fseek(fp,0,SEEK_END);
	unsigned long nLen=ftell(fp);
	fseek(fp,nCurPos,SEEK_SET);

	return nLen;
}

//////////////////////////////////
// 获取文件大小：
long GetFileLen(CString sFile)
{
	FILE *fp=fopen(sFile,"rb");
	fseek(fp,0,SEEK_END);
	long nFileLen = ftell(fp);
	fclose(fp);

	return nFileLen;
}


void ConstructFilterString(char *cFilter, CString sDiscription, CString sExt)
{
	if(sExt.Left (1)==".")sExt=sExt.Mid (1);
	int nExt=sExt.GetLength();

    if(sDiscription.Right(1)!=")")
    {
        sDiscription+="(*."+sExt+")";
    }

    int nDisc=sDiscription.GetLength();
	strcpy(cFilter,(LPCTSTR)sDiscription);
	cFilter[nDisc]='\0';
	cFilter[nDisc+1]='*';
	cFilter[nDisc+2]='.';
	strcpy(&cFilter[nDisc+3],(LPCTSTR)sExt);
	cFilter[nDisc+nExt+3]='\0';
	cFilter[nDisc+nExt+4]='\0';
}



CString GetFileForOpen(CString sDiscription, CString sExt, bool bMultiFile, CStringArray *pArray)
{
	CFileDialog dlg(true);

	char cFilter[1000];
	ConstructFilterString(cFilter,sDiscription,sExt);
	dlg.m_ofn.lpstrFilter=cFilter;

	char sOpen[100]={"打开文件"};
	dlg.m_ofn.lpstrTitle=sOpen;

    if(bMultiFile)
    {
        dlg.m_ofn.Flags|=OFN_ALLOWMULTISELECT;
    }


	char sExtension[100];
	int n=sExt.ReverseFind ('.');
	if(n!=-1)
	{
		sExt=sExt.Mid (n+1);
		if(sExt=="*")
		{
			sExt="";
		}
	}

	sprintf(sExtension,"%s",sExt);
	dlg.m_ofn.lpstrDefExt=sExtension;

	if(dlg.DoModal()==IDCANCEL)
    {
        return CString("");
    }

    CString sFileName;
	
    if(!bMultiFile)
    {
	    sFileName= dlg.GetPathName  ();
        return sFileName;
    }
    else 
    {
        if(pArray)
        {
            POSITION pos=dlg.GetStartPosition();
            while(pos)
            {
                sFileName=dlg.GetNextPathName(pos);
                pArray->Add(sFileName);
            }
        }

        return CString("");
    }

}

bool GetMultiFileForOpen(CString sDiscription, CString sExt,CStringArray &array)
{
	CFileDialog dlg(TRUE,sExt,sDiscription,OFN_ALLOWMULTISELECT); 
   	char cFilter[1000];
	ConstructFilterString(cFilter,sDiscription,sExt);
	dlg.m_ofn.lpstrFilter=cFilter;

	char s[5000];
    memset(s,0,5000);
	dlg.m_ofn.lpstrFile=s;
	dlg.m_ofn.nMaxFile=5000;

	if(dlg.DoModal()==IDCANCEL)
    {
		return FALSE;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   将存有文件名的内存区域转化为文件名数组.
    char sFileName[5000];
	int i,n=0;
	for(i=0;i<5000;i++)
    {
		sFileName[n]=s[i];
		n++;
        if(s[i]<0)
        {
            continue;
        }

		else if(iscntrl(s[i]))
        {
			array.Add(sFileName);
			n=0;
            if(s[i+1]<0)
            {
                continue;
            }
            else
            {
			    if(iscntrl(s[i+1]))break;
            }
		}
	}
    // 如果只有一个文件被选中, 则只有一个文件名.
    // 若有多个文件被选中, 则第一个文件名是路径名,其次才是各个文件名.  
    CString sFile;
    if(array.GetCount ()!=1)
    {
        CString sPath=array.GetAt(0);
        if(sPath.Right (1)!="\\")
        {
            sPath+="\\";
        }
        for(i=1;i<array.GetCount();i++)
        {
            sFile=sPath+array.GetAt(i);
            array.SetAt(i-1,sFile);
        }
        array.RemoveAt(array.GetCount()-1);
    }

    return true;

}

CString GetFileForSave(CString sDiscription, CString sExt)
{
	CFileDialog dlg(false);

	char cFilter[100];
	ConstructFilterString(cFilter,sDiscription,sExt);
	dlg.m_ofn.lpstrFilter=cFilter;

	char sOpen[100]={"存储文件"};
	dlg.m_ofn.lpstrTitle=sOpen;

	char sExtension[100];
	int n=sExt.ReverseFind ('.');
	if(n!=-1)
	{
		sExt=sExt.Mid (n+1);
		if(sExt=="*")
		{
			sExt="";
		}
	}
	sprintf(sExtension,"%s",sExt);
	dlg.m_ofn.lpstrDefExt=sExtension;

	if(dlg.DoModal()==IDCANCEL)
    {
        return CString("");
    }
	
	return dlg.GetPathName ();
}


bool Equal(double dValue1, double dValue2,double dPrecision)
{
	if(dPrecision<0.0000000001)
	{
		dPrecision=0.0000000001;
	}
	if(fabs(dValue1-dValue2)<dPrecision)return true;

	return false;
}

bool Equal(float fValue1, float fValue2,float fPrecision)
{
	if(fPrecision<0.0000000001)
	{
		fPrecision=0.0000000001;
	}
	if(fabs(fValue1-fValue2)<fPrecision)return true;

	return false;
}

long GetDataQtyInLine(CString sFile)
{
	FILE *fp=fopen(sFile,"rt");
	if(!fp)return 0;

	char sLine[300];
	fgets(sLine,300,fp);
	fclose(fp);

	return GetDataQtyInLine(sLine);
}


void RTrim(char *s)
{
	unsigned int good = strlen(s);
	
	if (good == 0)
		return;
	
	while (good > 0  
		&&  (s[good-1] == ' ' 
		  || s[good-1]==0 
		  || s[good-1]==10 
		  || s[good-1]==13))
	{
		--good;
	}

	if (good >= strlen(s))
		return;

	s[good] = 0;

}

void Trim(char *s)
{
	LTrim(s);
	RTrim(s);
}


void LTrim(char *s)
{
	int i=0;
	while (s[i] == ' ')
	{
		i++;
	}

	if(i>0)
	{
		int n=strlen(s);
		if (n <= i || i==0) 
		{
			return;
		}

		memmove (s, s+i, n-i+1);
	}

	return;
}

long GetDataQtyInLine(char sLine[])
{
	char *pLine=NULL;
	Trim(sLine);
	int n=1;
	pLine=strchr(sLine,' ');
	while(pLine)
	{
		n++;
		Trim(pLine);
		pLine=strchr(sLine,' ');
	};

	return n;
}

void Warn(UINT nIDS)
{
	CString s;
	s.LoadString (nIDS);
	Warn(s);
}

void Warn(UINT nIDS,CString sValue)
{
	CString s;
	s.LoadString (nIDS);
	if(s.Right (1)!=":")
	{
		s+=":";
	}
    
	Warn(s+":"+sValue);
}


void Warn(CString s)
{
	AfxMessageBox(s);
}

bool RemoveFile(CString sFile)
{
	// 测试文件是否存在
	CFileStatus rStatus;
	if(CFile::GetStatus(sFile,rStatus ))
	{
		// 删除之：
		TRY
		{
			CFile::Remove(sFile);
            return true;
		}
		CATCH( CFileException, e )
		{
            return false;
		}
		END_CATCH
	}

    return true;

}

void PurifyNumber(char sLine[])
{
	for(int i=0;i<strlen(sLine);i++)
	{
		if(!IsNumber(sLine[i]))
		{
			sLine[i]=' ';
		}
	}
}

void WriteString(FILE *fp, CString s)
{
    int n=s.GetLength();
    fwrite(&n,sizeof(int),1,fp);
    fwrite((LPCTSTR)s,1,n,fp);
}
void ReadString(FILE *fp, CString &s)
{
    int n;
    char sBuffer[1000];
    fread(&n,sizeof(int),1,fp);
    if(n<0||n>sizeof(sBuffer))
    {
        return;
    }
    memset(sBuffer,0,sizeof(sBuffer));

    fread(sBuffer,1,n,fp);
    s=sBuffer;

    return;
}

void WriteData(FILE *fp, char &c)
{
    fprintf(fp,"%c\n",c);
}
void WriteData(FILE *fp, int &i)
{
    fprintf(fp,"%d\n",i);
}
void WriteData(FILE *fp, unsigned int &i)
{
    fprintf(fp,"%u\n",i);
}
void WriteData(FILE *fp, double &d)
{
    fprintf(fp,"%f\n",d);
}

#endif

