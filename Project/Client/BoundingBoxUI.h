#pragma once
#include "ComponentUI.h"
class BoundingBoxUI :
    public ComponentUI
{

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    BoundingBoxUI();
    ~BoundingBoxUI();
};

