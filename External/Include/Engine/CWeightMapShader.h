#pragma once
#include "CComputeShader.h"

class CWeightMapShader :
    public CComputeShader
{
private:
    CStructuredBuffer*  m_pWeightMap;
    CStructuredBuffer*  m_pRaycastInfo;

    Ptr<CTexture>       m_pBrushArrTex;
    Vec2                m_vBrushScale;
    int                 m_iBrushIdx;
    int                 m_iWeightIdx; // 가중치 수정할 위치

    int                 m_iWidth;
    int                 m_iHeight;

    // 가중치 저장 텍스쳐
    Ptr<CTexture>       m_pWeightTex;

    bool                m_bSave;
    bool                m_bLoad;

public:
    void SetSave(bool _b) { m_bSave = _b; }
    void SetLoad(bool _b) { m_bLoad = _b; }
    void SetWeightMapTex(Ptr<CTexture> _pTex)
    {
        m_pWeightTex = _pTex;
    }

    void SetWeightMap(CStructuredBuffer* _pWeightMap, UINT _iWidth, UINT _iHeight)
    {
        m_pWeightMap = _pWeightMap;
        m_iWidth = _iWidth;
        m_iHeight = _iHeight;
    }

    void SetInputBuffer(CStructuredBuffer* _pRaycastData)
    {
        m_pRaycastInfo = _pRaycastData;
    }

    void SetBrushArrTex(Ptr<CTexture> _pBrushTex)
    {
        m_pBrushArrTex = _pBrushTex;
    }

    void SetBrushScale(Vec2 _vScale)
    {
        m_vBrushScale = _vScale;
    }

    void SetBrushIndex(int _iIdx)
    {
        m_iBrushIdx = _iIdx;
    }

    void SetWeightIdx(int _iIdx)
    {
        m_iWeightIdx = _iIdx;
    }

public:
    virtual void UpdateData();
    virtual void Clear();

public:
    CWeightMapShader();
    ~CWeightMapShader();
};