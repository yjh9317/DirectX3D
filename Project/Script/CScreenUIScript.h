#pragma once
#include <Engine/CScript.h>
class CScreenUIScript :
    public CScript
{
private:
    float m_fHPRatio;

public:
    virtual void update() override;
    
    void SetHPRatio(float _f) { m_fHPRatio = _f; }

    CLONE(CScreenUIScript)

public:
    CScreenUIScript();
    ~CScreenUIScript();
};

