#pragma once
#include "ComponentUI.h"
class Collider3DUI :
    public ComponentUI
{
public:
    virtual void update() override;
    virtual void render_update() override;

public:
    Collider3DUI();
    ~Collider3DUI();
};

