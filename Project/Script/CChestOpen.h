#pragma once
#include <Engine/CScript.h>

class CChestOpen :
    public CScript
{
private:
    bool    m_bOpen;
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void update() override;

    CLONE(CChestOpen)
public:
    CChestOpen();
    ~CChestOpen();
};

