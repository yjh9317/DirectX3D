#pragma once
#include <Engine/CScript.h>

class CSummon1Script :
    public CScript
{
public:
    virtual void update() override; 

    CLONE(CSummon1Script)

public:
    CSummon1Script();
    ~CSummon1Script();
};

