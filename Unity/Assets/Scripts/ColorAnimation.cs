/*

* @ 创建时间：

* @ 作者：

* @ 修改人：

* @ 修改时间：

* @ 描述：

* @ Copyright 2013-2017 深圳市国泰安信息技术有限公司版权所有

*/

using UnityEngine;
using System.Collections;


public class ColorAnimation: MonoBehaviour {

    public float colorSpeed = 0.1f;
    public float rotateSpeed = 0.5f;
    private Material mat;

    private float curTime;
    private int index = 0;
    // Use this for initialization
    void Start ()
    {
        mat = GetComponent<MeshRenderer>().material;
        mat.color = Color.white;
        curTime = Time.time;
	}

    // Update is called once per frame
    void Update ()
    {
        Color c = mat.color;
        if (index == 0)
        {
            c.r = increaseComponent(c.r);
        }
        else if(index == 1)
        {
            c.g = increaseComponent(c.g);
        }
        else if(index == 2)
        {
            c.b = increaseComponent(c.b);
        }

        mat.color = c;

        transform.Rotate(Vector3.up*rotateSpeed*Time.fixedDeltaTime,Space.World);
	}


    float increaseComponent(float comp)
    {
        comp -= colorSpeed * Time.fixedDeltaTime;
        if (comp <= 0)
        {
            comp = 0;
            index++;
            if(index > 2)
            {
                index = 0;
                colorSpeed = -colorSpeed;
            }
        }
        if (comp >= 1)
        {
            comp = 1;
            index++;
            if (index > 2)
            {
                index = 0;
                colorSpeed = -colorSpeed;
            }
        }
        //mat.color = c;
        return comp;
    }
}