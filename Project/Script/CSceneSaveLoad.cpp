#include "pch.h"
#include "CSceneSaveLoad.h"

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CPrefab.h>

#include <Engine/CScript.h>
#include "CScriptMgr.h"
#include <fstream>
#include <json/reader.h>

void CSceneSaveLoad::SaveScene(CScene* _pScene, const wstring& _strSceneFilePath)
{   
    // ���ҽ� ������� ����
    CResMgr::GetInst()->SaveChangedRes();

    // Scene ����
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"wb");

    assert(pFile);
    if (nullptr == pFile)
        return;

    // Scene �̸� ����
    _pScene->SaveToScene(pFile);

    // Scene �� Layer ���� ����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer �̸� ����
        CLayer* pLayer = _pScene->GetLayer(i);
        pLayer->SaveToScene(pFile);

        // Layer �� �������� Root ������Ʈ ���� ����
        vector<CGameObject*>& vecRootObj = pLayer->GetRootObjects();
        size_t iObjCount = vecRootObj.size();
        fwrite(&iObjCount, sizeof(size_t), 1, pFile);

        // �� ��Ʈ������Ʈ ����
        for (size_t j = 0; j < vecRootObj.size(); ++j)
        {
            SaveGameObject(vecRootObj[j], pFile);
        }
    }

    fclose(pFile);
}


CScene* CSceneSaveLoad::LoadScene(const wstring& _strSceneFilePath)
{
    // ���� ��ο��� ����θ� ����
    wstring strKey = CPathMgr::GetInst()->GetRelativePath(_strSceneFilePath);

    // CResMgr ���� ����θ� Ű������, CSceneFile �� ã�Ƴ�
    CResMgr::GetInst()->Load<CSceneFile>(strKey, strKey);

    // Load �� Scene ���� �� ���Ϸκ��� �ε�
    CScene* pLoadScene = new CScene;
    pLoadScene->SetResKey(strKey);

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"rb");

    assert(pFile);
    if (nullptr == pFile)
        return nullptr;

    // Scene �̸� �ҷ�����
    pLoadScene->LoadFromScene(pFile);

    // Scene �� Layer ���� �ҷ�����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        // Layer �� �̸� 
        CLayer* pLayer = pLoadScene->GetLayer(i);
        pLayer->LoadFromScene(pFile);

        // Layer ���� ������Ʈ ����
        size_t iObjCount = 0;
        fread(&iObjCount, sizeof(size_t), 1, pFile);

        // Root ������Ʈ
        for (size_t j = 0; j < iObjCount; ++j)
        {
            CGameObject* pLoadObj = LoadGameObject(pFile);
            pLoadScene->AddObject(pLoadObj, i);
        }
    }

    fclose(pFile);

    return pLoadScene;
}

void CSceneSaveLoad::SaveGameObject(CGameObject* _pObj, FILE* _pFile)
{
    // �̸�, ����, ������Ʈ ����
    _pObj->SaveToScene(_pFile);

    // Script ����
    const vector<CScript*>& vecScript = _pObj->GetScripts();
    size_t iScriptCount = vecScript.size();

    fwrite(&iScriptCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring strScriptName = CScriptMgr::GetScriptName(vecScript[i]);
        SaveWStringToFile(strScriptName, _pFile);
        vecScript[i]->SaveToScene(_pFile);
    }

    // Child Object
    const vector<CGameObject*>& vecChild = _pObj->GetChild();
    size_t iChildCount = vecChild.size();
    fwrite(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        SaveGameObject(vecChild[i], _pFile);
    }
}

CGameObject* CSceneSaveLoad::LoadGameObject(FILE* _pFile)
{
    CGameObject* pLoadObj = new CGameObject;

    // �̸�, ����, ������Ʈ �ҷ�����
    pLoadObj->LoadFromScene(_pFile);

    // Script �ҷ�����        
     size_t iScriptCount = 0;
    fread(&iScriptCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring strScriptName;
        LoadWStringFromFile(strScriptName, _pFile);
        CScript* pLoadScript = CScriptMgr::GetScript(strScriptName);
        pLoadObj->AddComponent(pLoadScript);
        pLoadScript->LoadFromScene(_pFile);
    }

    size_t iChildCount = 0;
    fread(&iChildCount, sizeof(size_t), 1, _pFile);

    for (size_t i = 0; i < iChildCount; ++i)
    {
        CGameObject* pChild = LoadGameObject(_pFile);
        pLoadObj->AddChild(pChild);
    }

    return pLoadObj;
}

// ======
// Prefab
// ======
void CSceneSaveLoad::SavePrefab(CPrefab* _Prefab, const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

    SaveGameObject(_Prefab->GetProto(), pFile);

    fclose(pFile);
}

int CSceneSaveLoad::LoadPrefab(CPrefab* _Prefab, const wstring& _strFilePath)
{
    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

    if (nullptr == pFile)
        return E_FAIL;

    CGameObject* pProto = LoadGameObject(pFile);
    _Prefab->SetProto(pProto);

    fclose(pFile);

    return S_OK;
}


void CSceneSaveLoad::SaveSceneToJson(CScene* _pScene, const wstring& _strSceneFilePath)
{
    // ���ҽ� ������� ����
    CResMgr::GetInst()->SaveChangedRes();

    // Scene ����
    // Value���� ����� �ű⿡ �����͸� �߰�
    Json::Value Root;

    FILE* pFile = nullptr;

    wchar_t szExt[50] = {};
    _wsplitpath_s(_strSceneFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
    wstring strExt = szExt;

    if (strExt != L".json")
        return;

    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"wb");

    assert(pFile);
    if (nullptr == pFile)
        return;

    // �����Է��� ���� StyledWriter�� �̿�
    Json::StyledWriter writer;

    //Scene �̸� ����
    _pScene->SaveJsonFile(Root);

    // Scene �� Layer ���� ����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
        Json::Value Layer;

        // Layer �̸� ����
        CLayer* pLayer = _pScene->GetLayer(i);

        if (pLayer->GetName().length() <= 0)
            continue;

        pLayer->SaveJsonFile(Layer);

        // Layer �� �������� Root ������Ʈ ���� ����
        vector<CGameObject*>& vecRootObj = pLayer->GetRootObjects();
        int iObjCount = vecRootObj.size();
        Layer["ObjCount"] = iObjCount;

        string LayerName;
        LayerName += "Layer";
        LayerName += to_string(i);
        LayerName += "Info";

        // �� ��Ʈ������Ʈ ����
        for (size_t j = 0; j < vecRootObj.size(); ++j)
        {
            string str = "Object";
            str += to_string(j);
            SaveGameObjectToJson(vecRootObj[j], Layer, str);
        }
        Root[LayerName] = Layer;
    }

    string str = writer.write(Root);

    fwrite(str.c_str(), str.length(), 1, pFile);

    fclose(pFile);
}

CScene* CSceneSaveLoad::LoadSceneToJson(const wstring& _strSceneFilePath)
{
    // ���� ��ο��� ����θ� ����
    wstring strKey = CPathMgr::GetInst()->GetRelativePath(_strSceneFilePath);

    // CResMgr ���� ����θ� Ű������, CSceneFile �� ã�Ƴ�
    CResMgr::GetInst()->Load<CSceneFile>(strKey, strKey);

    // Load �� Scene ���� �� ���Ϸκ��� �ε�
    CScene* pLoadScene = new CScene;
    pLoadScene->SetResKey(strKey);

    FILE* pFile = nullptr;
    _wfopen_s(&pFile, _strSceneFilePath.c_str(), L"rb");

    assert(pFile);
    if (nullptr == pFile)
        return nullptr;

    Json::Value Root;
    string FilePath = string(_strSceneFilePath.begin(), _strSceneFilePath.end());
    std::ifstream file(FilePath, std::ifstream::binary);
    file >> Root;

    // Scene �̸� �ҷ�����
    pLoadScene->LoadJsonFromFile(Root);
    
    // Scene �� Layer ���� �ҷ�����
    for (UINT i = 0; i < MAX_LAYER; ++i)
    {
		string LayerName;
		LayerName += "Layer";
		LayerName += to_string(i);
		LayerName += "Info";
        Json::Value Layer = Root[LayerName];
        // Layer �� �̸� 
        CLayer* pLayer = pLoadScene->GetLayer(i);
        pLayer->LoadJsonFromFile(Layer);

        // Layer ���� ������Ʈ ����
        size_t iObjCount = Layer["ObjCount"].asInt();

        // Root ������Ʈ
        for (size_t j = 0; j < iObjCount; ++j)
        {
            string str = "Object";
            str += to_string(j);
            CGameObject* pLoadObj = LoadGameObjectToJson(Layer, str);
            pLoadScene->AddObject(pLoadObj, i);
        }
    }

    fclose(pFile);

    return pLoadScene;
}




void CSceneSaveLoad::SaveGameObjectToJson(CGameObject* _pObj, Json::Value& Root, const string& Str)
{
    Json::Value GameObject;
    _pObj->SaveJsonFile(GameObject);

    const vector<CScript*>& vecScript = _pObj->GetScripts();
    size_t iScriptCount = vecScript.size();
    GameObject["ScriptCount"] = iScriptCount;

    // ��ũ��Ʈ �̸�
    Json::Value Script;
    for (size_t i = 0; i < iScriptCount; ++i)
    {
        wstring wstrScriptName = (CScriptMgr::GetScriptName(vecScript[i]));
        string strScriptName = string(wstrScriptName.begin(), wstrScriptName.end());
        Script.append(strScriptName);
    }
    GameObject["ScriptName"] = Script;

    //// Child Object
    const vector<CGameObject*>& vecChild = _pObj->GetChild();
    size_t iChildCount = vecChild.size();
    GameObject["ChildCount"] = iChildCount;

    for (size_t i = 0; i < iChildCount; ++i)
    {
        string str = "Child";
        str+= to_string(i);
        SaveGameObjectToJson(vecChild[i], GameObject, str);
    }
    Root[Str] = GameObject;
}

CGameObject* CSceneSaveLoad::LoadGameObjectToJson(Json::Value& Root, const string& Str)
{
    Json::Value GameObject = Root[Str];
    CGameObject* pLoadObj = new CGameObject;

    // �̸�, ����, ������Ʈ �ҷ�����
    pLoadObj->LoadJsonFromFile(GameObject);
    int iScriptCount = GameObject["ScriptCount"].asInt();

    Json::Value Script = GameObject["ScriptName"];

    Json::ValueIterator iter = Script.begin();

    for (; iter != Script.end(); iter++)
    {
        string strScriptName = (*iter).asString();
        wstring wstrScriptName = wstring(strScriptName.begin(), strScriptName.end());
        CScript* pLoadScript = CScriptMgr::GetScript(wstrScriptName);
        //pLoadScript->LoadJsonFromFile(*iter);
        pLoadObj->AddComponent(pLoadScript);
        
    }

    size_t iChildCount = GameObject["ChildCount"].asInt();
   

    for (size_t i = 0; i < iChildCount; ++i)
    {
        string str = "Child";
        str += to_string(i);
        CGameObject* pChild = LoadGameObjectToJson(GameObject, str);
        pLoadObj->AddChild(pChild);
    }

    return pLoadObj;
}
