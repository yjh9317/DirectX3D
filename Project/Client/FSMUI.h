#pragma once
#include "ComponentUI.h"
class FSMUI :
    public ComponentUI
{


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    FSMUI();
    ~FSMUI();
};

