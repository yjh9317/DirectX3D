#pragma once

#include <Engine/CScript.h>

class CPlayerRun :
    public CScript
{
private:
    float   m_fSpeed;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CPlayerRun)
public:
    CPlayerRun();
    ~CPlayerRun();
};

