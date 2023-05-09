#pragma once

#include <Engine/CScript.h>

class CPlayerJump :
    public CScript
{
private:
    float   m_fSpeed;
    float   m_fTime;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CPlayerJump)
public:
    CPlayerJump();
    ~CPlayerJump();
};

