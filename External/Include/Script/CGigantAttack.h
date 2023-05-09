#pragma once
#include <Engine/CScript.h>
class CGigantAttack :
    public CScript
{
private:
    CGameObject* m_HandCollision;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantAttack)
public:
    CGigantAttack();
    ~CGigantAttack();
};

