#pragma once
#include <Engine/CScript.h>

class CDirectionalLightScript :
    public CScript
{

public:
    virtual void start() override;
    virtual void update() override;

    CLONE(CDirectionalLightScript);

public:
    CDirectionalLightScript();
    ~CDirectionalLightScript();
};

