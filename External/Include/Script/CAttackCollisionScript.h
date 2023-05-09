#pragma once

#include <Engine/CScript.h>

class CAttackCollisionScript :
    public CScript
{
private:
    float   m_fCurTime;
    float   m_fEndTime;

public:
    virtual void update() override;

    void SetEndTime(float _f) { m_fEndTime = _f; }

    CLONE(CAttackCollisionScript)
public:
    CAttackCollisionScript();
    ~CAttackCollisionScript();
};

