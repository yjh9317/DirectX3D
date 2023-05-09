#pragma once
#include <Engine/CScript.h>

class CSummon2Script :
    public CScript
{

public:
    virtual void update() override;

    CLONE(CSummon2Script)
public:
    CSummon2Script();
    ~CSummon2Script();
};

