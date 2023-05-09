#pragma once
#include <Engine/CScript.h>

class CBulletScript :
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

    CLONE(CBulletScript)
public:
    CBulletScript();
    ~CBulletScript();
};

