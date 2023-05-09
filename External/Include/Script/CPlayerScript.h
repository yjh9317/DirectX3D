#pragma once

#include <Engine/CScript.h>

class CStructuredBuffer;

class CPlayerScript :
    public CScript
{
private:
    CGameObject*        m_pBloodScreen;
    CGameObject*        m_pHPUI;
    CScript*            m_pHPUIScript;
    
    
    float               m_fSpeed;
    int                 m_iCurHP;
    int                 m_iMaxHP;


    CStructuredBuffer*  m_pBuffer;
    float               m_fheight;


    bool                m_bIdleCheck;

public:
    virtual void start();
    virtual void update();
    virtual void lateupdate();

    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;


    float GetSpeed() { return m_fSpeed; }
    int GetHP() { return m_iCurHP; }

    void SetSpeed(float _f) { m_fSpeed = _f; }
    void SetHP(int _i) 
    {
        if (_i > m_iMaxHP)
            m_iCurHP = m_iMaxHP;
        else
            m_iCurHP = _i; 
    }

    void RotationValueLimit(Vec3& RadianValue);

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    CLONE(CPlayerScript)

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& _origin);
    ~CPlayerScript();

};

