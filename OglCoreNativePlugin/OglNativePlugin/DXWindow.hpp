#pragma once

#include<Windows.h>
#include<iostream>
#include<vector>
#include"Debug.h"

using namespace dxlib;
using namespace std;


struct ALLMONITORINFO
{
	HMONITOR hMonitor;
	RECT     rect;
	bool     isPrimary;
};

class DXWindow
{
public:
	DXWindow()
	{
		_hWnd = NULL;
		szTitle = L"显示窗口";
		szWindowClass = L"oglWindow";
	}

	~DXWindow()
	{
		//Close();
	}

	HINSTANCE _hInst;					// 当前实例
	TCHAR* szTitle;					// 标题栏文本
	TCHAR* szWindowClass;			// 主窗口类名
	BOOL   isFullScreen;
							//生成的窗口句柄
	HWND _hWnd;
	int width;
	int height;

	LONG startx = 0, starty = 0;

	///-------------------------------------------------------------------------------------------------
	/// <summary> 创建一个窗口. </summary>
	///
	/// <remarks> Xian.dai, 2017/3/23. </remarks>
	///
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	BOOL Create(WNDPROC proc,HWND parentWindow)
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
		wcex.style = CS_HREDRAW | CS_VREDRAW| CS_OWNDC | CS_DBLCLKS;

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
										   //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

										   //注册窗口类,这个是必须的，要不然下面的CreateWindow函数总返回NULL
		//ATOM atom = RegisterClassEx(&wcex);
		RegisterClassEx(&wcex);

		//所有的显示器信息。
		vector<ALLMONITORINFO> mInfo;
		//获取显示器信息。
		mInfo.clear();
		//get number of monitors
		mInfo.reserve(GetSystemMetrics(SM_CMONITORS));

		EnumDisplayMonitors(NULL, NULL, this->MonitorEnumProc, reinterpret_cast<LPARAM>(&mInfo));

		//cout << "the number of monitors:" << mInfo.size() << endl;
		Debug::GetInst()->Log("the number of monitors:%d.\r\n", mInfo.size());
		


		if (mInfo.size() == 1)
		{
			RECT rect = mInfo[0].rect;
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
			startx = rect.left;
			starty = rect.top;
		}
		else
		{
			for (int i = 0; i < mInfo.size(); i++)
			{
				Debug::GetInst()->Log("mInfo[%d].isPrimary:%d.\r\n",i, mInfo[i].isPrimary);
				//获取特定显示器的信息，最重要的是分辨率，起始坐标等。
				if (!mInfo[i].isPrimary)
				{
					RECT rect = mInfo[i].rect;
					width = rect.right - rect.left;
					height = rect.bottom - rect.top;
					startx = rect.left;
					starty = rect.top;
					break;
				}
			}
		}

		//width = 1280;
		//height = 720;
		//获取以像素为单位的屏幕尺寸。
		//打印得出确实得到了长宽。
		//cout << "width:" << width << "  height:" << height << endl;
		Debug::GetInst()->Log("width:%d,height:%d.startx:%d,starty:%d.\r\n",width,height,startx,starty);
		//创建窗口时，如果想全屏显示，不能将宽和高设定为width和height,可以设置成比它小的任意数。
		_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
			startx, starty, width/2, height/2, NULL, NULL, _hInst, NULL);

		if (!_hWnd)
		{
			Debug::GetInst()->Log("创建窗口失败：%d.\r\n",GetLastError());
			return FALSE;
		}
		ToggleFullScreen();

		ShowWindow(_hWnd, SW_SHOW);
		//ShowWindow(_hWnd, SW_MAXIMIZE);
		UpdateWindow(_hWnd);

		//SetWindowPos(_hWnd, HWND_TOPMOST, startx, starty, width, height, SWP_NOMOVE | SWP_NOSIZE); //SWP_NOMOVE 保留当前位置(忽略 x 和 y 参数）。
		//LONG oldlStyle = GetWindowLong(_hWnd, GWL_STYLE);
		//SetWindowPos(_hWnd, HWND_TOPMOST, startx, starty, width, height, oldlStyle);

		return TRUE;
	}

	void Close()
	{
		if (_hWnd)
		{
			CloseWindow(_hWnd);
			//CloseWindow destroy window
			BOOL result = DestroyWindow(_hWnd);
			//DWORD error = GetLastError();
			Debug::GetInst()->Log("Error:%d.\r\n", result);
		}
		_hWnd = NULL;

	}

	void ToggleFullScreen()
	{
		ToggleFullScreen(width, height, startx, starty);
	}

	void ToggleFullScreen(int width, int height, int startx, int starty)
	{
		if (_hWnd == NULL)
		{
			cout << "TXWindow.ChangeToFullScreenInRun():错误，_hWnd为空！ \r\n";
			return;
		}

		//从全屏退到非全屏有问题。
		if (isFullScreen)
		{

			SetWindowLong(_hWnd, GWL_STYLE, GetWindowLong(_hWnd, GWL_STYLE) | WS_OVERLAPPEDWINDOW);

			//GWL_EXSTYLE  extended style扩展格式。WS_EX_WINDOWEDGE  使得窗口显示，并不覆盖下面的任务栏。
			//SetWindowLong(_hWnd, GWL_EXSTYLE, GetWindowLong(_hWnd, GWL_EXSTYLE) | WS_EX_WINDOWEDGE);

			//在多显示器上不知道会不会有问题，是否会在主屏上显示
			//SetWindowPos(_hWnd, NULL, startx, starty, width, height, SWP_SHOWWINDOW);// SWP_FRAMECHANGED);
		}
		else
		{
			Debug::GetInst()->Log("设置成全屏。\r\n");
			LONG curWinStyle = GetWindowLong(_hWnd, GWL_STYLE);
			curWinStyle = curWinStyle & ~WS_OVERLAPPEDWINDOW;
			SetWindowLong(_hWnd, GWL_STYLE, curWinStyle | WS_POPUP);
			MoveWindow(_hWnd, startx, starty, width, height, TRUE);
		}
		//isFullScreen = !isFullScreen;
	}

	//RECT GetMonitorRECT(HMONITOR hMonitor)
	//{
	//	for (int i = 0; i < mInfo.size(); i++)
	//	{
	//		if (mInfo[i].hMonitor == hMonitor)
	//		{
	//			return mInfo[i].rect;
	//		}
	//	}
	//	return RECT();
	//}
private:

	///-------------------------------------------------------------------------------------------------
	/// <summary> 一个WndProc消息处理函数模板. </summary>
	/// <returns> A CALLBACK. </returns>
	///-------------------------------------------------------------------------------------------------
	static	LRESULT CALLBACK  MyWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		//int wmId, wmEvent;
		//dxlib::Debug::Log("WndProc():进入消息处理函数 message = %d \r\n", message);
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