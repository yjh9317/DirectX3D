#include "pch.h"
#include "CBounding.h"

#include "CTransform.h"
#include "CMeshRender.h"
#include "CCollider3D.h"
#include "CEventMgr.h"

CBounding::CBounding()
	:CComponent(COMPONENT_TYPE::BOUNDING)
	, m_vScale(Vec3(0.f,0.f,0.f))
{
	m_pDebugObject = new CGameObject;
	m_pDebugObject->AddComponent(new CTransform);
	m_pDebugObject->AddComponent(new CMeshRender);
	m_pDebugObject->AddComponent(new CCollider3D);

	m_pDebugObject->Transform()->SetIgnoreParentScale(true);

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl");

	m_pDebugObject->MeshRender()->SetMesh(pMesh);
	m_pDebugObject->MeshRender()->SetSharedMaterial(pMtrl, 0);

	m_pDebugObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::SPHERE);
	
}

CBounding::~CBounding()
{
}


void CBounding::finalupdate()
{
	//Vec3 vPos = Transform()->GetRelativePos();
	//vPos.y += Transform()->GetRelativeScale().y / 2.f;
	//m_pDebugObject->Transform()->SetRelativePos(vPos);
	//m_pDebugObject->Collider3D()->SetOffsetScale(Transform()->GetWorldScale());

	//int iCount = m_pDebugObject->Collider3D()->m_iCollisionCount;
	//m_pDebugObject->MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &iCount);

	//if (-1 == m_pDebugObject->GetLayerIndex())
	//{
	//	tEventInfo info;
	//	
	//	m_pDebugObject->SetName(GetOwner()->GetName() + L"BB");

	//	info.eType = EVENT_TYPE::CREATE_OBJ;
	//	info.lParam = (DWORD_PTR)m_pDebugObject;
	//	info.wParam = (DWORD_PTR)GetOwner()->GetLayerIndex();
	//	CEventMgr::GetInst()->AddEvent(info);

	//	info.eType = EVENT_TYPE::ADD_CHILD;
	//	info.lParam = (DWORD_PTR)GetOwner();
	//	info.wParam = (DWORD_PTR)m_pDebugObject;
	//	CEventMgr::GetInst()->AddEvent(info);
	//}
}

void CBounding::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_vScale, sizeof(Vec3), 1, _pFile);
}

void CBounding::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_vScale, sizeof(Vec3), 1, _pFile);
}

