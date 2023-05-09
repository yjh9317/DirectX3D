#include "pch.h"
#include "MenuUI.h"

#include <Engine/CCore.h>
#include <Engine/CPathMgr.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Script/CScriptMgr.h>

#include <Script/CSceneSaveLoad.h>
#include "CImGuiMgr.h"
#include "SceneOutliner.h"
#include "InspectorUI.h"

#include <Engine/CGameObject.h>

#include <Engine/CCamera.h>
#include <Engine/CCollider3D.h>
//#include <Engine/Animator3D.h>
#include <Engine/CLight3D.h>
#include <Engine/CBounding.h>
#include <Engine/CMeshRender.h>
#include <Engine/CParticleSystem.h>
#include <Engine/CLandScape.h>
#include <Engine/CDecal.h>
#include <Engine/CFSM.h>
#include <Engine/CSocket.h>
#include <Engine/CTexture.h>

#include "ListUI.h"
#include "ComponentUI.h"

MenuUI::MenuUI()
    : UI("Menu")
    , m_bPackaging(false)
    , m_bSceneSave(false)
    , m_bSceneLoad(false)
    , m_bScenePlayPause(false)
    , m_bSceneStop(false)
    , m_bCreateButton(false)
    , m_ObjLayer(0)
    , m_bCamera(false)
    , m_bCollider3D(false)
    , m_bMeshRender(false)
    , m_bAnimator3D(false)
    , m_bLight3D(false)
    , m_bBounding(false)
    , m_bParticle(false)
    , m_bLandScape(false)
    , m_bDecal(false)
    , m_bFSM(false)
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::update()
{
    Task();

    CheckAddComponent();
}

void MenuUI::render()
{
    if (ImGui::BeginMainMenuBar())
    {
        render_update();

        ImGui::EndMainMenuBar();
    }
}

void MenuUI::render_update()
{
    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("Packaging", NULL, &m_bPackaging);


        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Scene"))
    {
        ImGui::MenuItem("Scene Save", NULL, &m_bSceneSave);
        ImGui::MenuItem("Scene Load", NULL, &m_bSceneLoad);
        ImGui::MenuItem("Scene Json Save", NULL, &m_bSceneJsonSave);
        ImGui::MenuItem("Scene Json Load", NULL, &m_bSceneJsonLoad);

        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::PLAY == eState)
            m_strPlayPause = "Pause";
        else
            m_strPlayPause = "Play";

        ImGui::MenuItem(m_strPlayPause.c_str(), NULL, &m_bScenePlayPause);

        if (SCENE_STATE::STOP == eState)
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, false);
        else
            ImGui::MenuItem("Stop", NULL, &m_bSceneStop, true);

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Component"))
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            ImGui::MenuItem("Camera", NULL, &m_bCamera);
            ImGui::MenuItem("Collider3D", NULL, &m_bCollider3D);
            ImGui::MenuItem("MeshRender", NULL, &m_bMeshRender);
            ImGui::MenuItem("Animator3D", NULL, &m_bAnimator3D);
            ImGui::MenuItem("Light3D", NULL, &m_bLight3D);
            ImGui::MenuItem("BoundingBox", NULL, &m_bBounding);
            ImGui::MenuItem("Particle", NULL, &m_bParticle);
            ImGui::MenuItem("LandScape", NULL, &m_bLandScape);
            ImGui::MenuItem("Decal", NULL, &m_bDecal);
            ImGui::MenuItem("FSM", NULL, &m_bFSM);
            ImGui::MenuItem("Socket", NULL, & m_bSocket);
            ImGui::MenuItem("Script", NULL, &m_bScript);



            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }


    if (ImGui::BeginMenu("Add Object"))
    {
        ImGui::Text("Name :");
        ImGui::InputText("##Name", (char*)m_ObjName.c_str(), 50);

        ImGui::Text("Layer :");
        ImGui::InputInt("##Layer", &m_ObjLayer);

        if (m_ObjLayer < 0 || m_ObjLayer >= 32)
            m_ObjLayer = 0;

        if (ImGui::Button("Create"))
        {
            string s = m_ObjName.c_str();
            int _size = s.length();
            if (_size)
                m_bCreateButton = true;
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Value"))
    {
        Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DirLightMtrl.mtrl");
        static float ff = 0.001f;
        ImGui::InputFloat("Shadow", &ff);
        pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &ff);

        ImGui::EndMenu();
    }
}



void MenuUI::Task()
{
    if (m_bSceneSave)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetSaveFileName(&ofn))
        {
            CSceneSaveLoad::SaveScene(CSceneMgr::GetInst()->GetCurScene(), szName);
            //CSceneSaveLoad::SaveSceneToJson(CSceneMgr::GetInst()->GetCurScene(), szName);
        }

        m_bSceneSave = false;
    }

    else if (m_bSceneLoad)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"tile";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetOpenFileName(&ofn))
        {
            CScene* pLoadScene = CSceneSaveLoad::LoadScene(szName);
            CSceneMgr::GetInst()->ChangeScene(pLoadScene);
        }

        // SceneOutliner 갱신
        SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
        pUI->Reset();

        m_bSceneLoad = false;
    }

    if (m_bSceneJsonSave)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Scene\0*.scene\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"scene";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetSaveFileName(&ofn))
        {
            CSceneSaveLoad::SaveSceneToJson(CSceneMgr::GetInst()->GetCurScene(), szName);
        }

        m_bSceneJsonSave = false;
    }

    if (m_bSceneJsonLoad)
    {
        wchar_t szName[256] = {};

        OPENFILENAME ofn = {};

        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = CCore::GetInst()->GetMainHwnd();
        ofn.lpstrFile = szName;
        ofn.nMaxFile = sizeof(szName);
        ofn.lpstrFilter = L"ALL\0*.*\0Tile\0*.tile\0";
        ofn.nFilterIndex = 0;
        ofn.lpstrFileTitle = nullptr;
        ofn.nMaxFileTitle = 0;

        wstring strTileFolder = CPathMgr::GetInst()->GetContentPath();
        strTileFolder += L"tile";

        ofn.lpstrInitialDir = strTileFolder.c_str();
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Modal
        if (GetOpenFileName(&ofn))
        {
            CScene* pLoadScene = CSceneSaveLoad::LoadSceneToJson(szName);
            CSceneMgr::GetInst()->ChangeScene(pLoadScene);
        }

        // SceneOutliner 갱신
        SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");
        pUI->Reset();

        m_bSceneJsonLoad = false;
    }

    if (m_bScenePlayPause)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PLAY);
        }
        else if (SCENE_STATE::PLAY == eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::PAUSE);
        }

        m_bScenePlayPause = false;
    }

    if (m_bSceneStop)
    {
        CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
        SCENE_STATE eState = pCurScene->GetSceneState();

        if (SCENE_STATE::STOP != eState)
        {
            pCurScene->SetSceneState(SCENE_STATE::STOP);
            CSceneFile* pSceneFile = pCurScene->GetSceneFile().Get();

            wstring strFilePath = CPathMgr::GetInst()->GetContentPath() + pSceneFile->GetRelativePath();
            CScene* pNewScene = CSceneSaveLoad::LoadScene(strFilePath);
            CSceneMgr::GetInst()->ChangeScene(pNewScene);
        }

        // SceneOutliner 갱신
        ((SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner"))->Reset();

        m_bSceneStop = false;
    }


    if (m_bCreateButton)
    {
        SceneOutliner* pUI = (SceneOutliner*)CImGuiMgr::GetInst()->FindUI("SceneOutliner");

        pUI->AddObjToScene(m_ObjName, m_ObjLayer);

        m_ObjName = {};

        m_bCreateButton = false;
    }

}

void MenuUI::CheckAddComponent()
{
    InspectorUI* pUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");

    CGameObject* pObj = pUI->GetTargetObject();

    if (nullptr == pObj)
        return;

    if (m_bCamera)
    {
        

        if (pObj->Camera())
        {
            m_bCamera = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CCamera);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bCamera = false;
    }

    if (m_bCollider3D)
    {

        if (pObj->Collider3D())
        {
            m_bCollider3D = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CCollider3D);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bCollider3D = false;
    }

    if (m_bMeshRender)
    {

        if (pObj->MeshRender())
        {
            m_bMeshRender = false;
            return;
        }

        pObj->AddComponent(new CMeshRender);


        Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
        Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Std3DMtrl.mtrl");

        pObj->MeshRender()->SetMesh(pMesh);

        Ptr<CMaterial> Mtrl = pObj->MeshRender()->GetMtrl()[0].pMtrl;
        pObj->MeshRender()->GetMtrl()[0].pMtrl = pMtrl;
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bMeshRender = false;
    }

    if (m_bAnimator3D)
    {
        //if (pObj->Animator3D())
        //{
        //    m_bAnimator3D = false;
        //    return;
        //}

         //pUI->GetTargetObj()->AddComponent(new CAniamtor3D);
        m_bAnimator3D = false;
    }

    if (m_bLight3D)
    {
        if (pObj->Light3D())
        {
            m_bLight3D = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CLight3D);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bLight3D = false;
    }

    if (m_bBounding)
    {

        if (pObj->Bounding())
        {
            m_bBounding = false;
            return;
        }
        pUI->GetTargetObject()->AddComponent(new CBounding);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bBounding = false;
    }
    if (m_bParticle)
    {

        if (pObj->ParticleSystem())
        {
            m_bParticle = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CParticleSystem);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bParticle = false;
    }

    if (m_bLandScape)
    {

        if (pObj->LandScape())
        {
            m_bLandScape = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CLandScape);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bLandScape = false;
    }

    if (m_bDecal)
    {

        if (pObj->Decal())
        {
            m_bDecal = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CDecal);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bDecal = false;
    }

    if (m_bFSM)
    {

        if (pObj->FSM())
        {
            m_bFSM = false;
            return;
        }

        pUI->GetTargetObject()->AddComponent(new CFSM);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bFSM = false;
    }

    if (m_bSocket)
    {
        if (pObj->Socket())
        {
            m_bSocket = false;
            return;
        }
        pUI->GetTargetObject()->AddComponent(new CSocket);
        pUI->SetTargetObject(pUI->GetTargetObject());
        m_bSocket = false;
    }


    if (m_bScript)
    {
        vector<wstring> vecScriptName;
        CScriptMgr::GetScriptInfo(vecScriptName);

        ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
        pListUI->Clear();
        pListUI->SetTitle("Script List");

        for (int i = 0; i < vecScriptName.size(); ++i)
        {
            pListUI->AddList(string(vecScriptName[i].begin(), vecScriptName[i].end()));
        }

        pListUI->Activate();
        pListUI->SetDBCEvent(this, (DBCLKED)&ComponentUI::ScriptSelect);
        m_bScript = false;
    }
}
