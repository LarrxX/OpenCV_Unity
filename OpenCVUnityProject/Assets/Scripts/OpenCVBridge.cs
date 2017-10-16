using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class OpenCVBridge : MonoBehaviour {

    public Material m_material;

    private OpenCVDllInterface m_dllInterface;
    private OpenCVDllInterface.FrameInfo m_frameInfo;
    private Texture2D m_texture;

    private delegate void eventCallbackDelegate(int eventID);
    eventCallbackDelegate m_eventCallback = null;

    void OnDestroy()
    {
        StopCoroutine("CallPluginAtEndOfFrames");
        m_dllInterface.Dispose();
        m_dllInterface = null;
    }

    IEnumerator Start() {
        m_dllInterface = new OpenCVDllInterface();
        m_frameInfo = new OpenCVDllInterface.FrameInfo();

        m_eventCallback = new eventCallbackDelegate(m_dllInterface.updateFrameDataOGL);

        m_dllInterface.getFrameBufferInfo(m_frameInfo);


        m_texture = new Texture2D(m_frameInfo.width, m_frameInfo.height, TextureFormat.RGB24, false);
        m_texture.filterMode = FilterMode.Point;
        m_texture.Apply();
        m_material.mainTexture = m_texture;

        m_dllInterface.setTextureHandle(m_texture.GetNativeTexturePtr());

        yield return StartCoroutine("CallPluginAtEndOfFrames");
    }

    // Update is called once per frame
    void Update() {
    }

    private IEnumerator CallPluginAtEndOfFrames()
    {
        if (m_eventCallback != null)
        {
            while (m_dllInterface != null)
            {
                yield return null;

                GL.IssuePluginEvent(Marshal.GetFunctionPointerForDelegate(m_eventCallback), 1);
            }
        }
    } 
}
