#pragma once
#include <Engine/CScript.h>
class CSubScript :
    public CScript
{
private:
    CGameObject* m_pHPUI;
    CScript* m_pHPUIScript;
    CStructuredBuffer* m_pBuffer;
    float    m_bIdleCheck;
    int     m_MaxHP;
    int     m_CurHP;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CLONE(CSubScript)

public:
    CSubScript();
    CSubScript(const CSubScript& _origin);
    ~CSubScript();
};

