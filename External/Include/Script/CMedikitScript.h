#pragma once
#include <Engine/CScript.h>
class CMedikitScript :
    public CScript
{
private:
    CStructuredBuffer*  m_pBuffer;
    bool                m_b;
    Vec3                m_vDir;
    float               m_fPower;
    float               m_fTime;

    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

public:
    void SetDir(Vec3 _v) { m_vDir = _v; }

    CLONE(CMedikitScript)

public:
    CMedikitScript();
    CMedikitScript(const CMedikitScript& _origin);
    ~CMedikitScript();
};

