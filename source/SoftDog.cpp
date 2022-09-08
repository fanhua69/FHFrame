
#include "StdAfx.h"
#include "..\\include\\softdog.h"

#define DOG_ROCKY_SMART

#ifndef DOG_ROCKY_SMART
    #include "ryvc32.h" // rockey 4 dog
#else
    #include "ry4s.h"  // rockey smart dog
#endif

#include "fhcglobal.h"


// fanhua
//#pragma comment(lib, "d:\\projects\\fhframe\\include\\Ry4S.lib")
//#pragma comment(lib, "d:\\projects\\fhframe\\ryvc32.obj")

//////////////////////////////////
// �������ϵ�кţ������������
// ����UltraEdit�޸�
//////////////////////////////////
char sTest[]={"900829"};
int GL_SERIALNO1[4] ={34,12,43,48};
int GL_SERIALNO2[4] ={12,33,44,47};
int GL_SERIALNO[20]  ={89,88,86,83,44,73,75,70,82,44,47,47,48,50,53,-1,-1,-1,-1}; //ZYWT-JLGS-00113
int GL_SERIALNO3[20] ={43,68,26,43,74,71,15,30,42,54,67,17,27,39,24,47,47,47,48};
//int GL_IDNO     [20] ={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 54,98,56,50,51,55,99,49};
//int GL_IDNO     [20] ={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 53,51,56,51,54,102,98,101};
//int GL_IDNO     [20] ={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 52,51,50,101,54,58,103,98};// this is the dog number;
int GL_IDNO     [20] ={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 50,100,57,99,52,98,54,100};// this is the dog number;
int GL_IDNO2    [20] ={0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 , 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 };


CSoftDog::CSoftDog(void) //36,36,36,36,36,36,36,36};
{
	m_nRandNumber=0;

	// �������4�����룺
	m_handle=0;
	m_nReturnCode=0;
	m_lp1=0;
	m_lp2=0;
	memset(m_buffer,0,sizeof(BYTE)*1024);

	//////////////////////////
	// ����ϵ�к�
	//////////////////////////
	CString sSer;
	char *p=sSer.GetBuffer(100);
	for(int i=0;i<17;i++)
	{
		p[i]=(GL_SERIALNO[i]+1);
	}
	sSer.ReleaseBuffer ();

	sSer=sSer.Right (5);
	sscanf((LPCTSTR)sSer,"%ld",&m_nSerNumber);

	///////////////////////////////
	// ׼��д��Ĺ�ʽ��
	///////////////////////////////
	char cmd[] = "H=H^H, A=A*23, F=B*17, A=A+F, A=A+G, A=A<C, A=A^D, B=B^B, C=C^C, D=D^D";
	char cmd1[] = "A=A+B, A=A+C, A=A+D, A=A+E, A=A+F, A=A+G, A=A+H";
	char cmd2[] = "A=E|E, B=F|F, C=G|G, D=H|H";

	ResetCheckedMark();
}

void CSoftDog::ResetCheckedMark()
{
	m_bMarkChecked	=false;
	m_bUserIDChecked=false;
	m_bRandChecked	=false;
	m_bCalcChecked	=false;
}

CSoftDog::~CSoftDog(void)
{
	CloseDog();
}

void CSoftDog::SetPassword()
{
	m_p1[0] = 0xc44c;   // The first dog's password;
	m_p2[0] = 0xc8f8;

	m_p1[1] = 0xb0ef;   // the second dog's password;
	m_p2[1] = 0xbf7c;

	m_p1[2] = 0x8CEF;   // the fourth dog's password;
	m_p2[2] = 0xBF8C;

	m_p1[3] = 0x73ef; 	// the 4th ��������
	m_p2[3] = 0x734c;	// 4��������

	m_nPassQty=4;
}

bool CSoftDog::OpenDog()
{
	int i=0;

	/////////////////////////////
	// �������
	/////////////////////////////
	SetPassword();

	int nFound=-1;
	for(i=0;i<m_nPassQty;i++)
	{
		m_nReturnCode = Rockey(RY_FIND, &m_handle, &m_lp1, &m_lp2, &m_p1[i], &m_p2[i], &m_p3[i], &m_p4[i], m_buffer);
		if(m_nReturnCode==0)
		{
			nFound=i;
			break;
		}
	}
	if (nFound==-1||m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

    // This is the only Dog number:
	char sIDNumber[100];
	sprintf(sIDNumber,"%x",m_lp1);

    //
    char sCheck[100];
    for(i=0;i<8;i++)
    {
        sCheck[i]=GL_IDNO[11+i]-1;
    }

    //
	bool bSame=true;
	for(i=0;i<8;i++)
	{
		if(sIDNumber[i]!=GL_IDNO[11+i]-1)
		{
			bSame=false;
 			break;
		}
	}

	if(!bSame)
	{
		return false;
	}

	m_nReturnCode = Rockey(RY_OPEN, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

	return true;
}

bool CSoftDog::CloseDog()
{
	if(m_handle==0)return true;

	int nFound=0;

	m_nReturnCode = Rockey(RY_CLOSE, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

	m_handle=0;

	return true;
}

bool CSoftDog::_getTime(char sBuffer[], 
                        int &nYear, 
                        int &nMonth, 
                        int &nDay, 
                        int &nHour, 
                        int &nMinute, 
                        int &nSecond)
{
    int i=0;
    for(i=0;i<16;i++)
    {
        if(sBuffer[i]==0)
        {
            sBuffer[i]=32;
        }
    }
    CString s(sBuffer);
    nYear   =stovf(s.Left(4));
    nMonth  =stovf(s.Mid(4,2));
    nDay    =stovf(s.Mid(6,2));
    nHour   =stovf(s.Mid(8,2));
    nMinute =stovf(s.Mid(10,2));
    nSecond =stovf(s.Mid(12,2));

    if(nYear<1900||nYear>2100)
    {
        return false;
    }
    if(nMonth<1 || nMonth>12)
    {
        return false;
    }
    if(nDay<1 || nDay>31)
    {
        return false;
    }
    if(nHour<0||nHour>24)
    {
        return false;
    }
    if(nMinute<0||nMinute>60)
    {
        return false;
    }
    if(nSecond<0||nSecond>60)
    {
        return false;
    }
    return true;
}

bool CSoftDog::CheckTimeMark()
{
    //
    int nYear,nMonth,nDay,nHour,nMinute,nSecond;
    CString s;

    ///////////////////////
    // �򿪹�
    ////////////////////////
    if(m_handle==0&&!OpenDog())
    {
        return false;
    }

    /////////////////////////////
    // ��ȡ���д��ڵ���ʼ��Чʱ�䣺
    int nFound=0,i;
    m_p1[nFound] = 16;
    m_p2[nFound] = 16;
    memset(m_buffer, 0, 64);
    m_nReturnCode = Rockey(RY_READ, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
    if (m_nReturnCode)
    {
#ifdef DOG_ROCKY_SMART
        ShowERR(m_nReturnCode);
        return false;
#else
        return true; // this should be the old dog;
#endif
    }

    if(!_getTime((char*)m_buffer,nYear,nMonth,nDay,nHour,nMinute,nSecond))
    {
        // There is no time that were set:
        s=m_buffer;
        s.Trim();
        if(s=="")
        {
            return true;
        }

        //
        return false;
    }

    CTime   timeExistStart(nYear,nMonth,nDay,nHour,nMinute,nSecond);

    /////////////////////////////
    // ��ȡ���д��ڵ���ֹ��Чʱ�䣺
    m_p1[nFound] = 32;
    m_p2[nFound] = 16;
    memset(m_buffer, 0, 64);
    m_nReturnCode = Rockey(RY_READ, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
    if (m_nReturnCode)
    {
        ShowERR(m_nReturnCode);
        return false;
    }
    s=m_buffer;
    s.Trim();
    if(s=="")
    {
        nYear=2500;
        nMonth=1;
        nDay=1;
        nHour=1;
        nMinute=1;
        nSecond=1;
    }
    else
    {
        if(!_getTime((char*)m_buffer,nYear,nMonth,nDay,nHour,nMinute,nSecond))
        {
            return false;
        }
    }
    CTime   timeExistEnd(nYear,nMonth,nDay,nHour,nMinute,nSecond);

    // Compare the existing time with current time:
    CTime timeCurrent=CTime::GetCurrentTime();
    
    bool bFailed=false;
    if(timeCurrent<timeExistStart ||timeCurrent>timeExistEnd )
    {
        bFailed=true;
    }

    // Write the current system time into the dog:
    m_p1[nFound] = 16;
    m_p2[nFound] = 16;
    memset(m_buffer, 0, 64);

    nYear   =timeCurrent.GetYear();
    nMonth  =timeCurrent.GetMonth();
    nDay    =timeCurrent.GetDay();
    nHour   =timeCurrent.GetHour();
    nMinute =timeCurrent.GetMinute();
    nSecond =timeCurrent.GetSecond();

    sprintf((char*)&m_buffer[0],"%04d",nYear);
    sprintf((char*)&m_buffer[4],"%02d",nMonth);
    sprintf((char*)&m_buffer[6],"%02d",nDay);
    sprintf((char*)&m_buffer[8],"%02d",nHour);
    sprintf((char*)&m_buffer[10],"%02d",nMinute);
    sprintf((char*)&m_buffer[12],"%02d",nSecond);

    m_nReturnCode = Rockey(RY_WRITE, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);

    return !bFailed;
}

bool CSoftDog::CheckMark()
{
	///////////////////////
	// �򿪹�
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	/////////////////////////////
	// ��ȡ��־���Ƚϣ�
	int nFound=0;
	m_p1[nFound] = 0;
	m_p2[nFound] = 8;
	memset(m_buffer, 0, 64);
	m_nReturnCode = Rockey(RY_READ, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}
	CString s(m_buffer);
	CString sHead=s.Left(4);
	if(sHead!="JLGS"&&sHead!="EAST")
	{
		return false;
	}

	m_bMarkChecked=true;

	CString sNum=s.Right (4);
	long nNum=-1;
	sscanf((LPCTSTR)sNum,"%ld",&nNum);
	if(nNum!=m_nSerNumber)
	{
		return false;
	}
	
	return true;
}

bool CSoftDog::CheckUserID()
{
	///////////////////////
	// �򿪹�
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	//��ȡ�û�ID:
	///////////////////////
	int nFound=0;
	m_lp1 = 0;
	m_nReturnCode = Rockey(RY_READ_USERID, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

	//////////////////////////
	// �û�ID����16���ƴ洢��
	//////////////////////////
	char errmsg[1000];
	sprintf(errmsg,"%08X", m_lp1);
	CString s=CString(errmsg);

	m_bUserIDChecked=true;

	long nLp1;
	sscanf((LPCTSTR)s,"%ld",&nLp1);

	if(nLp1!=(10000000+m_nSerNumber))
	{
		return false;
	}


	return true;
}

/////////////////////////////
// д���������
/////////////////////////////
bool CSoftDog::WriteRand(int iPos)
{
    ////fanhua
    //return true;

    ///////////////////////
	// �򿪹�
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	// ���������:
	///////////////////////
	int nFound=0;
	m_p1[nFound] = 0;
	m_nReturnCode = Rockey(RY_RANDOM, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}
	m_nRandNumber=m_p1[nFound];

	///////////////////////
	// д����������Ա��Ժ��飺
	///////////////////////
	if(iPos<0)iPos=0;
	if(iPos>1)iPos=1;
	sprintf((char*)m_buffer,"%04X",m_nRandNumber);
	m_p1[nFound]=8+iPos*4;
	m_p2[nFound]=4;
	m_nReturnCode = Rockey(RY_WRITE, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);

	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

	return true;
}

/////////////////////////////
// ����������
/////////////////////////////
bool CSoftDog::CheckRand(int iPos)
{
    ////fanhua
    //return true;

	///////////////////////
	// �򿪹�
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	////////////////////////////////////
	// ��ȡ�������
	////////////////////////////////////
	int nFound=0;

	if(iPos<0)iPos=0;
	if(iPos>1)iPos=1;

	m_p1[nFound]=8+iPos*4;
	m_p2[nFound]=4;
	m_nReturnCode = Rockey(RY_READ, &m_handle, &m_lp1, &m_lp2, &m_p1[nFound], &m_p2[nFound], &m_p3[nFound], &m_p4[nFound], m_buffer);
	
	m_bRandChecked=true;

	long nReadOut;
	sscanf((char*)m_buffer,"%x",&nReadOut);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}
	
	return (nReadOut==m_nRandNumber);
}

bool CSoftDog::CheckCalc(WORD &v1,WORD &v2,WORD &v3,WORD &v4)
{
    ////fanhua
    //return true;


	///////////////////////
	// �򿪹�
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	//���Լ�����
	///////////////////////
	WORD v1b=v1;
	WORD v2b=v2;
	WORD v3b=v3;
	WORD v4b=v4;
	CalcResult(v1b,v2b,v3b,v4b);

	m_bCalcChecked=true;

	m_lp1=0;
	m_lp2=0;
	m_nReturnCode = Rockey(RY_CALCULATE1, &m_handle, &m_lp1, &m_lp2, &v1, &v2, &v3, &v4, m_buffer);
	if (m_nReturnCode)
	{
		ShowERR(m_nReturnCode);
		return false;
	}

	if(v1==v1b && v2==v2b && v4==v4b)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void CSoftDog::ShowERR(int nError)
{
	if (m_nReturnCode == 0) return;

	CString sError;
	switch (m_nReturnCode)
	{
	case 1:
		sError="�˻�û�в���";
		break;
	case 2:
		sError="û�а�װ��������";
		break;
	case 3:
		sError="û�а�װROCKEY�����";
		break;
	case 4:
		sError="�����������";
		break;
	case 5:
		sError="������������Ӳ��ID";
		break;
	case 6:
		sError="����Ӳ��ID����";
		break;
	case 7:
		sError="��д��ַ���߳�������";
		break;
	case 8:
		sError="û�д�����";
		break;
	case 9:
		sError="�ڲ�����";
		break;
	case 10:
		sError="�����ݴ���";
		break;
	case 11:
		sError="д���ݴ���";
		break;
	case 12:
		sError="�������";
		break;
	case 13:
		sError="���������";
		break;
	case 14:
		sError="�������";
		break;
	case 15:
		sError="�ڲ���ǰû�д���";
		break;
	case 16:
		sError="�򿪵���̫��>16";
		break;
	case 17:
		sError="�Ҳ����������";
		break;
	case 18:
		sError="û��Findֱ������FindNext";
		break;
	case 19:
		sError="�ݼ�����";
		break;
	case 20:
		sError="�㷨ָ�����";
		break;
	case 21:
		sError="�㷨�������";
		break;
	case 22:
		sError="�㷨�ĵ�һ��ָ��г���";
		break;
	case 23:
		sError="�㷨�����һ��ָ��г���";
		break;
	case 24:
		sError="�㷨�г���ֵ>63";
		break;
	case 0x100:
		sError="���ղ���";
		break;
	case 0x101:
		sError="��ӡ��æ";
		break;
	case 0x102:
		sError="δ֪�Ĳ���ϵͳ";
		break;
	case 0xffff:
		sError="δ֪����";
		break;
	}

	AfxMessageBox(sError);
}


bool CSoftDog::InstallRockyDriver(bool bInstall)
{
    return true;
	char para[255];

	//PINSTROCKEY		pInstRockey=NULL;	//��װ����ָ�룬�μ�install.h˵��
	//PISNEEDRESTART	pIsNeedRestart=NULL;//�ж��Ƿ���Ҫ������,�μ�install.h˵��

	//HMODULE		hDll=NULL;			//��ľ��
	//LONG		lRet;				//��װ����ֵ


	////����instdll.dll ��ȷ��instdll.dll�ڵ�ǰĿ¼��ϵͳĿ¼
 //   hDll=LoadLibrary("InstDll.dll");
	//if(NULL==hDll)
	//{
	//	AfxMessageBox("�޷��ҵ�Instdll.dll.�뿽��������Ŀ¼.\n");
	//	return 0;
	//}

	////�õ�����ָ��,����������
	//pInstRockey=(PINSTROCKEY)GetProcAddress(hDll,"InstRockey");
	//if(NULL==pInstRockey)
	//{
	//	AfxMessageBox("InstDll.dll ����... �����\n");
	//	FreeLibrary(hDll);
	//	return 0;
	//}
	//pIsNeedRestart=(PISNEEDRESTART)GetProcAddress(hDll,"IsNeedRestart");
	//if(NULL==pIsNeedRestart)
	//{
	//	AfxMessageBox("�޷��ҵ�IsNeedRestart����, �����Instdll.dll\n");		
	//}

	////���밲װ����,�μ�˵�������˵��
	////AfxMessageBox("Please input install parameter:\n");
	////gets(para);
	///*******************************************
	///? ��ʾ�����а���
 //  /i ����ʾ����ֱ�Ӱ�װ
 //  /r ����ʾ����ֱ��ж��
 //  /s ��װ����ʾ�κ���Ϣ
 //  /T0 �����Բ���æ(Ĭ��ֵ,�ɲ���)
 //  /T1 ���Բ���æ
 //  /P0 ֧�ֲ�������USB��(Ĭ��ֵ,�ɲ���)
 //  /P1 ֻ֧�ֲ�����
 //  /P2 ֻ֧��USB��
 //  /N �����⵽�Ѿ���װ�������������ٰ�װ
 //  /D278 ����ʱ��ֹʹ��278�˿�
 //  /D378��/D3BC ͬ��,��ֹʹ��ָ���˿�.
	//*******************************************/
	//if(bInstall)
	//{
	//	strcpy(para,"/i");
	//}
	//else
	//{
	//	strcpy(para,"/r");
	//}
	//
	////AfxMessageBox("��ʼ��װ�������������...\n");
	////���ú���
	//lRet=pInstRockey(NULL,NULL,para);

	//bool bFailed=false;
	//if(INST_ERROR_SUCCESS==lRet)	
	//{
	//	AfxMessageBox("�������������װ�ɹ�.\n");	

	//	//����Ƿ���Ҫ����
	//	if(pIsNeedRestart)
	//	{
	//		if(pIsNeedRestart())
	//			AfxMessageBox("����Ҫ���������������ʹ���������������Ч.\n");
	//		else
	//			AfxMessageBox("����Ҫ�������������,��������������Ѿ���Ч.\n");
	//	}
	//}
	//	
	//else if(INST_ERROR_NONEEDINSTALL==lRet)
	//{		
	//}
	//else
	//{
	//	char sErr[200];
	//	sprintf(sErr,"�������������װ����,������:%d",lRet);
	//	AfxMessageBox(sErr);
	//	bFailed=true;
	//}
	//FreeLibrary(hDll);
	//
	//return !bFailed;
}

void CSoftDog::CalcResult(WORD &v1,WORD &v2,WORD &v3,WORD &v4)
{
	v1=v1+GetSerNumber(); 

#ifndef DOG_ROCKY_SMART
	v1=v1^0xFFFF;
#endif
}

