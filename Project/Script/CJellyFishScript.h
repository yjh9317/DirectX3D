#pragma once
#include <Engine/CScript.h>

class CJellyFishScript :
    public CScript
{
private:
    CGameObject*    m_pHPUI;
    CScript*        m_pHPUIScript;
    float           m_CurHP;
    float           m_MaxHP;
    bool            m_bIdleCheck;

    int             m_iHit;
    float           m_ftime;


public:
    virtual void start() override;
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;
    CLONE(CJellyFishScript)
public:
    CJellyFishScript();
    ~CJellyFishScript();
};

