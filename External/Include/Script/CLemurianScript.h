#pragma once
#include <Engine/CScript.h>

class CLemurianScript :
    public CScript
{
private:
    CGameObject* m_pHPUI;
    CScript* m_pHPUIScript;
    CStructuredBuffer* m_pBuffer;
    bool    m_bIdleCheck;
    int     m_MaxHP;
    int     m_CurHP;

public:
    virtual void start() override;
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CLONE(CLemurianScript)

public:
    CLemurianScript();
    CLemurianScript(const CLemurianScript& _origin);
    ~CLemurianScript();
};

