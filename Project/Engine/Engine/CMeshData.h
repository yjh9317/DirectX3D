#pragma once
#include "CRes.h"


#include "Ptr.h"
#include "CMaterial.h"
#include "CMesh.h"


class CMeshData :
    public CRes
{
    Ptr<CMesh>				m_pMesh;
    vector<Ptr<CMaterial>>	m_vecMtrl;

public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual int Save(const wstring& _strFilePath);
	virtual int Load(const wstring& _strFilePath);

	Ptr<CMesh> GetMesh() { return m_pMesh; }
	CGameObject* Instantiate();

	const vector<Ptr<CMaterial>>& GetMtrl() { return m_vecMtrl; }

public:
	CMeshData();
	virtual ~CMeshData();
};

