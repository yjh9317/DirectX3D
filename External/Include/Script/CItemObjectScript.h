#pragma once
#include <Engine/CScript.h>

class CItemObjectScript :
    public CScript
{
private:
    CStructuredBuffer* m_pBuffer;

public:
    virtual void update() override;

    CLONE(CItemObjectScript)

public:
    CItemObjectScript();
    CItemObjectScript(const CItemObjectScript& _origin);
    ~CItemObjectScript();
};

