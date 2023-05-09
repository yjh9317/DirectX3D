#pragma once
#include <Engine/CScript.h>

class CImpScript :
    public CScript
{
private:
    CGameObject* m_pHPUI;
    CScript* m_pHPUIScript;

    CStructuredBuffer* m_pBuffer;
    float    m_fSpeed;
    float    m_bIdleCheck;

    int     m_MaxHP;
    int     m_CurHP;

public:
    virtual void enter();
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CImpScript)

public:
    CImpScript();
    CImpScript(const CImpScript& _origin);
    ~CImpScript();
};

