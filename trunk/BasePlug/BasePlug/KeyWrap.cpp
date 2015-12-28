#include "StdAfx.h"
#include "KeyWrap.h"
#include "comutil.h"

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void KeyWrap::QueryKey(HKEY hKey,wchar_t * valueList) 
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				if (IsPlayKey(achKey))
				{
					HKEY hKey;					
					wchar_t szKey[100];
					wcscpy(szKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\");				
					wcscat(szKey,achKey);
					if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_READ,&hKey) == ERROR_SUCCESS)
					{
						QueryValue(hKey,valueList);
					}
					RegCloseKey(hKey);			
				}
				_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
			}

		}
	}
}

void KeyWrap::QueryValue(HKEY hKey,wchar_t * valueList)
{
	TCHAR    achKey[MAX_KEY_LENGTH];		 // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time
	DWORD i, retCode; 
	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD dwValueSize = MAX_VALUE_NAME; 
	TCHAR chData[MAX_VALUE_NAME];
	DWORD dwDataSize = MAX_VALUE_NAME;//这里要重新赋值

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the key values. 

	if (cValues) 
	{
		printf( "\nNumber of values: %d\n", cValues);

		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ 
			dwValueSize = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			dwDataSize =MAX_VALUE_NAME;
			chData[0]='\0';


			BOOL bRet;
			retCode = RegEnumValue(hKey, i, 
				achValue, 
				&dwValueSize, 
				NULL, 
				NULL,
				(LPBYTE)chData, 
				&dwDataSize);

			if (retCode == ERROR_SUCCESS && dwValueSize ==0 ) 
			{ 
				ReoveSpecialWord(&chData[0]);
				wcscat(valueList,chData);
				wcscat(valueList,_T("*"));
				_tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
			} 
		}
	}

}

bool KeyWrap::IsPlayKey(wchar_t * valueList) 
{

	if (!wcscasecmp(valueList,_T("mplayer2.exe")))
	{
		return true;
	}	
	else if (!wcscasecmp(valueList,_T("KuGou.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("TTPlayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("winamp.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("foobar2000.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("QQMusic.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("KwMusic.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("DuoMi.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("StormPlayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("QQPlayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("XMP.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("KMPlayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("wmplayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("BaiduPlayer.exe")))
	{
		return true;
	}
	else if (!wcscasecmp(valueList,_T("QvodPlayer.exe")))
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

int KeyWrap::wcscasecmp(const wchar_t* cs,const wchar_t * ct)
{
	while (towlower(*cs) == towlower(*ct))
	{
		if (*cs == 0)
			return 0;
		cs++;
		ct++;
	}
	return towlower(*cs) - towlower(*ct);
}

int KeyWrap::ReoveSpecialWord(wchar_t*  cs)
{
	if (*cs == '\"')
	{
		wcscpy(cs,cs+1);
	}
	if (cs[wcslen(cs)-1]=='\"')
	{
		cs[wcslen(cs)-1]='\0';

	}
	return 0;	
}
void KeyWrap::StereoMix_W7(bool isMix)
{
	HKEY hKey;
	map<wstring,MMAudioDev> m_devs;


	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture"),

		0,
		KEY_READ,
		&hKey) == ERROR_SUCCESS
		)
	{
		KeyWrap::QueryInputKey(hKey,m_devs);

	}

	RegCloseKey(hKey);
	if (isMix)
	{
		KeyWrap::SetInputDevice(m_devs,1,1);
	} 
	else
	{
		KeyWrap::SetInputDevice(m_devs,1,0);
	}
	
	
}
void KeyWrap::QueryInputKey(HKEY hKey,map<wstring,MMAudioDev>& m_devs) 
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				
				HKEY hKey;					
				wchar_t szKey[300];
				wcscpy(szKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture\\");				
				wcscat(szKey,achKey);
				HKEY hKey1;
				if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_READ,&hKey1) == ERROR_SUCCESS)
				{
					QueryInputValue(hKey1,achKey,m_devs);
				}
				RegCloseKey(hKey1);			
				
				_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
			}

		}
	}
}

void KeyWrap::QueryInputValue(HKEY hKey,wchar_t* key,map<wstring,MMAudioDev>& m_devs)
{
	TCHAR    achKey[MAX_KEY_LENGTH];		 // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time
	DWORD i, retCode; 
	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD dwValueSize = MAX_VALUE_NAME; 
	TCHAR chData[MAX_VALUE_NAME];
	DWORD dwDataSize = MAX_VALUE_NAME;//这里要重新赋值

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the key values. 

	if (cValues) 
	{
		printf( "\nNumber of values: %d\n", cValues);
		MMAudioDev dev;
		wcscpy(dev.szID,key);		
		
		/*for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ */
			dwValueSize = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			dwDataSize =MAX_VALUE_NAME;
			chData[0]='\0';


			BOOL bRet;
			retCode = RegEnumValue(hKey, 0, 
				achValue, 
				&dwValueSize, 
				NULL, 
				NULL,
				(LPBYTE)chData, 
				&dwDataSize);

			if (retCode == ERROR_SUCCESS  ) 
			{ 
				ReoveSpecialWord(&chData[0]);
				
				dev.nState=chData[0];
				
				wchar_t szKey[200];
				wcscpy(szKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture\\");				
				wcscat(szKey,key);
				wcscat(szKey,L"\\Properties");//\\{a45c254e-df1c-4efd-8020-67d146a850e0},2
				HKEY hKey2;
				if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_READ,&hKey2) == ERROR_SUCCESS)
				{
					QueryInputValueSingle(hKey2,achKey,_T("{a45c254e-df1c-4efd-8020-67d146a850e0},2"),dev.szEndPointName);
					QueryInputValueSingle(hKey2,achKey,_T("{b3f8fa53-0004-438e-9003-51a46e139bfc},6"),dev.szDeviceName);
				}
				RegCloseKey(hKey2);
			

				/*wcscat(valueList,chData);
				wcscat(valueList,_T("*"));*/
				_tprintf(TEXT("(%d) %s\n"), 1, achValue); 
			} 
			m_devs.insert(map<wstring,MMAudioDev>::value_type(dev.szID,dev));

		/*}*/
	}

}


void KeyWrap::QueryInputValueSingle(HKEY hKey,wchar_t* key,wchar_t* subKey,wchar_t* value)
{
	TCHAR    achKey[MAX_KEY_LENGTH];		 // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time
	DWORD i, retCode; 
	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD dwValueSize = MAX_VALUE_NAME; 
	TCHAR chData[MAX_VALUE_NAME];
	DWORD dwDataSize = MAX_VALUE_NAME;//这里要重新赋值

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the key values. 

	if (cValues) 
	{
		printf( "\nNumber of values: %d\n", cValues);
		
		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ 
			dwValueSize = MAX_VALUE_NAME; 
			achValue[0] = '\0'; 
			dwDataSize =MAX_VALUE_NAME;
			chData[0]='\0';


			BOOL bRet;
			retCode = RegEnumValue(hKey, i, 
				achValue, 
				&dwValueSize, 
				NULL, 
				NULL,
				(LPBYTE)chData, 
				&dwDataSize);

			if (retCode == ERROR_SUCCESS  ) 
			{ 
				
				if (wcscmp(achValue,subKey)==NULL)
				{
					wcscpy(value,chData);
					break;
				}
				_tprintf(TEXT("(%d) %s\n"), i+1, achValue); 

			} 
		}
	}

}

//设置默认录音输入:0:麦克风 1:混音输入 2：LineIn 参数2：声卡包含名
//nState:1使能 0禁用

bool  KeyWrap::SetInputDevice(map<wstring,MMAudioDev>& m_devs,int nType,int nState,wchar_t* szShengKa)
{
	bool bFind=false;
	/*if(szShengKa!=NULL)
		Towlower(szShengKa);*/
	
	MMAudioDev dev;
	
	for (map<wstring,MMAudioDev>::iterator it=m_devs.begin();it!=m_devs.end();it++)
	{
		dev=it->second;
		
		//Towlower(dev.szEndPointName);
		//Towlower(dev.szDeviceName);

	    if (szShengKa ==NULL || wcscmp(dev.szDeviceName,szShengKa)!=NULL )
	    {
			if(nType==0)
			{
				if( wcsstr(dev.szEndPointName,_T("mic"))!=NULL || wcsstr(dev.szEndPointName,_T("麦克风"))!=NULL) 
					bFind=true;
			}
			else if(nType==1)
			{
				if(wcsstr(dev.szEndPointName,_T("mix"))!=NULL||wcsstr(dev.szEndPointName,_T("sum"))!=NULL \
					|| wcsstr(dev.szEndPointName,_T("混"))!=NULL||wcsstr(dev.szEndPointName,_T("立体"))!=NULL\
					|| wcsstr(dev.szEndPointName,_T("stereo"))!=NULL||wcsstr(dev.szEndPointName,_T("总和"))!=NULL)

					bFind=true;
				
			}
			if(nType==2)
			{
				if(wcsstr(dev.szEndPointName,_T("line"))!=NULL||wcsstr(dev.szEndPointName,_T("线路"))!=NULL) 
					bFind=true;

				
			}
			if(bFind)
				break;
	    }
		
	}

	if(!bFind)
		return false;
	wchar_t szKey[200];
	wcscpy(szKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\MMDevices\\Audio\\Capture\\");				
	wcscat(szKey,dev.szID);
	//wcscat(szKey,L"\\Properties");//\\{a45c254e-df1c-4efd-8020-67d146a850e0},2
	HKEY hKey;
	int i= RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_WRITE,&hKey);
	if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_SET_VALUE,&hKey) == ERROR_SUCCESS)
	{
		//写状态
		DWORD dwState;
		if (nState==1)
		{
			dwState =0x00000001;
		}
		else 
		{
			dwState=0x10000001;//停用  0x00000001启用
		}
		

		BOOL bOK=WriteImputDeviceState(hKey,_T("DeviceState"),dwState);
		

		BYTE b[16];
		DateToBytes(b);
		//写Role:0
	   /* WriteImputDeviceState(hKey,_T("Role:0"),b);
	    WriteImputDeviceState(hKey,_T("Role:1"),b);
		WriteImputDeviceState(hKey,_T("Role:2"),b);*/
		
		if(!bOK)
			return false;
		return true;
		

	}
	// i=GetLastError();
	RegCloseKey(hKey);




}

bool KeyWrap::WriteImputDeviceState(HKEY hKey,wchar_t* lpSubKey, DWORD dwVa)
{
	long lReturn=RegSetValueEx(hKey,lpSubKey,0L,REG_DWORD,(const BYTE *) &dwVa,sizeof(DWORD));

	/*char temp[20];	
	sprintf_s(temp,"RegSetValueEx:%ld\0",lReturn);
	_bstr_t strStr = temp;
	LPCSTR lpctStrName = strStr;
	::MessageBox(NULL,(LPCWSTR)lpctStrName,_T("提示信息"),MB_OK   |  MB_ICONINFORMATION);*/

	if(lReturn==ERROR_SUCCESS)
		return TRUE;

	return FALSE;



}

bool KeyWrap::WriteImputDeviceState(HKEY hKey,wchar_t* lpSubKey, BYTE* lpSubKeyValue)
{
	long lReturn=RegSetValueEx(hKey,lpSubKey,0L,REG_BINARY,(const BYTE *) lpSubKey,strlen((const char*)lpSubKeyValue)+1);

	if(lReturn==ERROR_SUCCESS)
		return TRUE;

	return FALSE;

}

 void  KeyWrap::DateToBytes(BYTE *bytes)
{
	SYSTEMTIME date;
	::GetLocalTime(&date);
	bytes[1] = (BYTE)(date.wYear / 0x100);
	bytes[0] = (BYTE)(date.wYear - bytes[1] * 0x100);
	bytes[2] = (BYTE)date.wMonth;
	bytes[3] = 0; bytes[5] = 0; bytes[7] = 0; bytes[9] = 0; bytes[11] = 0; bytes[13] = 0;
	bytes[15] = 0;
	bytes[4] = (BYTE)date.wDayOfWeek;
	bytes[6] = (BYTE)date.wDay;
	bytes[8] = (BYTE)date.wHour;
	bytes[10] = (BYTE)date.wMinute;
	bytes[12] = (BYTE)date.wSecond;
	bytes[14] = (BYTE)date.wMilliseconds;
}

 void KeyWrap::QueryKeyForQQ(HKEY hKey,wchar_t * valueList) 
 {
	 TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	 DWORD    cbName;                   // size of name string 
	 TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	 DWORD    cchClassName = MAX_PATH;  // size of class string 
	 DWORD    cSubKeys=0;               // number of subkeys 
	 DWORD    cbMaxSubKey;              // longest subkey size 
	 DWORD    cchMaxClass;              // longest class string 
	 DWORD    cValues;              // number of values for key 
	 DWORD    cchMaxValue;          // longest value name 
	 DWORD    cbMaxValueData;       // longest value data 
	 DWORD    cbSecurityDescriptor; // size of security descriptor 
	 FILETIME ftLastWriteTime;      // last write time 

	 DWORD i, retCode; 

	 TCHAR  achValue[MAX_VALUE_NAME]; 
	 DWORD cchValue = MAX_VALUE_NAME; 

	 // Get the class name and the value count. 
	 retCode = RegQueryInfoKey(
		 hKey,                    // key handle 
		 achClass,                // buffer for class name 
		 &cchClassName,           // size of class string 
		 NULL,                    // reserved 
		 &cSubKeys,               // number of subkeys 
		 &cbMaxSubKey,            // longest subkey size 
		 &cchMaxClass,            // longest class string 
		 &cValues,                // number of values for this key 
		 &cchMaxValue,            // longest value name 
		 &cbMaxValueData,         // longest value data 
		 &cbSecurityDescriptor,   // security descriptor 
		 &ftLastWriteTime);       // last write time 

	 // Enumerate the subkeys, until RegEnumKeyEx fails.

	 if (cSubKeys)
	 {
		 for (i=0; i<cSubKeys; i++) 
		 { 
			 cbName = MAX_KEY_LENGTH;
			 retCode = RegEnumKeyEx(hKey, i,
				 achKey, 
				 &cbName, 
				 NULL, 
				 NULL, 
				 NULL, 
				 &ftLastWriteTime); 
			 if (retCode == ERROR_SUCCESS) 
			 {
				 if (!wcscasecmp(achKey,_T("QQMusic")))
				 {
					 HKEY hKey;					
					 wchar_t szKey[100];
					 wcscpy(szKey,L"SOFTWARE\\Tencent\\");				
					 wcscat(szKey,achKey);				
					 if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_READ,&hKey) == ERROR_SUCCESS)
					 {
						 QueryValueForQQ(hKey,valueList,1);
					 }
					 RegCloseKey(hKey);	

				 }

				 if (!wcscasecmp(achKey,_T("QQPlayer")))
				 {
					 HKEY hKey;					
					 wchar_t szKey[100];
					 wcscpy(szKey,L"SOFTWARE\\Tencent\\");				
					 wcscat(szKey,achKey);
					 if( RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_READ,&hKey) == ERROR_SUCCESS)
					 {
						 QueryValueForQQ(hKey,valueList,0);
					 }
					 RegCloseKey(hKey);	

				 }			
				 _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
			 }

		 }
	 }
 }

 void KeyWrap::QueryValueForQQ(HKEY hKey,wchar_t * valueList,int nType)
 {
	 TCHAR    achKey[MAX_KEY_LENGTH];		 // buffer for subkey name
	 DWORD    cbName;                   // size of name string 
	 TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	 DWORD    cchClassName = MAX_PATH;  // size of class string 
	 DWORD    cSubKeys=0;               // number of subkeys 
	 DWORD    cbMaxSubKey;              // longest subkey size 
	 DWORD    cchMaxClass;              // longest class string 
	 DWORD    cValues;              // number of values for key 
	 DWORD    cchMaxValue;          // longest value name 
	 DWORD    cbMaxValueData;       // longest value data 
	 DWORD    cbSecurityDescriptor; // size of security descriptor 
	 FILETIME ftLastWriteTime;      // last write time
	 DWORD i, retCode; 
	 TCHAR  achValue[MAX_VALUE_NAME]; 
	 DWORD dwValueSize = MAX_VALUE_NAME; 
	 TCHAR chData[MAX_VALUE_NAME];
	 DWORD dwDataSize = MAX_VALUE_NAME;//这里要重新赋值

	 // Get the class name and the value count. 
	 retCode = RegQueryInfoKey(
		 hKey,                    // key handle 
		 achClass,                // buffer for class name 
		 &cchClassName,           // size of class string 
		 NULL,                    // reserved 
		 &cSubKeys,               // number of subkeys 
		 &cbMaxSubKey,            // longest subkey size 
		 &cchMaxClass,            // longest class string 
		 &cValues,                // number of values for this key 
		 &cchMaxValue,            // longest value name 
		 &cbMaxValueData,         // longest value data 
		 &cbSecurityDescriptor,   // security descriptor 
		 &ftLastWriteTime);       // last write time 

	 // Enumerate the key values. 

	 if (cValues) 
	 {
		 printf( "\nNumber of values: %d\n", cValues);

		 for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		 { 
			 dwValueSize = MAX_VALUE_NAME; 
			 achValue[0] = '\0'; 
			 dwDataSize =MAX_VALUE_NAME;
			 chData[0]='\0';


			 BOOL bRet;
			 retCode = RegEnumValue(hKey, i, 
				 achValue, 
				 &dwValueSize, 
				 NULL, 
				 NULL,
				 (LPBYTE)chData, 
				 &dwDataSize);

			 if (retCode == ERROR_SUCCESS ) 
			 { 
				 if (nType==1 && (!wcscasecmp(achValue,_T("Install"))))
				 {
					 ReoveSpecialWord(&chData[0]);
					 wcscat(valueList,chData);
					 wcscat(valueList,_T("\QQMusic.exe"));
					 wcscat(valueList,_T("*"));
					 _tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
					 break;
				 } 
				 else if(nType ==0&& (!wcscasecmp(achValue,_T("ExePath"))))
				 {
					 ReoveSpecialWord(&chData[0]);
					 wcscat(valueList,chData);
					 wcscat(valueList,_T("*"));
					 _tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
					 break;

				 }

			 } 			
		 }
	 }
 }

 int KeyWrap::IsHaveAdminControl()
 {
	 wchar_t szKey[200];
	 wcscpy(szKey,L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\");	
	 HKEY hKey;
	 int i= RegOpenKeyEx( HKEY_LOCAL_MACHINE,szKey,0,KEY_SET_VALUE,&hKey);
	 long lReturn=RegSetValueEx(hKey,L"InstallerLocation",0L,REG_MULTI_SZ,(const BYTE *)(L"C:\\Windows\\system32\\"),strlen("C:\\Windows\\system32\\")*2);
	 return lReturn;	 
	 return i;
 }
