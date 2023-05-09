#pragma once
#include <Engine/CScript.h>

class CChestIdle :
    public CScript
{
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void update() override;

    CLONE(CChestIdle)

public:
    CChestIdle();
    ~CChestIdle();
};

