using System;
using System.Runtime.InteropServices;
using UnityEngine;

public class OglCoreTest : MonoBehaviour {

    public RenderTexture rt;

    private const string dllName = "OpenGLRendering";

    private bool isWindowOpen = false;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
		if(isWindowOpen)
        {
            SetTexture(rt.GetNativeTexturePtr());
            GL.IssuePluginEvent(GetRenderEventFunc(),InfoType.WINDOWMSG);
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.DRAW);
        }
	}

    private void OnGUI()
    {
        if(GUILayout.Button("打开窗口"))
        {
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.OPENWINDOW);
        }
        if (GUILayout.Button("关闭窗口"))
        {
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.CLOSEWINDOW);
        }
    }


    [DllImport(dllName)]
    private static extern IntPtr GetRenderEventFunc();

    [DllImport(dllName)]
    private static extern void SetTexture(IntPtr texturePtr);
}

public static class InfoType
{
    public const int WINDOWMSG = 100;
    public const int OPENWINDOW = 101;
    public const int DRAW = 102;
    public const int CLOSEWINDOW = 103;
}
