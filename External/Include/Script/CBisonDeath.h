#pragma once
#include <Engine/CScript.h>
class CBisonDeath :
    public CScript
{
private:
    bool    m_bDead;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CBisonDeath)

public:
    CBisonDeath();
    ~CBisonDeath();
};

