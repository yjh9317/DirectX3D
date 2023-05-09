#pragma once
#include <Engine/CScript.h>

class CGigantSpawn :
    public CScript
{
private:
    bool    m_bSpawn;

    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantSpawn)
public:
    CGigantSpawn();
    ~CGigantSpawn();
};

