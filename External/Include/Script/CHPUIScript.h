#pragma once
#include <Engine/CScript.h>

class CHPUIScript :
    public CScript
{
private:
    CGameObject*    m_pCamera;
    float           m_fHPRatio;

public:
    virtual void start();
    virtual void update();


    void SetHPRatio(float _f) { m_fHPRatio = _f; }


    CLONE(CHPUIScript)
public:
    CHPUIScript();
    ~CHPUIScript();
};

