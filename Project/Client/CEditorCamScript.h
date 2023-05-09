#pragma once
#include <Engine\CScript.h>

class CEditorCamScript :
    public CScript
{
private:
    float   m_fSpeed;
    float   m_fDist;

public:
    virtual void update() override;


    CLONE(CEditorCamScript);
public:
    CEditorCamScript();
    ~CEditorCamScript();
};

