#pragma once
#include "CComputeShader.h"
class CHDRFirstPass :
    public CComputeShader
{
private:
    Ptr<CTexture>       m_pTex;
    CStructuredBuffer* m_pBuffer;

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    void SetBuffer(CStructuredBuffer* pBuffer) { m_pBuffer = pBuffer; }
    void SetTex (Ptr<CTexture> _pTex) { m_pTex = _pTex; }


public:
    CHDRFirstPass();
    ~CHDRFirstPass();
};

