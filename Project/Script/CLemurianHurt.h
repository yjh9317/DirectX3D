#pragma once
#include <Engine/CScript.h>
class CLemurianHurt :
    public CScript
{
public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CLemurianHurt)
public:
    CLemurianHurt();
    ~CLemurianHurt();
};

