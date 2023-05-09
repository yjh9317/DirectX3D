#pragma once
#include <Engine/CScript.h>

class CPlayerIdle :
    public CScript
{

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CPlayerIdle)
public:
    CPlayerIdle();
    ~CPlayerIdle();
};

