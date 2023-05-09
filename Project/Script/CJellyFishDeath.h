#pragma once
#include <Engine/CScript.h>

class CJellyFishDeath :
    public CScript
{
private:
    bool    m_bCheck;
public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;


    CLONE(CJellyFishDeath)
public:
    CJellyFishDeath();
    ~CJellyFishDeath();

};

