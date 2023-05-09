#pragma once
#include "ComponentUI.h"
class SocketUI :
    public ComponentUI
{
private:


public:
    virtual void update() override;
    virtual void render_update() override;

public:
    SocketUI();
    ~SocketUI();
};

