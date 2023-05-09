#pragma once
#include <Engine/CScript.h>

class CBisonRun :
    public CScript
{
private:
    float   m_fAttackTime;

    Vec2    m_vDir;
    float   m_fTime;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    void SetDir(Vec2 _v) { m_vDir = _v; }

    CLONE(CBisonRun)

public:
    CBisonRun();
    ~CBisonRun();
};

