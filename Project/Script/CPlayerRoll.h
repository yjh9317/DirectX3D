#pragma once

#include <Engine/CScript.h>

class CPlayerRoll :
    public CScript
{
private:
    float   m_fSpeed;
    float   m_fTime;
    int     m_iDir;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update() override;

    void SetDir(int _i) { m_iDir = _i; }

    CLONE(CPlayerRoll)
public:
    CPlayerRoll();
    ~CPlayerRoll();
};

