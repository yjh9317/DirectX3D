#pragma once
#include <Engine/CScript.h>

class CBloodScreenScript :
    public CScript
{
private:
    float   m_fTime;

public:
    virtual void start();
    virtual void update();

    void SetTime(float _f) { m_fTime = _f; }

    CLONE(CBloodScreenScript)
public:
    CBloodScreenScript();
    ~CBloodScreenScript();
};

