#pragma once
#include <Engine/CScript.h>
class CGigantEnd :
    public CScript
{
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantEnd)
public:
    CGigantEnd();
    ~CGigantEnd();
};

