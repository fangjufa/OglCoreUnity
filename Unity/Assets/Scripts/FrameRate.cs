using UnityEngine;
using UnityEngine.UI;

public class FrameRate : MonoBehaviour
{
    public Text txt;

    private float currentTime;

    public int frameInterval = 100;

    private int count = 0;
    // Use this for initialization
    void Start()
    {
        currentTime = Time.time;
    }

    // Update is called once per frame
    void Update()
    {
        count++;
        //计算帧率
        if (count >= frameInterval)
        {
            float fps = frameInterval / (Time.time - currentTime);
            txt.text = fps.ToString();
            currentTime = Time.time;
            count = 0;
        }
    }
}