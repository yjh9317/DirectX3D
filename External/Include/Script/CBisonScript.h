#pragma once
#include <Engine/CScript.h>

class CBisonScript :
    public CScript
{
private:
    CGameObject* m_pHPUI;
    CScript* m_pHPUIScript;
    CStructuredBuffer* m_pBuffer;
    bool    m_bIdleCheck;
    int     m_MaxHP;
    int     m_CurHP;

    int             m_iHit;
    float           m_ftime;
public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;

    CLONE(CBisonScript)

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    CBisonScript();
    CBisonScript(const CBisonScript& _origin);
    ~CBisonScript();
};

