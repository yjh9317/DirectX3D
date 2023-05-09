#pragma once
#include <Engine/CScript.h>
class CGigantDeath :
    public CScript
{
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantDeath)
public:
    CGigantDeath();
    ~CGigantDeath();
};

