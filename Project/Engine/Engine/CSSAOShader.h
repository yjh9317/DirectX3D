#pragma once
#include "CComputeShader.h"
class CSSAOShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_FirstBuffer;
    CStructuredBuffer* m_SecondBuffer;

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    void SetFirstBuffer(CStructuredBuffer* pBuffer) { m_FirstBuffer = pBuffer; }
    void SetSecondBuffer(CStructuredBuffer* pBuffer) { m_SecondBuffer = pBuffer; }

public:
    CSSAOShader();
    ~CSSAOShader();
};

