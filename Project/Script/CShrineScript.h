#pragma once
#include <Engine/CScript.h>

class CShrineScript :
    public CScript
{

public:
    virtual void update() override;
    CLONE(CShrineScript)

public:
    CShrineScript();
    ~CShrineScript();
};

