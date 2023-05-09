#pragma once
#include <Engine/CScript.h>
class CLemurianFireball :
    public CScript
{
private:
    bool    IsFire;


public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CLemurianFireball)
public:
    CLemurianFireball();
    ~CLemurianFireball();
};

