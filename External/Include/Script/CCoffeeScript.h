#pragma once
#include <Engine/CScript.h>
class CCoffeeScript :
    public CScript
{

public:
    virtual void update() override;
    virtual void OnCollisionEnter(CGameObject* _OtherObject) override;

    CLONE(CCoffeeScript)

public:
    CCoffeeScript();
    ~CCoffeeScript();
};

