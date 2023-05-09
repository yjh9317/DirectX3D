#pragma once
#include "CComponent.h"
class CBounding :
    public CComponent
{
private:
    Vec3    m_vScale;

    CGameObject* m_pDebugObject;

public:
    virtual void finalupdate() override;

    float GetRadius() { return m_vScale.x; }
    void SetScale(float _f)
    { 
        m_vScale.x = _f; 
        m_vScale.y = _f;
        m_vScale.z = _f;
    }


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CBounding)

    

public:
    CBounding();
    ~CBounding();
};

