#pragma once
#include <Engine/CScript.h>

class CHDRScript :
    public CScript
{
private:
    CStructuredBuffer* m_FirstBuffer;
    CStructuredBuffer* m_SecondBuffer;


public:
    virtual void update() override;

    CLONE(CHDRScript);

public:
    CHDRScript();
    ~CHDRScript();
};