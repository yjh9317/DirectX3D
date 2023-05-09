#pragma once
#include <Engine/CScript.h>
class CGigantScript :
    public CScript
{
private:
    CGameObject*        m_pHPUI;
    CScript*            m_pHPUIScript;
    CStructuredBuffer*  m_pBuffer;
    bool                m_bIdleCheck;
    int                 m_MaxHP;
    int                 m_CurHP;

public:
    virtual void start() override;
    virtual void update() override;
    virtual void lateupdate() override;

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    float GetHPRatio() { return float(m_CurHP/float(m_MaxHP)); }

    CLONE(CGigantScript)
public:
    CGigantScript();
    CGigantScript(const CGigantScript& _origin);
    ~CGigantScript();
};

