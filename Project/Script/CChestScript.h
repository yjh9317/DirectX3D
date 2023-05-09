#pragma once
#include <Engine/CScript.h>

class CChestScript :
    public CScript
{
private:
    CStructuredBuffer* m_pBuffer;
    bool    m_bIdleCheck;

public:
    virtual void update();

    CLONE(CChestScript);
public:
    CChestScript();
    CChestScript(const CChestScript& _origin);
    ~CChestScript();
};

