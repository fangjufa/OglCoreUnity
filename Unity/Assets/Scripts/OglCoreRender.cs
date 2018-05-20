using UnityEngine;
using System;
using System.Runtime.InteropServices;

public class OglCoreRender : MonoBehaviour {

    public Texture2D header;

    public RenderTexture leftRT;

    public RenderTexture rightRT;

    public bool isDrawHeader;

    private bool isWindowOpen = false;

    /// <summary>
    ///是否是左右显示。0为单图显示，1为左右双图显示。
    /// </summary>
    private int isLR;

    private const string dllName = "OglNativePlugin";

    private byte[] headerData;

    public void OpenWindow()
    {
        if (leftRT == null && rightRT == null)
        {
            Debug.LogError("请给leftRT和rightRT至少一个变量赋值！");
            return;
        }
        GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.OPEN);
        isWindowOpen = true;

        if (leftRT == null || rightRT == null)
            isLR = 0;
        else
            isLR = 1;
        SetIsLR(isLR);
        GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.SHADER_SETTING);
    }

    public void CloseWindow()
    {
        GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.CLOSE);
        isWindowOpen = false;
    }

    void Start()
    {
        if (isDrawHeader)
        {
            headerData = header.GetRawTextureData();
            SetHeaderData(headerData, header.width, header.height);
        }
    }

    void OnGUI()
    {
        if (GUILayout.Button("打开窗口"))
        {
            //sendTexCoroutine = StartCoroutine(SendTexture());
            OpenWindow();
        }
        if(GUILayout.Button("关闭窗口"))
        {
            CloseWindow();
        }
    }

    void FixedUpdate()
    {
        if (isWindowOpen)
        {
            if (leftRT == null && rightRT == null)
            {
                Debug.LogError("请给leftRT和rightRT至少一个变量赋值！");
                return;
            }

            if (isLR == 0)
                DrawEveryFrame(leftRT == null ? rightRT.GetNativeTexturePtr() : leftRT.GetNativeTexturePtr(), IntPtr.Zero);
            else
                DrawEveryFrame(leftRT.GetNativeTexturePtr(), rightRT.GetNativeTexturePtr());
            GL.IssuePluginEvent(GetRenderEventFunc(), InfoType.WINDOW_MESSAGE);
        }
    }



    void OnDestroy()
    {
        CloseWindow();
    }

    [DllImport(dllName)]
    private extern static IntPtr GetRenderEventFunc();

    [DllImport(dllName)]
    private extern static void SetTexture(IntPtr textureAddr);

    [DllImport(dllName)]
    private extern static void DrawEveryFrame();

    [DllImport(dllName)]
    private extern static void DrawEveryFrame(IntPtr lTex,IntPtr rTex);

    [DllImport(dllName)]
    private extern static void SetIsLR(int isLR);

    [DllImport(dllName)]
    private extern static void SetHeaderData(byte[] data,int w,int h);
}

public static class InfoType
{
    public const int WINDOW_MESSAGE = 100;
    public const int OPEN = 101;
    public const int DRAW = 102;
    public const int CLOSE = 103;
    public const int SHADER_SETTING = 104;
}
