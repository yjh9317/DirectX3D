#pragma once
#include <Engine/CScript.h>

class CSummon3Script :
    public CScript
{

public:
    virtual void update() override;

    CLONE(CSummon3Script);

public:
    CSummon3Script();
    ~CSummon3Script();
};

