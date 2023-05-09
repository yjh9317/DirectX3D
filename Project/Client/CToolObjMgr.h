#pragma once

class CGameObjectEx;

class CToolObjMgr
	: public CSingleton<CToolObjMgr>
{
	SINGLE(CToolObjMgr);
private:
	vector<CGameObjectEx*>	m_vecObj;

public:
	void AddGameObjectEx(CGameObjectEx* pObj) { m_vecObj.push_back(pObj); }

public:
	void init();
	void progress();
};

