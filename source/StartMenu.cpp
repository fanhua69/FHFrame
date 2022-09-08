#include "StdAfx.h"
#include "FHFrame.h"
#include "..\include\startmenu.h"
#include "FHMainFrm.h"

CStartMenu::CStartMenu(void)
{
}

CStartMenu::~CStartMenu(void)
{
}


////////////////////////////////// 
// 向开始菜单添加快捷方式
void CStartMenu::AddToStartMenu(char sMyPath[], char sMyShortCut[],char sDiscription[]) 
{
	// 保存找到的\Windows\Start Menu\Programs路径
	char chDir[512];  

	long result;
	HKEY hKey;    // 注册表打开键的句柄
	DWORD dwType;
	DWORD dwSize;
	
	// 打开注册表 
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
						NULL,KEY_QUERY_VALUE,&hKey);	
	
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		// 查询Programs的键值并把查询到的结果
		// 保存在字符串“chDir”中（因为Windows
		// 并不一定安装在c盘中，所以要查询注册表）
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey); // 关闭注册表
		strcat(chDir,"\\"); 
		strcat(chDir,sMyPath);
		
		// 在“开始“菜单里建立 菜单组
		CreateDirectory(chDir,NULL);        // 创建目录
		
		
		strcat(chDir,"\\");
		strcat(chDir,sMyShortCut) ;// "\\Myshortcut.lnk");        
		strcat(chDir,".lnk");
		
		//在“开始“菜单里建立“Myshortcut”菜单项
		HRESULT hres;
		IShellLink* pShellLink=NULL;	// 得到IshellLink接口指针
		CoInitialize(NULL);				// 初始化COM类库
		hres=CoCreateInstance(	CLSID_ShellLink,
								NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,
								(LPVOID *)&pShellLink);
  		
		// 建立一个IID_IShellLink实例
		if(SUCCEEDED(hres))
		{
			// IpersistFile接口提供了允许
			// 将一个对象从磁盘加载或保存到磁盘的方法
			IPersistFile*  pPf=NULL;
			// 得到当前运行的可执行程序的全路径名
			TCHAR exeFullPath[MAX_PATH];
			GetModuleFileName(NULL,exeFullPath,MAX_PATH);
			pShellLink->SetPath(exeFullPath);
			
			// 设置快捷方式的路径
			pShellLink->SetDescription(sDiscription); // 设置快捷方式的描述
			hres=pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPf);
			if(SUCCEEDED(hres))
			{
				WORD sz[MAX_PATH];
				// 字符串为ANSI 格式，须转化为Unicode格式 
				MultiByteToWideChar(CP_ACP,0,chDir,-1,(LPWSTR)sz,MAX_PATH);
				hres=pPf->Save((LPCOLESTR)sz,TRUE);   // 保存链接
				pPf->Release();
			}
			pShellLink->Release();
		}
	}
	else
	{
		//TRACEERROR(IDS_ShortCutErrorWhenCreate); //“创建菜单组和菜单项快捷方式失败",
	}
}

/////////////////////////////////
// 删除向开始菜单添加的快捷方式
/////////////////////////////////
void CStartMenu::DeleteFromStartMenu(char sMyPath[], char sMyShortCut[])
{
	//与向开始菜单添加快捷方式时一样，
	//首先取得快捷方式所在的目录路径，
	//方法与向开始菜单添加快捷方式相同
	char chDir[512];
	long result;
	HKEY hKey;         // 注册表打开键的句柄
	DWORD dwType;
	DWORD dwSize;


	// 打开注册表
	result=RegOpenKeyEx(	HKEY_CURRENT_USER,
							"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
							NULL,KEY_QUERY_VALUE,&hKey);

    if(result==ERROR_SUCCESS)
    {
		dwSize=512;

		// 查询Programs的键值并把查询到的
		//	结果保存在字符串“chDir”中
		//（因为Windows并不一定安装在c盘中，
		//所以要查询注册表）
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey);   // 关闭注册表
		strcat(chDir,"\\");
		strcat(chDir,sMyPath); //"\\My_shortcut_example"); 
	
		// 得到快捷方式的目录删除快捷方式的目录，
		// 首先确保文件存在
		CFileFind findfile;   
		char str[MAX_PATH];
		strcpy(str, sMyShortCut); 
		strcat(str,".lnk"); ////="Myshortcut.lnk";

		// 重新设置当前目录为快捷方式所在的目录
		/* 不再进行查找。因为查找会锁死目录，不允许删除了。
		   直接删除，目录存在与否，没有关系。有则删除，无则退出。
		SetCurrentDirectory(chDir); 

		// 在当前目录下搜索快捷方式文件
		if(!findfile.FindFile(str))  
		{
		//	return;
	    }
		*/
		
		char lpDelDir[512];    // 要删除的目录
		for(int i=0; i<512; i++)
		{
			lpDelDir[i]='\0';   // 初始化 
		}
		strcat(lpDelDir,chDir);
		SHFILEOPSTRUCT fileOp;  // 文件操作结构
		ZeroMemory( &fileOp, sizeof( fileOp)) ;  
		// 初始化
		fileOp.hwnd=NULL;   
		// 设置hwnd为NULL以便隐藏文件操作对话框
		fileOp.wFunc=FO_DELETE; // 删除操作
		fileOp.pFrom=lpDelDir;    // 要删除的目录
		fileOp.fFlags=FOF_NOCONFIRMATION | FOF_SILENT| FOF_ALLOWUNDO; // 删除文件到回收站
		
		if(SHFileOperation(&fileOp)==0)  // 删除
		{
			//TRACEPROGRESS(IDS_ShortCutDeletedFromStartMenu);//从开始菜单上删除快捷方式完成",
		}
		else
		{
			//TRACEERROR(IDS_ShortCutDeleteFailedFromStartMenu); //从开始菜单上删除快捷方式失败
		}
	}
	else
	{
		//TRACEERROR(IDS_ShortCut_RegisterTableFailedToOpen); //“打开注册表失败",
	}

}


////////////////////////////////////////////////
// 向桌面上添加快捷方式，因程序与向开始菜单上
//添加快捷方式基本一样，故不再给出注释。
////////////////////////////////////////////////
void CStartMenu::AddToDesktop(char sMyShortCut[],char sDiscription[])  
{
    char chDir[512];
	long result;
	HKEY hKey;     
	DWORD dwType;
	DWORD dwSize;
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
					"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
					NULL,KEY_QUERY_VALUE,&hKey);
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		//查询Desktop的键值并把查询到的结果
		//保存在字符串“chDir”中（因为Windows
		//并不一定安装在c盘中，所以要查询注册表）
		result=RegQueryValueEx(hKey,"Desktop",
								0,&dwType,
								(LPBYTE)chDir,&dwSize);

		result=RegCloseKey(hKey);   
		strcat(chDir,"\\");
		strcat(chDir,sMyShortCut);
		strcat(chDir,".lnk");

		HRESULT hres;
		IShellLink* pShellLink; 
		CoInitialize(NULL);
		hres=CoCreateInstance(CLSID_ShellLink,
								NULL,
								CLSCTX_INPROC_SERVER,
								IID_IShellLink,
								(LPVOID *)&pShellLink);
		if(SUCCEEDED(hres))
		{
			IPersistFile* pPf;
			TCHAR exeFullPath[MAX_PATH];
			GetModuleFileName(NULL,exeFullPath,MAX_PATH);
			pShellLink->SetPath(exeFullPath);
			pShellLink->SetDescription(sDiscription);
			hres=pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPf);
			if(SUCCEEDED(hres))
			{
				WORD sz[MAX_PATH];
				MultiByteToWideChar(CP_ACP,0,chDir,-1,(LPWSTR)sz,MAX_PATH);
				hres=pPf->Save((LPCOLESTR)sz,TRUE);
				pPf->Release();
			}
			pShellLink->Release();
		}
	}
	else
	{
		TRACEERROR(IDS_ShortCutCreationFailedOnTable); //“向桌面上添加快捷方式失败",
	}
}

/////////////////////////////////////
// 删除向桌面上添加的快捷方式，
//因程序与删除开始菜单上添加快捷方式
//基本一样，故不再给出注释。
void CStartMenu::DeleteFromDesktop(char sMyShortCut[])
{
	char chDir[512];
	long result;
	HKEY hKey;
	DWORD dwType;
	DWORD dwSize;
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
						NULL,KEY_QUERY_VALUE,&hKey);
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		result=RegQueryValueEx(hKey,"Desktop",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey); 
		CFileFind findfile;
		char str[MAX_PATH];
		strcpy(str,sMyShortCut);
		strcat(str,".lnk");

		SetCurrentDirectory(chDir); 
		if(!findfile.FindFile(str)) 
		{
			//TRACEERROR(IDS_ShortCutUnbaleToFind,str); //“找不到快捷方式文件 Myshortcut.lnk,
			return;
		}

		char sPath[200];
		strcpy(sPath,str);

		strcat(chDir,"\\");
		strcat(chDir,sMyShortCut);
		strcat(chDir,".lnk");


		char lpDelDir[512];    
		for(int i=0; i<512; i++)
		{
			lpDelDir[i]='\0';   
		}
		strcat(lpDelDir,chDir);

		SHFILEOPSTRUCT fileOp;  
		ZeroMemory( &fileOp, sizeof(fileOp)) ;
		fileOp.hwnd=NULL;     
		fileOp.wFunc=FO_DELETE; 
		fileOp.pFrom=lpDelDir; 
		fileOp.fFlags=FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO; // 删除文件到回收站
		if(SHFileOperation(&fileOp)==0) 
		{
			//TRACEERROR(IDS_ShortCutDeletedFromTable); //“从桌面上删除快捷方式完成",
		}
		else
		{
			//TRACEERROR(IDS_ShortCutDeletedFromTable); //“从桌面上删除快捷方式失败",NULL,
		}
	}

	else
	{
		//TRACEERROR(IDS_ShortCut_RegisterTableFailedToOpen); // 打开注册表失败
	}
}   

HRESULT CStartMenu::ResolvePath(char sDir[], char sShortCut[], LPSTR lpszPath) 
{	
	// 保存找到的\Windows\Start Menu\Programs路径
	char chDir[512];  

	long result;
	HKEY hKey;    // 注册表打开键的句柄
	DWORD dwType;
	DWORD dwSize;
	
	// 打开注册表 
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
						NULL,KEY_QUERY_VALUE,&hKey);	
	
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		// 查询Programs的键值并把查询到的结果
		// 保存在字符串“chDir”中（因为Windows
		// 并不一定安装在c盘中，所以要查询注册表）
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey); // 关闭注册表
		
		strcat(chDir,"\\"); 
		strcat(chDir,sDir);
		strcat(chDir,"\\");
		strcat(chDir,sShortCut) ;
		strcat(chDir,".lnk");
	}

	return ResolveIt(NULL,chDir,lpszPath);


}


HRESULT CStartMenu::ResolvePath(char sShortCut[], LPSTR lpszPath) 
{
    char chDir[512];
	long result;
	HKEY hKey;     
	DWORD dwType;
	DWORD dwSize;
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
					"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
					NULL,KEY_QUERY_VALUE,&hKey);
	
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		//查询Desktop的键值并把查询到的结果
		//保存在字符串“chDir”中（因为Windows
		//并不一定安装在c盘中，所以要查询注册表）
		result=RegQueryValueEx(hKey,"Desktop",
								0,&dwType,
								(LPBYTE)chDir,&dwSize);

		result=RegCloseKey(hKey);   
		strcat(chDir,"\\");
		strcat(chDir,sShortCut);
		strcat(chDir,".lnk");
	}

	return ResolveIt(NULL,chDir,lpszPath);

}

HRESULT CStartMenu::ResolveIt(HWND hwnd, LPCSTR sMyShortCut, LPSTR lpszPath) 
{ 
    HRESULT hres; 
    IShellLink* psl; 
    char szGotPath[MAX_PATH]; 
    char szDescription[MAX_PATH]; 
    WIN32_FIND_DATA wfd; 

    *lpszPath = 0; // assume failure 
 
    // Get a pointer to the IShellLink interface. 
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IShellLink, (LPVOID*)&psl); 
    if (SUCCEEDED(hres)) 
    { 
        IPersistFile* ppf; 
 
        // Get a pointer to the IPersistFile interface. 
        hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf); 
		
        if (SUCCEEDED(hres)) 
        { 
            WCHAR wsz[MAX_PATH]; 
 
            // Ensure that the string is Unicode. 
            MultiByteToWideChar(CP_ACP, 0, sMyShortCut, -1, wsz, MAX_PATH); 
 
            // TODO: Check return value from MultiByteWideChar to ensure success.
 
            // Load the shortcut. 
            hres = ppf->Load(wsz, STGM_READ); 
            
            if (SUCCEEDED(hres)) 
            { 
                // Resolve the link. 
                hres = psl->Resolve(hwnd, 0); 

                if (SUCCEEDED(hres)) 
                { 
                    // Get the path to the link target. 
                    hres = psl->GetPath(szGotPath, 
                                        MAX_PATH, 
                                        (WIN32_FIND_DATA*)&wfd, 
                                        SLGP_SHORTPATH); 

                    if (SUCCEEDED(hres)) 
                    { 
                        // Get the description of the target. 
                        hres = psl->GetDescription(szDescription, MAX_PATH); 

                        if (SUCCEEDED(hres)) 
                        {
                            //hres = strcpy(lpszPath, szGotPath);
							strcpy(lpszPath, szGotPath);

                            /*if (SUCCEEDED(hres))
                            {
                                // Handle success
                            }
                            else
							{
								   // application-defined function
                                //HandleErr(hres); 
							}*/
                             
                        }
                    }
                } 
            } 

            // Release the pointer to the IPersistFile interface. 
            ppf->Release(); 
        } 

        // Release the pointer to the IShellLink interface. 
        psl->Release(); 
    } 
    return hres; 
}
