#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
public:
    virtual void update() override;
    virtual void render_update() override;

public:
    DecalUI();
    ~DecalUI();
};

