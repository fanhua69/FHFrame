
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
// 这是软件系列号，由软件发布人
// 利用UltraEdit修改
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

	// 软件狗的4组密码：
	m_handle=0;
	m_nReturnCode=0;
	m_lp1=0;
	m_lp2=0;
	memset(m_buffer,0,sizeof(BYTE)*1024);

	//////////////////////////
	// 生成系列号
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
	// 准备写入的公式：
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

	m_p1[3] = 0x73ef; 	// the 4th 狗的密码
	m_p2[3] = 0x734c;	// 4狗的密码

	m_nPassQty=4;
}

bool CSoftDog::OpenDog()
{
	int i=0;

	/////////////////////////////
	// 检测密码
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
    // 打开狗
    ////////////////////////
    if(m_handle==0&&!OpenDog())
    {
        return false;
    }

    /////////////////////////////
    // 读取狗中存在的起始有效时间：
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
    // 读取狗中存在的中止有效时间：
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
	// 打开狗
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	/////////////////////////////
	// 读取标志并比较：
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
	// 打开狗
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	//读取用户ID:
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
	// 用户ID是以16进制存储的
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
// 写入随机数：
/////////////////////////////
bool CSoftDog::WriteRand(int iPos)
{
    ////fanhua
    //return true;

    ///////////////////////
	// 打开狗
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	// 建立随机数:
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
	// 写入随机数，以备以后检查：
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
// 检测随机数：
/////////////////////////////
bool CSoftDog::CheckRand(int iPos)
{
    ////fanhua
    //return true;

	///////////////////////
	// 打开狗
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	////////////////////////////////////
	// 读取随机数：
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
	// 打开狗
	////////////////////////
	if(m_handle==0&&!OpenDog())
	{
		return false;
	}

	///////////////////////
	//测试计算结果
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
		sError="此机没有并口";
		break;
	case 2:
		sError="没有安装驱动程序";
		break;
	case 3:
		sError="没有安装ROCKEY软件狗";
		break;
	case 4:
		sError="基本密码错误";
		break;
	case 5:
		sError="错误的密码或者硬件ID";
		break;
	case 6:
		sError="设置硬件ID错误";
		break;
	case 7:
		sError="读写地址或者长度有误";
		break;
	case 8:
		sError="没有此命令";
		break;
	case 9:
		sError="内部错误";
		break;
	case 10:
		sError="读数据错误";
		break;
	case 11:
		sError="写数据错误";
		break;
	case 12:
		sError="随机数据";
		break;
	case 13:
		sError="种子码错误";
		break;
	case 14:
		sError="计算错误";
		break;
	case 15:
		sError="在操作前没有打开锁";
		break;
	case 16:
		sError="打开的锁太多>16";
		break;
	case 17:
		sError="找不到更多的锁";
		break;
	case 18:
		sError="没有Find直接用了FindNext";
		break;
	case 19:
		sError="递减错误";
		break;
	case 20:
		sError="算法指令错误";
		break;
	case 21:
		sError="算法运算符错";
		break;
	case 22:
		sError="算法的第一条指令含有常数";
		break;
	case 23:
		sError="算法的最后一条指令含有常数";
		break;
	case 24:
		sError="算法中常数值>63";
		break;
	case 0x100:
		sError="接收不到";
		break;
	case 0x101:
		sError="打印口忙";
		break;
	case 0x102:
		sError="未知的操作系统";
		break;
	case 0xffff:
		sError="未知错误";
		break;
	}

	AfxMessageBox(sError);
}


bool CSoftDog::InstallRockyDriver(bool bInstall)
{
    return true;
	char para[255];

	//PINSTROCKEY		pInstRockey=NULL;	//安装函数指针，参见install.h说明
	//PISNEEDRESTART	pIsNeedRestart=NULL;//判断是否需要重起函数,参见install.h说明

	//HMODULE		hDll=NULL;			//库的句柄
	//LONG		lRet;				//安装返回值


	////加载instdll.dll 请确保instdll.dll在当前目录或系统目录
 //   hDll=LoadLibrary("InstDll.dll");
	//if(NULL==hDll)
	//{
	//	AfxMessageBox("无法找到Instdll.dll.请拷贝到本地目录.\n");
	//	return 0;
	//}

	////得到函数指针,有两个函数
	//pInstRockey=(PINSTROCKEY)GetProcAddress(hDll,"InstRockey");
	//if(NULL==pInstRockey)
	//{
	//	AfxMessageBox("InstDll.dll 错误... 请更新\n");
	//	FreeLibrary(hDll);
	//	return 0;
	//}
	//pIsNeedRestart=(PISNEEDRESTART)GetProcAddress(hDll,"IsNeedRestart");
	//if(NULL==pIsNeedRestart)
	//{
	//	AfxMessageBox("无法找到IsNeedRestart函数, 请更新Instdll.dll\n");		
	//}

	////输入安装参数,参见说明书参数说明
	////AfxMessageBox("Please input install parameter:\n");
	////gets(para);
	///*******************************************
	///? 显示命令行帮助
 //  /i 不显示界面直接安装
 //  /r 不显示界面直接卸载
 //  /s 安装不显示任何信息
 //  /T0 不测试并口忙(默认值,可不加)
 //  /T1 测试并口忙
 //  /P0 支持并口锁和USB锁(默认值,可不加)
 //  /P1 只支持并口锁
 //  /P2 只支持USB锁
 //  /N 如果检测到已经安装了驱动程序，则不再安装
 //  /D278 搜索时禁止使用278端口
 //  /D378或/D3BC 同上,禁止使用指定端口.
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
	////AfxMessageBox("开始安装软件狗驱动程序...\n");
	////调用函数
	//lRet=pInstRockey(NULL,NULL,para);

	//bool bFailed=false;
	//if(INST_ERROR_SUCCESS==lRet)	
	//{
	//	AfxMessageBox("软件狗驱动程序安装成功.\n");	

	//	//检查是否需要重起
	//	if(pIsNeedRestart)
	//	{
	//		if(pIsNeedRestart())
	//			AfxMessageBox("你需要重新启动计算机以使软件狗驱动程序生效.\n");
	//		else
	//			AfxMessageBox("不需要重新启动计算机,软件狗驱动程序已经生效.\n");
	//	}
	//}
	//	
	//else if(INST_ERROR_NONEEDINSTALL==lRet)
	//{		
	//}
	//else
	//{
	//	char sErr[200];
	//	sprintf(sErr,"软件狗驱动程序安装错误,错误码:%d",lRet);
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

