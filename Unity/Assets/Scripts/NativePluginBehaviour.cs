using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class NativePluginBehaviour : MonoBehaviour
{
    public GameObject go;
    private Mesh m;

    private bool isQuit = false;

    private const string dllName= "OpenGLRendering";
	// Use this for initialization
	void Start ()
    {
        m = go.GetComponent<MeshFilter>().mesh;
        Matrix4x4 mvpMatrix = Camera.main.projectionMatrix * Camera.main.worldToCameraMatrix * transform.localToWorldMatrix;
        float[] mvpArray = new float[16];
        for (int i = 0; i < 16; i++)
        {
            mvpArray[i] = mvpMatrix[i];
        }
        SetMVPMatrix(mvpArray, 16);

        Vector3[] vertexArray = m.vertices;
        int[] indicesArray = m.triangles;
        float[] vertexFArray = new float[vertexArray.Length*3];
        for (int i = 0; i < vertexArray.Length; i++)
        {
            vertexFArray[i * 3] = vertexArray[i].x;
            vertexFArray[i * 3 + 1] = vertexArray[i].y;
            vertexFArray[i * 3 + 2] = vertexArray[i].z;
        }

        SetMeshInfo(vertexFArray, vertexFArray.Length, indicesArray, indicesArray.Length);

        StartCoroutine(NativeRendering());
	}
    IEnumerator NativeRendering()
    {
        while (!isQuit)
        {
            yield return new WaitForEndOfFrame();
            GL.IssuePluginEvent(GetRenderEventFunc(), 101);
        }
    }
	
	// Update is called once per frame
	void Update ()
    {
		
	}

    private void OnApplicationQuit()
    {
        Debug.Log("Application Quit.");
        GL.IssuePluginEvent(GetRenderEventFunc(),111);
        isQuit = true;
    }

    [DllImport(dllName)]
    private static extern void SetMeshInfo(float[] vertexArray,int vertexLength,int[] indicesArray,int indicesLength);

    [DllImport(dllName)]
    private static extern void SetMVPMatrix(float[] vertexArray, int length);

    [DllImport(dllName)]
    private static extern System.IntPtr GetRenderEventFunc();
}
