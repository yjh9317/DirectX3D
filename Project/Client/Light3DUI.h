#pragma once
#include "ComponentUI.h"
class Light3DUI :
    public ComponentUI
{


public:
	virtual void update() override;
	virtual void render_update() override;

public:
    Light3DUI();
    ~Light3DUI();
};

