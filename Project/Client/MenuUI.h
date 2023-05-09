#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:
    string  m_strPlayPause;

    bool    m_bCamera;
    bool    m_bCollider3D;
    bool    m_bMeshRender;
    bool    m_bAnimator3D;
    bool    m_bLight3D;
    bool    m_bBounding;
    bool    m_bParticle;
    bool    m_bLandScape;
    bool    m_bDecal;
    bool    m_bFSM;
    bool    m_bSocket;
    bool    m_bScript;

    bool    m_bPackaging;
    bool    m_bSceneSave;
    bool    m_bSceneLoad;
    bool    m_bSceneJsonSave;
    bool    m_bSceneJsonLoad;
    bool    m_bScenePlayPause;
    bool    m_bSceneStop;

    int     m_ObjLayer;
    string  m_ObjName;
    bool    m_bCreateButton;

public:
    virtual void update() override;
    virtual void render() override;
    virtual void render_update() override;

private:
    void Task();
    void CheckAddComponent();

public:
    MenuUI();
    ~MenuUI();
};

