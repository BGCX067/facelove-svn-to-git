#pragma once
#include <string>
#include <map>

using namespace std;
typedef struct tagMMAudioDev
{
	int nState;
	wchar_t  szID[200];
	wchar_t szEndPointName[200];
	wchar_t szDeviceName[200];
}MMAudioDev;
class KeyWrap
{
public:
	KeyWrap(void){};
public:
	virtual ~KeyWrap(void){};

public:
	static void QueryKey(HKEY hKey,wchar_t * valueList);
	static void QueryValue(HKEY hKey,wchar_t * valueList);
	static bool IsPlayKey(wchar_t * valueList);
	static int wcscasecmp(const wchar_t* cs,const wchar_t * ct);
	static int ReoveSpecialWord(wchar_t*  cs);

	static void StereoMix_W7(bool isMix);
	static void QueryInputKey(HKEY hKey,map<wstring,MMAudioDev>& m_devs);
	static void QueryInputValue(HKEY hKey,wchar_t* key,map<wstring,MMAudioDev>& m_devs);
	static void QueryInputValueSingle(HKEY hKey,wchar_t* key,wchar_t* subKey,wchar_t* value);

	static bool SetInputDevice(map<wstring,MMAudioDev>& m_devs,int nType=0,int nState=1,wchar_t* szShengKa=NULL);
	static bool WriteImputDeviceState(HKEY hKey,wchar_t* lpSubKey, DWORD dwVa);
	static bool WriteImputDeviceState(HKEY hKey,wchar_t* lpSubKey, BYTE* lpSubKeyValue);
	//static void Towlower(wchar_t* str);
	static void DateToBytes(BYTE *bytes);
	static void QueryKeyForQQ(HKEY hKey,wchar_t * valueList);
	static void QueryValueForQQ(HKEY hKey,wchar_t * valueList,int nType);
	static int IsHaveAdminControl();

};
