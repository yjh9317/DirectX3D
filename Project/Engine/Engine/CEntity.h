#pragma once


class CEntity
{
private:
	static UINT g_iNextID;

private:
	const UINT	m_iID;
	wstring		m_strName;
	
public:
	void SetName(const wstring& _name){m_strName = _name;}
	const wstring& GetName(){return m_strName;}
	UINT GetID() { return m_iID; }

public:
	virtual void UpdateData() {}
	virtual CEntity* Clone() = 0;

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	virtual void SaveJsonFile(Json::Value& root);
	virtual void LoadJsonFromFile(Json::Value& root);

	// ComponentClass에서 Json배열을 이용할 때 저장,불러오기할 이름을 전달
	virtual void SaveJsonFile(Json::Value& root, const string& str) {};
	virtual void LoadJsonFromFile(Json::Value& root,const string& str) {};

public:
	CEntity();
	CEntity(const CEntity& _origin);
	virtual ~CEntity();
};

