#pragma once
#include <Engine/CScript.h>

class CPlayerFire :
    public CScript
{
private:
    bool    m_bFire;
    bool    m_bSound;

    int     m_LeftShotNum;
    int     m_RightShotNum;


public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CPlayerFire)
public:
    CPlayerFire();
    ~CPlayerFire();
};

