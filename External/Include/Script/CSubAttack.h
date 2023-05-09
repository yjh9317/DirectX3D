#pragma once
#include <Engine/CScript.h>
class CSubAttack :
    public CScript
{
private:
    bool     m_bAttack;
public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CSubAttack)
public:
    CSubAttack();
    ~CSubAttack();
};

