#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    CGameObject*    m_Filter;
    float           m_fCamSpeed;
    float           m_fDist;
    bool            m_bType;

    bool            m_bVib;

    CGameObject*    m_Player;
    float           m_fCameraYDist;

public:
    void SetFilter(CGameObject* _pFilterObject){m_Filter = _pFilterObject;}

public:
    virtual void start() override;
    virtual void update() override;

    void SetVibration(bool _b) { m_bVib = _b; }
    void SetCam(bool _b) { m_bType = _b; }
    bool GetType() { return m_bType; }

    CLONE(CCameraMoveScript)
    
public:
    CCameraMoveScript();
    ~CCameraMoveScript();
};

