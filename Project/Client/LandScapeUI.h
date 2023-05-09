#pragma once
#include "ComponentUI.h"
class LandScapeUI :
    public ComponentUI
{
private:
    string m_str;
    bool    m_bCheck;
    bool    m_bWeightCheck;

    string m_WeightString;
    string m_Weight;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    LandScapeUI();
    ~LandScapeUI();
};

