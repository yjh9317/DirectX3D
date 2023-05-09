#pragma once
#include <Engine/CScript.h>

class CBellScript :
    public CScript
{
private:
    float   m_fTime;
    float   m_fSpeed;
    Vec3    m_vFrontDir;

    

public:
    virtual void update() override;
    void SetSpeed(float _f) { m_fSpeed = _f; }
    void SetFrontDir(Vec3 _v) { m_vFrontDir = _v; }

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    
    CLONE(CBellScript)
public:
    CBellScript();
    ~CBellScript();
};

