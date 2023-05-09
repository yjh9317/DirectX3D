#include "pch.h"
#include "CRenderComponent.h"

#include "CSceneMgr.h"
#include "CScene.h"

#include "CTransform.h"
#include "CAnimator3D.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _type)
	: CComponent(_type)
	, m_pMesh(nullptr)	
	, m_bDynamicShadow(false)
	, m_bFrustumCulling(false)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _origin)
	: CComponent(_origin)
	, m_pMesh(_origin.m_pMesh)		
	, m_bDynamicShadow(_origin.m_bDynamicShadow)
	, m_bFrustumCulling(_origin.m_bFrustumCulling)
{
	if (false == _origin.m_vecMtrls.empty())
	{
		SetMesh(m_pMesh);
		for (size_t i = 0; i < _origin.m_vecMtrls.size(); ++i)
		{
			SetSharedMaterial(_origin.m_vecMtrls[i].pSharedMtrl, i);
		}		
	}
}

CRenderComponent::~CRenderComponent()
{
	for (size_t i = 0; i < m_vecMtrls.size(); ++i)
	{
		if (nullptr != m_vecMtrls[i].pDynamicMtrl)
			delete  m_vecMtrls[i].pDynamicMtrl.Get();
	}	
}

void CRenderComponent::SetMesh(Ptr<CMesh> _pMesh)
{
	m_pMesh = _pMesh;

	if (!m_vecMtrls.empty())
	{
		m_vecMtrls.clear();
		vector<tMtrlSet> vecMtrls;
		m_vecMtrls.swap(vecMtrls);
	}

	m_vecMtrls.resize(m_pMesh->GetSubsetCount());
}

void CRenderComponent::SetSharedMaterial(Ptr<CMaterial> _pMtrl, UINT _iIdx)
{
	m_vecMtrls[_iIdx].pSharedMtrl = _pMtrl;
	m_vecMtrls[_iIdx].pMtrl = _pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetMaterial(UINT _iIdx)
{
	if (nullptr == m_vecMtrls[_iIdx].pMtrl)
	{
		m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pSharedMtrl;
	}

	return m_vecMtrls[_iIdx].pMtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial(UINT _iIdx)
{
	m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pSharedMtrl;

	return m_vecMtrls[_iIdx].pSharedMtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial(UINT _iIdx)
{
	// Play 모드에서만 동작가능
	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() != SCENE_STATE::PLAY)
		return nullptr;

	if (nullptr != m_vecMtrls[_iIdx].pDynamicMtrl && m_vecMtrls[_iIdx].pDynamicMtrl->GetMasterMtrl() != m_vecMtrls[_iIdx].pSharedMtrl)
	{
		CMaterial* pMtrl = m_vecMtrls[_iIdx].pDynamicMtrl.Get();
		m_vecMtrls[_iIdx].pDynamicMtrl = nullptr;
		delete pMtrl;
	}

	if (nullptr == m_vecMtrls[_iIdx].pDynamicMtrl)
	{
		m_vecMtrls[_iIdx].pDynamicMtrl = m_vecMtrls[_iIdx].pSharedMtrl->GetMtrlInst();
	}

	m_vecMtrls[_iIdx].pMtrl = m_vecMtrls[_iIdx].pDynamicMtrl;

	return m_vecMtrls[_iIdx].pMtrl;
}



void CRenderComponent::render_shadowmap()
{
	Transform()->UpdateData();

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ShadowMap.mtrl");
	CAnimator3D* pAnim = GetOwner()->Animator3D();

	if (pAnim)
	{
		pAnim->UpdateData();
		pMtrl->SetAnim3D(true);
		pMtrl->SetBoneCount(Animator3D()->GetBoneCount());
	}
	else
	{
		pMtrl->SetAnim3D(false);
		pMtrl->SetBoneCount(0);
	}
	pMtrl->UpdateData();
	
	m_pMesh->render(0);

	if (pAnim)
	{
		pAnim->ClearData();
	}
}

void CRenderComponent::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	UINT iMtrlCount = GetMtrlCount();
	fwrite(&iMtrlCount, sizeof(UINT), 1, _pFile);

	SaveResPtr(m_pMesh, _pFile);


	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		SaveResPtr(m_vecMtrls[i].pSharedMtrl, _pFile);
	}

	fwrite(&m_bDynamicShadow, 1, 1, _pFile);
	fwrite(&m_bFrustumCulling, 1, 1, _pFile);
}

void CRenderComponent::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	UINT iMtrlCount = GetMtrlCount();
	fread(&iMtrlCount, sizeof(UINT), 1, _pFile);

	LoadResPtr(m_pMesh, _pFile);

	wstring wMeshData = { };
	wMeshData += L"meshdata\\";
	wMeshData +=path(m_pMesh->GetName()).stem();
	wMeshData += L".mdat";

	
	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(wMeshData);
	if (nullptr != pMeshData)
	{
		m_pMesh = pMeshData->GetMesh();

		CAnimator3D* pAnim = GetOwner()->Animator3D();
		if (pAnim)
		{
			if (m_pMesh->IsAnimMesh())
			{
				GetOwner()->Animator3D()->SetBones(m_pMesh->GetBones());
				GetOwner()->Animator3D()->SetAnimClip(m_pMesh->GetAnimClip());
			}
		}
	}

	
	m_vecMtrls.resize(iMtrlCount);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		Ptr<CMaterial> pMtrl;
		LoadResPtr(pMtrl, _pFile);
		SetSharedMaterial(pMtrl, i);		
	}

	fread(&m_bDynamicShadow, 1, 1, _pFile);
	fread(&m_bFrustumCulling, 1, 1, _pFile);
}

void CRenderComponent::SaveJsonFile(Json::Value& root)
{
	Json::Value MeshRenderInfo;

	UINT iMtrlCount = GetMtrlCount();
	MeshRenderInfo["MtrlCount"] = iMtrlCount;

	SaveJsonRes(m_pMesh,"Mesh", &MeshRenderInfo);

	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		string str = "Materail" + std::to_string(i);
		SaveJsonRes(m_vecMtrls[i].pSharedMtrl, str, &MeshRenderInfo);
	}

	MeshRenderInfo["DynamicShadow"] = m_bDynamicShadow;
	MeshRenderInfo["FrustumCulling"] = m_bFrustumCulling;

	root["MESHRENDER"] = MeshRenderInfo;
}

void CRenderComponent::LoadJsonFromFile(Json::Value& root)
{
	Json::Value MeshRenderInfo = root;
	
	UINT iMtrlCount = MeshRenderInfo["MtrlCount"].asInt();

	LoadJsonRes(m_pMesh, &(MeshRenderInfo["Mesh"]));

	m_vecMtrls.resize(iMtrlCount);
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		string str = "Materail" + std::to_string(i);
		LoadJsonRes(m_vecMtrls[i].pSharedMtrl, &(MeshRenderInfo[str]));
	}

	wstring wMeshData = { };
	wMeshData += L"meshdata\\";
	wMeshData += path(m_pMesh->GetName()).stem();
	wMeshData += L".mdat";


	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->FindRes<CMeshData>(wMeshData);
	if (nullptr != pMeshData)
	{
		m_pMesh = pMeshData->GetMesh();

		CAnimator3D* pAnim = GetOwner()->Animator3D();
		if (pAnim)
		{
			if (m_pMesh->IsAnimMesh())
			{
				GetOwner()->Animator3D()->SetBones(m_pMesh->GetBones());
				GetOwner()->Animator3D()->SetAnimClip(m_pMesh->GetAnimClip());
			}
		}
	}

	m_bDynamicShadow = MeshRenderInfo["DynamicShadow"].asBool();
	m_bFrustumCulling = MeshRenderInfo["FrustumCulling"].asBool();

}