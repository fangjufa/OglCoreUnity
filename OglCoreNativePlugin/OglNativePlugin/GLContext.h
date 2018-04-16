#pragma once

#include <gl/GL.h>
#include <windows.h>

#include "Debug.h"

class GLContext
{
public:

	int _format;

	/// <summary> 窗口句柄. </summary>
	HWND _hWnd;

	/// <summary> 绘制设备上下文. </summary>
	HDC _hDC;

	/// <summary> Opengl上下文. </summary>
	HGLRC _hRC;


	BOOL isShared;

	GLContext()
	{
		_format = 0;
		_hWnd = 0;
		_hDC = 0;
		_hRC = 0;
		isShared = false;
	}


	~GLContext()
	{
	
	}


	///-------------------------------------------------------------------------------------------------
	/// <summary> Setups. </summary>
	///
	/// <remarks> Xian.dai, 2017/3/23. </remarks>
	///
	/// <param name="hWnd"> Handle of the window. </param>
	/// <param name="hDC">  The device-context. </param>
	///
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	bool setup(HWND hWnd, HDC hDC)
	{
		_hWnd = hWnd;
		_hDC = hDC;
		unsigned PixelFormat;
		PIXELFORMATDESCRIPTOR pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),//nSize
			1,//nVersion
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,//dwFlags
			PFD_TYPE_RGBA,//iPixelType ,不采用索引，索引效率比较低，虽然占用的内存小
			32,//cColorBits
			0,//cRedBits;
			0,//cRedShift;
			0,//cGreenBits;
			0,//cGreenShift;
			0,//cBlueBits;
			0,//cBlueShift;
			0,//cAlphaBits;
			0,//cAlphaShift;
			0,//cAccumBits;
			0,//cAccumRedBits;
			0,//cAccumGreenBits;
			0,//cAccumBlueBits;
			0,//cAccumAlphaBits;
			24,//cDepthBits;
			8,//cStencilBits;模板缓冲
			0,//cAuxBuffers;累计缓存
			PFD_MAIN_PLANE,//iLayerType;
			0,//bReserved;
			0,//dwLayerMask;
			0,//dwVisibleMask;
			0,//dwDamageMask;

		};
	
		if (_format == 0)
		{
			PixelFormat = ChoosePixelFormat(_hDC, &pfd);
		}
		else
		{
			PixelFormat = _format;
		}
		if (!SetPixelFormat(_hDC, PixelFormat, &pfd))
		{
			return false;
		}
		_hRC = wglCreateContext(_hDC);
		if (!wglMakeCurrent(_hDC, _hRC))
		{
			return false;
		}
		return true;

	}

	void makeCurrent()
	{
		if (_hRC != NULL && _hDC != NULL)
		{
			BOOL re = wglMakeCurrent(_hDC, _hRC);
			dxlib::Debug::Log("GLContext.makeCurrent():设置绘图上下文，结果 = %d\r\n", re);
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary> Share lists. </summary>
	///
	/// <remarks> Xian.dai, 2017/3/27. </remarks>
	///
	/// <param name="u3dhRC"> The 3dh rectangle. </param>
	///
	/// <returns>
	/// true if it succeeds, false if it fails.
	/// </returns>
	///-------------------------------------------------------------------------------------------------
	BOOL shareLists(HGLRC u3dhRC)
	{
		if (_hRC != NULL && u3dhRC != NULL && isShared == FALSE)
		{
			dxlib::Debug::Log("shareLists():执行wglShareLists(u3dhRC, _hRC) ！\r\n", isShared);
			isShared = wglShareLists(u3dhRC, _hRC);
			dxlib::Debug::Log("shareLists():当前glContext.isShared = %d ！\r\n", isShared);

			if (isShared == FALSE)
			{
				dxlib::Debug::Log("shareLists():执行wglShareLists(_hRC,u3dhRC) ！\r\n", isShared);
				isShared = wglShareLists(_hRC, u3dhRC);
				dxlib::Debug::Log("shareLists():当前glContext.isShared = %d ！\r\n", isShared);
			}

			return isShared;
		}
		return FALSE;
	}

	void shutdown()
	{
		if (_hRC != NULL)
		{
			//wglMakeCurrent(NULL, NULL);//在这个程序里并不需要这个
			wglDeleteContext(_hRC);
			_hRC = NULL;
		}
		if (_hDC != NULL)
		{
			ReleaseDC(_hWnd, _hDC);
			_hDC = NULL;
		}

		isShared = FALSE;
	}

	void swapBuffer()
	{
		SwapBuffers(_hDC);//windows的函数
	}

};

