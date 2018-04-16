#pragma once

#include<Windows.h>
#include<iostream>
#include<vector>

using namespace std;

struct ALLMONITORINFO
{
	HMONITOR hMonitor;
	RECT     rect;
	bool     isPrimary;
};

class oglWindow
{
public:
	oglWindow()
	{
		_hWnd = NULL;
		szTitle = L"显示窗口";
		szWindowClass = L"oglWindow";
	}

	~oglWindow()
	{
		//Close();
	}

	HINSTANCE _hInst;					// 当前实例
	TCHAR* szTitle;					// 标题栏文本
	TCHAR* szWindowClass;			// 主窗口类名
									//生成的窗口句柄
	HWND _hWnd;
	int width;
	int height;

	///-------------------------------------------------------------------------------------------------
	/// <summary> 创建一个窗口. </summary>
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	BOOL Create(int w,int h,WNDPROC proc= NULL)
	{
		if (_hWnd != NULL)//如果已经创建了一个窗口那么不再重复创建
		{
			return FALSE;
		}

		//返回模块的句柄，比如是.exe模块的句柄或者.dll模块的句柄。
		//如果传入参数为NULL，则返回调用该窗口的.exe模块句柄。
		//也可以像这样调用获取模块句柄GetModuleHandle("mydll.dll")
		_hInst = GetModuleHandle(NULL);

		WNDCLASSEX wcex;
		memset(&wcex, 0, sizeof(wcex));

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_OWNDC;

											 //设置其回调函数
		if (proc != NULL)
			wcex.lpfnWndProc = proc;
		else
			wcex.lpfnWndProc = this->MyWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = _hInst;

		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(107));
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(108));
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN01);//不要菜单栏
		wcex.lpszClassName = szWindowClass;//主窗口类名

		RegisterClassEx(&wcex);

		int startx = 0, starty = 0;


		//if (mInfo.size() == 1)
		//{
		//	RECT rect = mInfo[0].rect;
		//	width = rect.right - rect.left;
		//	height = rect.bottom - rect.top;
		//	startx = rect.left;
		//	starty = rect.top;
		//}
		//else
		//{
		//	for (int i = 0; i < mInfo.size(); i++)
		//	{
		//		//获取特定显示器的信息，最重要的是分辨率，起始坐标等。
		//		if (!mInfo[i].isPrimary)
		//		{
		//			RECT rect = mInfo[0].rect;
		//			width = rect.right - rect.left;
		//			height = rect.bottom - rect.top;
		//			startx = rect.left;
		//			starty = rect.top;
		//			break;
		//		}
		//	}
		//}

		width = w;
		height = h;
		//获取以像素为单位的屏幕尺寸。
		//打印得出确实得到了长宽。
		//cout << "width:" << width << "  height:" << height << endl;
		_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			startx, starty, width, height, NULL, NULL, _hInst, NULL);

		if (!_hWnd)
		{
			//Debug::GetInst()->Log("创建窗口失败：%d", GetLastError());
			return FALSE;
		}
		//ToggleFullScreen();

		ShowWindow(_hWnd, SW_SHOW);
		//ShowWindow(_hWnd, SW_MAXIMIZE);
		UpdateWindow(_hWnd);

		return TRUE;
	}

	void Close()
	{
		if (_hWnd)
		{
			//CloseWindow(_hWnd);
			//CloseWindow destroy window
			BOOL result = DestroyWindow(_hWnd);
			DWORD error = GetLastError();
			//Debug::GetInst()->Log("Error:%d.", error);
		}
		_hWnd = NULL;

	}

private:

	///-------------------------------------------------------------------------------------------------
	/// <summary> 一个WndProc消息处理函数模板. </summary>
	/// <returns> A CALLBACK. </returns>
	///-------------------------------------------------------------------------------------------------
	static	LRESULT CALLBACK  MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_COMMAND:
			break;
		case WM_PAINT:
			//hdc = BeginPaint(hWnd, &ps);
			//// TODO:  在此添加任意绘图代码...
			//EndPaint(hWnd, &ps);
			break;

		case WM_KEYDOWN:
			//处理键盘上某一键按下的消息

			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}

	///----------------------------------------------------------------------------------------
	///获取所有显示器的信息，并把它们存储在vector数组内。
	///该方法会作为EnumDisplayMonitors的回调函数，该vector数组也会以此形式传出去。
	///
	///----------------------------------------------------------------------------------------
	static BOOL CALLBACK MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData)
	{
		vector<ALLMONITORINFO>& infoArray = *reinterpret_cast<vector<ALLMONITORINFO>* >(dwData);

		ALLMONITORINFO monitorInfo;
		monitorInfo.hMonitor = hMonitor;
		monitorInfo.rect = *lprcMonitor;

		HMONITOR priMonitor = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
		if (priMonitor == hMonitor)
			monitorInfo.isPrimary = true;
		else
			monitorInfo.isPrimary = false;

		infoArray.push_back(monitorInfo);
		return TRUE;
	}

};