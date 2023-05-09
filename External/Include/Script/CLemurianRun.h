#pragma once
#include <Engine/CScript.h>

class CLemurianRun :
    public CScript
{
private:
    float   m_fAttackTime;
public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CLemurianRun)
public:
    CLemurianRun();
    ~CLemurianRun();
};

