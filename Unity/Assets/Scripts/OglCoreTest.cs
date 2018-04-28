using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Runtime.InteropServices;

public class OglCoreTest : MonoBehaviour {

    public RenderTexture rt;

    private bool isWindowOpen = false;

    private const string dllName = "OglNativePlugin";

    private Coroutine sendTexCoroutine;
	// Use this for initialization
	void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        //if (isWindowOpen)
        //{
        //    SetTexture(rt.GetNativeTexturePtr());
        //    //发送窗口消息机制
        //    GL.IssuePluginEvent(GetRenderEventFunc(), 100);
        //    //发送每帧渲染消息
        //    GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.DRAW);
        //    //DrawEveryFrame();
        //}
    }

    void OnGUI()
    {
        if (GUILayout.Button("打开窗口"))
        {
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.OPEN);
            isWindowOpen = true;

            //sendTexCoroutine = StartCoroutine(SendTexture());
        }
        if(GUILayout.Button("关闭窗口"))
        {
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.CLOSE);
            isWindowOpen = false;
        }
    }

    void LateUpdate()
    {
        //rt.GetNativeTexturePtr();
        if (isWindowOpen)
        {
            //激活图像指针，这个真是有点怪异。
            //rt.GetNativeTexturePtr();
            //SetTexture(rt.GetNativeTexturePtr());
            //发送窗口消息机制
            //GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.WINDOW_MESSAGE);
            //发送每帧渲染消息
            //GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.DRAW);
            //DrawEveryFrame();
        }
    }

    void FixedUpdate()
    {
        if (isWindowOpen)
        {
            SetTexture(rt.GetNativeTexturePtr());
            DrawEveryFrame();
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.WINDOW_MESSAGE);
        }
    }

    IEnumerator SendTexture()
    {
        while (isWindowOpen)
        {
            
            yield return new WaitForEndOfFrame();
            SetTexture(rt.GetNativeTexturePtr());
            //发送窗口消息机制
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.WINDOW_MESSAGE);
            //发送每帧渲染消息
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.DRAW);
        }
        StopCoroutine(sendTexCoroutine);
    }

    //[DllImport(dllName)]
    //public static extern IntPtr MonitorFromWindow(IntPtr hWnd, uint uFlag);

    [DllImport(dllName)]
    private extern static IntPtr GetRenderEventFunc();

    [DllImport(dllName)]
    private extern static void SetTexture(IntPtr textureAddr);

    [DllImport(dllName)]
    private extern static void DrawEveryFrame();
}

public static class InfoType
{
    public const int WINDOW_MESSAGE = 100;
    public const int OPEN = 101;
    public const int DRAW = 102;
    public const int CLOSE = 103;
    public const int READ_TEXTURE = 104;
}
