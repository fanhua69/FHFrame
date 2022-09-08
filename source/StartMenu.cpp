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
// ��ʼ�˵���ӿ�ݷ�ʽ
void CStartMenu::AddToStartMenu(char sMyPath[], char sMyShortCut[],char sDiscription[]) 
{
	// �����ҵ���\Windows\Start Menu\Programs·��
	char chDir[512];  

	long result;
	HKEY hKey;    // ע���򿪼��ľ��
	DWORD dwType;
	DWORD dwSize;
	
	// ��ע��� 
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
						NULL,KEY_QUERY_VALUE,&hKey);	
	
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		// ��ѯPrograms�ļ�ֵ���Ѳ�ѯ���Ľ��
		// �������ַ�����chDir���У���ΪWindows
		// ����һ����װ��c���У�����Ҫ��ѯע���
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey); // �ر�ע���
		strcat(chDir,"\\"); 
		strcat(chDir,sMyPath);
		
		// �ڡ���ʼ���˵��ｨ�� �˵���
		CreateDirectory(chDir,NULL);        // ����Ŀ¼
		
		
		strcat(chDir,"\\");
		strcat(chDir,sMyShortCut) ;// "\\Myshortcut.lnk");        
		strcat(chDir,".lnk");
		
		//�ڡ���ʼ���˵��ｨ����Myshortcut���˵���
		HRESULT hres;
		IShellLink* pShellLink=NULL;	// �õ�IshellLink�ӿ�ָ��
		CoInitialize(NULL);				// ��ʼ��COM���
		hres=CoCreateInstance(	CLSID_ShellLink,
								NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,
								(LPVOID *)&pShellLink);
  		
		// ����һ��IID_IShellLinkʵ��
		if(SUCCEEDED(hres))
		{
			// IpersistFile�ӿ��ṩ������
			// ��һ������Ӵ��̼��ػ򱣴浽���̵ķ���
			IPersistFile*  pPf=NULL;
			// �õ���ǰ���еĿ�ִ�г����ȫ·����
			TCHAR exeFullPath[MAX_PATH];
			GetModuleFileName(NULL,exeFullPath,MAX_PATH);
			pShellLink->SetPath(exeFullPath);
			
			// ���ÿ�ݷ�ʽ��·��
			pShellLink->SetDescription(sDiscription); // ���ÿ�ݷ�ʽ������
			hres=pShellLink->QueryInterface(IID_IPersistFile,(LPVOID *)&pPf);
			if(SUCCEEDED(hres))
			{
				WORD sz[MAX_PATH];
				// �ַ���ΪANSI ��ʽ����ת��ΪUnicode��ʽ 
				MultiByteToWideChar(CP_ACP,0,chDir,-1,(LPWSTR)sz,MAX_PATH);
				hres=pPf->Save((LPCOLESTR)sz,TRUE);   // ��������
				pPf->Release();
			}
			pShellLink->Release();
		}
	}
	else
	{
		//TRACEERROR(IDS_ShortCutErrorWhenCreate); //�������˵���Ͳ˵����ݷ�ʽʧ��",
	}
}

/////////////////////////////////
// ɾ����ʼ�˵���ӵĿ�ݷ�ʽ
/////////////////////////////////
void CStartMenu::DeleteFromStartMenu(char sMyPath[], char sMyShortCut[])
{
	//����ʼ�˵���ӿ�ݷ�ʽʱһ����
	//����ȡ�ÿ�ݷ�ʽ���ڵ�Ŀ¼·����
	//��������ʼ�˵���ӿ�ݷ�ʽ��ͬ
	char chDir[512];
	long result;
	HKEY hKey;         // ע���򿪼��ľ��
	DWORD dwType;
	DWORD dwSize;


	// ��ע���
	result=RegOpenKeyEx(	HKEY_CURRENT_USER,
							"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
							NULL,KEY_QUERY_VALUE,&hKey);

    if(result==ERROR_SUCCESS)
    {
		dwSize=512;

		// ��ѯPrograms�ļ�ֵ���Ѳ�ѯ����
		//	����������ַ�����chDir����
		//����ΪWindows����һ����װ��c���У�
		//����Ҫ��ѯע���
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey);   // �ر�ע���
		strcat(chDir,"\\");
		strcat(chDir,sMyPath); //"\\My_shortcut_example"); 
	
		// �õ���ݷ�ʽ��Ŀ¼ɾ����ݷ�ʽ��Ŀ¼��
		// ����ȷ���ļ�����
		CFileFind findfile;   
		char str[MAX_PATH];
		strcpy(str, sMyShortCut); 
		strcat(str,".lnk"); ////="Myshortcut.lnk";

		// �������õ�ǰĿ¼Ϊ��ݷ�ʽ���ڵ�Ŀ¼
		/* ���ٽ��в��ҡ���Ϊ���һ�����Ŀ¼��������ɾ���ˡ�
		   ֱ��ɾ����Ŀ¼�������û�й�ϵ������ɾ���������˳���
		SetCurrentDirectory(chDir); 

		// �ڵ�ǰĿ¼��������ݷ�ʽ�ļ�
		if(!findfile.FindFile(str))  
		{
		//	return;
	    }
		*/
		
		char lpDelDir[512];    // Ҫɾ����Ŀ¼
		for(int i=0; i<512; i++)
		{
			lpDelDir[i]='\0';   // ��ʼ�� 
		}
		strcat(lpDelDir,chDir);
		SHFILEOPSTRUCT fileOp;  // �ļ������ṹ
		ZeroMemory( &fileOp, sizeof( fileOp)) ;  
		// ��ʼ��
		fileOp.hwnd=NULL;   
		// ����hwndΪNULL�Ա������ļ������Ի���
		fileOp.wFunc=FO_DELETE; // ɾ������
		fileOp.pFrom=lpDelDir;    // Ҫɾ����Ŀ¼
		fileOp.fFlags=FOF_NOCONFIRMATION | FOF_SILENT| FOF_ALLOWUNDO; // ɾ���ļ�������վ
		
		if(SHFileOperation(&fileOp)==0)  // ɾ��
		{
			//TRACEPROGRESS(IDS_ShortCutDeletedFromStartMenu);//�ӿ�ʼ�˵���ɾ����ݷ�ʽ���",
		}
		else
		{
			//TRACEERROR(IDS_ShortCutDeleteFailedFromStartMenu); //�ӿ�ʼ�˵���ɾ����ݷ�ʽʧ��
		}
	}
	else
	{
		//TRACEERROR(IDS_ShortCut_RegisterTableFailedToOpen); //����ע���ʧ��",
	}

}


////////////////////////////////////////////////
// ����������ӿ�ݷ�ʽ�����������ʼ�˵���
//��ӿ�ݷ�ʽ����һ�����ʲ��ٸ���ע�͡�
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
		//��ѯDesktop�ļ�ֵ���Ѳ�ѯ���Ľ��
		//�������ַ�����chDir���У���ΪWindows
		//����һ����װ��c���У�����Ҫ��ѯע���
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
		TRACEERROR(IDS_ShortCutCreationFailedOnTable); //������������ӿ�ݷ�ʽʧ��",
	}
}

/////////////////////////////////////
// ɾ������������ӵĿ�ݷ�ʽ��
//�������ɾ����ʼ�˵�����ӿ�ݷ�ʽ
//����һ�����ʲ��ٸ���ע�͡�
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
			//TRACEERROR(IDS_ShortCutUnbaleToFind,str); //���Ҳ�����ݷ�ʽ�ļ� Myshortcut.lnk,
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
		fileOp.fFlags=FOF_NOCONFIRMATION | FOF_SILENT | FOF_ALLOWUNDO; // ɾ���ļ�������վ
		if(SHFileOperation(&fileOp)==0) 
		{
			//TRACEERROR(IDS_ShortCutDeletedFromTable); //����������ɾ����ݷ�ʽ���",
		}
		else
		{
			//TRACEERROR(IDS_ShortCutDeletedFromTable); //����������ɾ����ݷ�ʽʧ��",NULL,
		}
	}

	else
	{
		//TRACEERROR(IDS_ShortCut_RegisterTableFailedToOpen); // ��ע���ʧ��
	}
}   

HRESULT CStartMenu::ResolvePath(char sDir[], char sShortCut[], LPSTR lpszPath) 
{	
	// �����ҵ���\Windows\Start Menu\Programs·��
	char chDir[512];  

	long result;
	HKEY hKey;    // ע���򿪼��ľ��
	DWORD dwType;
	DWORD dwSize;
	
	// ��ע��� 
	result=RegOpenKeyEx(HKEY_CURRENT_USER,
						"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders",
						NULL,KEY_QUERY_VALUE,&hKey);	
	
	if(result==ERROR_SUCCESS)
	{
		dwSize=512;
		// ��ѯPrograms�ļ�ֵ���Ѳ�ѯ���Ľ��
		// �������ַ�����chDir���У���ΪWindows
		// ����һ����װ��c���У�����Ҫ��ѯע���
		result=RegQueryValueEx(hKey,"Programs",0,&dwType,(LPBYTE)chDir,&dwSize);
		result=RegCloseKey(hKey); // �ر�ע���
		
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
		//��ѯDesktop�ļ�ֵ���Ѳ�ѯ���Ľ��
		//�������ַ�����chDir���У���ΪWindows
		//����һ����װ��c���У�����Ҫ��ѯע���
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
