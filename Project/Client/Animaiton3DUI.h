#pragma once

#include "ComponentUI.h"

class Animaiton3DUI :
    public ComponentUI
{
private:
    string StrSave;
    string StrLoad;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    Animaiton3DUI();
    ~Animaiton3DUI();
};

