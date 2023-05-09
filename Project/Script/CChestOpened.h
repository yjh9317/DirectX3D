#pragma once
#include <Engine/CScript.h>

class CChestOpened :
    public CScript
{
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void update() override;

    CLONE(CChestOpened)
public:
    CChestOpened();
    ~CChestOpened();
};

