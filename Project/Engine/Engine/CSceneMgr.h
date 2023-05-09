#pragma once

class CScene;
class CGameObject;

class CSceneMgr
	: public CSingleton<CSceneMgr>
{	
	friend class CSingleton<CSceneMgr>;
private:
	CSceneMgr();
	~CSceneMgr();

private:
	CScene*		m_pCurScene;

	CScene*		m_pAnimScene;


public:
	CScene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(CScene* _pNextScene);

	void SpawnObject(CGameObject* _pSpawnObject, Vec3 _vWorldPos, wstring _strName, UINT _iLayerIdx);
	void SpawnObject(CGameObject* _pSpawnObject, UINT _iLayerIdx);
	void AddChild(CGameObject* _pParentObject, CGameObject* _pChildObject);	
	void DisconnectParent(CGameObject* _pObject);
	void ClearLayer();

	CGameObject* FindObjectByName(const wstring& _strName);
	void FindObjectsByName(wstring& _strName, vector<CGameObject*>& _vecOut);

	void SetAnimScene(CScene* _AnimScene) { m_pAnimScene = _AnimScene; }

public:
	void init();
	void progress();	

};

