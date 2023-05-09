#pragma once
#include <Engine/CScript.h>
class CImpDeath :
    public CScript
{

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CImpDeath)
public:
    CImpDeath();
    ~CImpDeath();
};

