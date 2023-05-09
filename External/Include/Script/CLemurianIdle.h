#pragma once
#include <Engine/CScript.h>
class CLemurianIdle :
    public CScript
{

public:
    virtual void start() override;
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void update() override;

    CLONE(CLemurianIdle)
public:
    CLemurianIdle();
    ~CLemurianIdle();
};

