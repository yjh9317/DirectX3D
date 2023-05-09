#pragma once

#include "UI.h"

class AnimationListUI :
    public UI
{
private:
    CGameObject* pObj;

    string  m_str;
    int     m_istratframe;
    int     m_iendframe;

    int     m_ifixstartframe;
    int     m_ifixendframe;
    float   m_ifixAnimSpeed;


public:
    virtual void render_update() override;

private:
    void AnimClip();
    void CreateAnim();
    void AnimInfo();

public:
    AnimationListUI();
    ~AnimationListUI();
};

