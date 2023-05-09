#pragma once

class CScene;
class CGameObject;
class CPrefab;

class CSceneSaveLoad
{
public:
	// Save
	static void SaveScene(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);
	static void SavePrefab(CPrefab* _Prefab, const wstring& _strFilePath);

	static void SaveSceneToJson(CScene* _pScene, const wstring& _strSceneFilePath);
	static void SaveGameObjectToJson(CGameObject* _pObj, Json::Value& Root,const string& Str);


	// Load
	static CScene* LoadScene(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObject(FILE* _pFile);
	static int LoadPrefab(CPrefab* _Prefab, const wstring& _strFilePath);

	static CScene* LoadSceneToJson(const wstring& _strSceneFilePath);
	static CGameObject* LoadGameObjectToJson(Json::Value& Root, const string& Str);
};

