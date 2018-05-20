#include<Windows.h>
#include"DXWindow.hpp"
#include"Debug.h"
#include"Unity\IUnityGraphics.h"
#include"Unity\IUnityInterface.h"
#include"GLEW\glew.h"
#include"ShaderResource.hpp"
#include<assert.h>
#include"GLContext.h"
#include"TextureQuad.hpp"

using namespace dxlib;

DXWindow	dxWind;
HGLRC		u3dhRC;

///Frame buffer object,帧缓冲对象。
///将图像渲染到帧缓冲对象中而不是直接渲染到窗口。
GLuint fbo;
GLuint fboTexture;

GLContext glContext;

byte* headerData;
int headerWidth;
int headerHeight;

//顶点数据不是以Vector3的形式存储，而是以float的形式存储，这个有点意外。
float* vertexArray;
int*   indexArray;

int vertexLength;
int indicesLength;

//是否是绘制左右。
BOOL isDrawLR;

TextureQuad texQuad;
TextureQuad headerQuad;

BOOL isDrawHeader = FALSE;
//创建Vertex和Fragment shader.

static const char* kGlesVProgTextGLCore = VERTEX_SHADER_SRC();

static const char* kGlesFSingleShaderTextGLCore = FRAGMENT_SINGLE_TEXTURE_SHADER_SRC();

static const char* kGlesFDoubleShaderTextGLCore = FRAGMENT__DOUBLE_TEXTURE_SHADER_SRC();


static void CreateResources()
{
	if (dxWind._hWnd == NULL)
		return;

	vertexArray = new float[20]{
		-1.f, -1.f, 0.f, 0.f,0.f,
		-1.f,  1.f, 0.f, 0.f,1.f,
		1.f,  1.f, 0.f, 1.f,1.f,
		1.f, -1.f, 0.f ,1.f,0.f };
	//vertexArray = new float[12]{
	//	-1.f, -1.f, 0.f,
	//	-1.f,  1.f, 0.f,
	//	1.f,  1.f, 0.f, 
	//	1.f, -1.f, 0.f };
	//逆时针
	indexArray = new int[6]{ 0,2,1,3,2,0 };
	texQuad.CreateResource(vertexArray, 20, indexArray, 6);

	if (isDrawHeader)
	{
		//设置FBO，将FBO渲染设置到图像当中。
		glGenTextures(1, &fboTexture);
		glBindTexture(GL_TEXTURE_2D, fboTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, dxWind.width, dxWind.height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0,dxWind.height - 1,headerWidth,headerHeight, GL_RGBA, GL_UNSIGNED_BYTE, headerData);

		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		headerQuad.CreateResource(vertexArray, 20, indexArray, 6);
	}
}

static void ChangeSize(int width, int height)
{
	dxWind.width = width;
	dxWind.height = height;
}

LRESULT CALLBACK  WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	switch (message)
	{
	case WM_CLOSE:
		dxWind.Close();
		Debug::GetInst()->Log("Close window。\r\n");
		break;
	case WM_DESTROY:
		//glContext.shutdown();
		Debug::GetInst()->Log("Destroy window。\r\n");
		glContext.shutdown();
		texQuad.Clean();
		headerQuad.Clean();
		//dxWind.Close();
		u3dhRC = NULL;
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		ChangeSize(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		//Debug::GetInst()->Log("Default.\r\n");
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

///打开窗口并设置窗口渲染的上下文等。
static void SetupWindow()
{
	//打开窗口
	//BOOL result = dxWind.Create(WndProc,GetCurrentWin);
	if (dxWind._hWnd == NULL)
		dxWind.Create(WndProc, NULL);

	//自主创建绘制上下文，有点问题。
	//PIXELFORMATDESCRIPTOR pfd;
	//imageDC = GetDC(dxWind._hWnd);

	//int nPixelFormat = 2;

	////了解一下该函数的作用
	//SetPixelFormat(imageDC, nPixelFormat, &pfd);
	////创建OGL上下文，并将其设置为当前的窗口。
	//u3dhRC = wglCreateContext(imageDC);

	//BOOL makeCurr = wglMakeCurrent(imageDC, u3dhRC);

	//获取Unity的绘制上下文。
	u3dhRC = wglGetCurrentContext();
	glContext.setup(dxWind._hWnd, GetDC(dxWind._hWnd));
	glContext.shareLists(u3dhRC);
	//wglShareLists()


	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
		Debug::GetInst()->Log("glContext create error:%d.\r\n", err);

	const GLubyte  *oglVersion = glGetString(GL_VERSION);
	Debug::GetInst()->Log("This system supports OpenGL Version %s.\r\n", oglVersion);
}

static void UNITY_INTERFACE_API UnityRendering(int eventID)
{
	if (eventID == 100)
	{
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//创建窗口并创建上下文。
	else if (eventID == 101)
	{
		if (dxWind._hWnd != NULL)
		{
			Debug::GetInst()->Log("Window is existed.\r\n");
			return;
		}
		Debug::GetInst()->Log("Open window.\r\n");

		SetupWindow();
		glewExperimental = GL_TRUE;//Needed for core profile  
		GLenum init = glewInit();
		if (init != GLEW_OK)
			Debug::GetInst()->Log("glew初始化错误:%s.", glewGetErrorString(init));
		//要先创建上下文，才能创建shader和resources.

		CreateResources();
		//CreateShader();
		////可在Update里写，也可以在这里写。
		//glClearColor(0, 0, 1, 1);
	}
	//开始每帧渲染
	else if (eventID == 102)
	{
		//if(dxWind._hWnd !=NULL)
		//	Draw();
	}
	//关闭窗口
	else if (eventID == 103)
	{
		if (dxWind._hWnd == NULL)
		{
			dxlib::Debug::Log("Window was already closed.\r\n");
			return;
		}
		else
			//Close window
			dxlib::Debug::Log("CloseWindow was called.\r\n");
		//glContext.shutdown();
		dxWind.Close();
		//u3dhRC = NULL;
	}
	//设置shader,shader设置必须要在GL.IssuePlugin内，
	//即必须要保证与打开窗口在同一个渲染线程内。
	else if (eventID == 104)
	{
		//glBindTexture(GL_TEXTURE_2D,)
		//CreateShader();
		if(isDrawLR == TRUE)
			texQuad.CreateShader(kGlesVProgTextGLCore, kGlesFDoubleShaderTextGLCore);
		else
			texQuad.CreateShader(kGlesVProgTextGLCore, kGlesFSingleShaderTextGLCore);

		if (isDrawHeader)
			headerQuad.CreateShader(kGlesVProgTextGLCore, kGlesFSingleShaderTextGLCore);
	}
}

extern "C" _declspec(dllexport) void DrawEveryFrame(void* lTextureAddr, void* rTextureAddr)
{
	if (dxWind._hWnd == NULL)
		return;
	//这个函数是必须的，不然会没有效果。
	glContext.makeCurrent();

	//BOOL wglMakeCur = wglMakeCurrent(imageDC, u3dhRC);
	GLenum glEnum = glGetError();
	if (glEnum != GL_NO_ERROR)
		Debug::GetInst()->Log("Draw error:%d.\r\n", glEnum);
	if(isDrawHeader)
	{
		//如果需要绘制文件头的话，就需要先绘制到FBO，然后将图片绘制到窗口上去。
		//渲染到FBO
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glViewport(0, 0, dxWind.width, dxWind.height);
		texQuad.Draw((GLuint)(size_t)lTextureAddr, (GLuint)(size_t)rTextureAddr,false,0,0,0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, dxWind.width, dxWind.height);
		headerQuad.Draw(fboTexture, 0, true, headerWidth, headerHeight, headerData);
	}
	else
	{
		glClearColor(0, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glViewport(0, 0, dxWind.width, dxWind.height);
		texQuad.Draw((GLuint)(size_t)lTextureAddr, (GLuint)(size_t)rTextureAddr, false, 0, 0, 0);
	}
	//SwapBuffers(imageDC);
	glContext.swapBuffer();
}


extern "C" _declspec(dllexport) void SetHeaderData(byte* data, int w, int h)
{
	//if (dxWind._hWnd == NULL)
	//	return;
	Debug::GetInst()->Log("Header width:%d,Header height:%d,header data:%d.\r\n", w, h, data);
	isDrawHeader = true;
	headerData = data;
	headerWidth = w;
	headerHeight = h;
}

///函数签名，一定要正确，不是extern "C" static  xxx  xxx(xxx xx);
///设置该渲染方式是左右渲染还是单图渲染。
extern "C" _declspec(dllexport) void SetIsLR(BOOL m_isDrawLR)
{
	isDrawLR = m_isDrawLR;
}

//Unity 的GL.IssuePluginEvent函数中调用的接口。这里的函数签名必须要带上UnityReneringEvent。
extern "C" UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API GetRenderEventFunc()
{
	return UnityRendering;
}


extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	Debug::GetInst()->CreatLogFile("LogFile.txt");
	Debug::Log("UnityPluginLoad():加载插件 UnityPlugin  -  Load  \r\n");

	IUnityGraphics* s_Graphics = unityInterfaces->Get<IUnityGraphics>();
	UnityGfxRenderer apiType = s_Graphics->GetRenderer();
	if (apiType != kUnityGfxRendererOpenGLCore)
	{
		Debug::Log("Please use opengl core api.\r\n");
		return;
	}
}