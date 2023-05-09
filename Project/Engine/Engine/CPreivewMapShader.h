#pragma once
#include "CComputeShader.h"
class CPreivewMapShader :
    public CComputeShader
{
private:
    Ptr<CTexture>       m_pPreviewMap;
    Ptr<CTexture>       m_pBrushTex;
    Vec2                m_vScale; // 높이 Brush 크기
    int                 m_iBrushIdx;
    CStructuredBuffer* m_pInput; // Ray 충돌 위치    


public:
    void SetPreviewMap(Ptr<CTexture> _pTex) { m_pPreviewMap = _pTex; }
    void SetBrushTex(Ptr<CTexture> _pTex) { m_pBrushTex = _pTex; }
    void SetBrushScale(Vec2 _vScale) { m_vScale = _vScale; }
    void SetBrushIndex(int _iIdx) { m_iBrushIdx = _iIdx; }
    void SetInputBuffer(CStructuredBuffer* _pInput) { m_pInput = _pInput; }

    void SetZero(int _i) { m_Param.iArr[3] = _i; }


public:
    virtual void UpdateData();
    virtual void Clear();

public:
    CPreivewMapShader();
    ~CPreivewMapShader();
};

