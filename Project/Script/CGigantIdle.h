#pragma once
#include <Engine/CScript.h>
class CGigantIdle :
    public CScript
{
private:
    float m_fTime;
    int     m_iAtk;
    bool    m_bSpawn;
    CGameObject* m_HandCollision;

    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantIdle)
public:
    CGigantIdle();
    ~CGigantIdle();
};

