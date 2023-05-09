#pragma once
#include <Engine/CScript.h>
class CGigantLoop :
    public CScript
{
private:
    float m_fTime;
    float m_fFireTime;

    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    CLONE(CGigantLoop)
public:
    CGigantLoop();
    ~CGigantLoop();
};

