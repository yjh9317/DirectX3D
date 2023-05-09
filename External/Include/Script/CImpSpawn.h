#pragma once
#include <Engine/CScript.h>
class CImpSpawn :
    public CScript
{

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CImpSpawn)
public:
    CImpSpawn();
    ~CImpSpawn();
};

