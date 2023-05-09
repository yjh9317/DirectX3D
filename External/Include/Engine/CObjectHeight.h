#pragma once
#include "CComputeShader.h"

class CObjectHeight :
    public CComputeShader
{
private:
    Ptr<CTexture>      m_pHeightMap;
    CStructuredBuffer* m_pBuffer;

    Vec3                m_vPos;
    float               m_LandScapeYScale;

public:
    void SetHeightBuffer(CStructuredBuffer* _buffer) { m_pBuffer = _buffer; }

    void SetPos(Vec3 _v) { m_vPos = _v; }
    float GetHeightScale() { return m_LandScapeYScale; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CObjectHeight();
    ~CObjectHeight();
};

