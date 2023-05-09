#include "pch.h"
#include "CCollider3D.h"

#include "CTransform.h"
#include "CConstBuffer.h"
#include "CScript.h"
#include "CSocket.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_eColliderType(COLLIDER3D_TYPE::CUBE)
	, m_vOffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_vOffsetScale(Vec3(1.f, 1.f, 1.f))
	, m_iCollisionCount(0)
{
	// Collider3D 모양에 맞는 메쉬 참조
	SetCollider3DType(m_eColliderType);

	// Collider3D 전용 재질 참조	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl");
}

CCollider3D::CCollider3D(const CCollider3D& _Origin)
	: CComponent(_Origin)
	, m_eColliderType(_Origin.m_eColliderType)
	, m_vOffsetPos(_Origin.m_vOffsetPos)
	, m_vOffsetScale(_Origin.m_vOffsetScale)
	, m_iCollisionCount(0)
{
	// Collider3D 모양에 맞는 메쉬 참조
	SetCollider3DType(m_eColliderType);

	// Collider3D 전용 재질 참조	
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Collider2DMtrl.mtrl");

}

CCollider3D::~CCollider3D()
{

}


void CCollider3D::SetCollider3DType(COLLIDER3D_TYPE _type)
{
	m_eColliderType = _type;

	if (COLLIDER3D_TYPE::CUBE == m_eColliderType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	}
	else if(COLLIDER3D_TYPE::SPHERE == m_eColliderType)
	{
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
	}
	
}

void CCollider3D::SetOffsetScale(Vec3 _vOffsetScale)
{
	m_vOffsetScale = _vOffsetScale;

	if (COLLIDER3D_TYPE::SPHERE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}

void CCollider3D::SetOffsetScale(float _x, float _y, float _z)
{
	m_vOffsetScale = Vec3(_x, _y, _z);

	if (COLLIDER3D_TYPE::SPHERE == m_eColliderType)
	{
		m_vOffsetScale.y = m_vOffsetScale.x;
	}
}



void CCollider3D::finalupdate()
{
	Matrix matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColWorld = matScale * matTrans;

	Vec3 vObjScale = Transform()->GetWorldScale();
	Matrix matObjScaleInv = XMMatrixInverse(nullptr, XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z));

	// 충돌체 상대행렬 * 오브젝트 월드 크기 역행렬(크기^-1) * 오브젝트 월드 행렬(크기 * 회전 * 이동)
	m_matColWorld = m_matColWorld * matObjScaleInv * Transform()->GetWorldMat();

	if (Socket())
	{
		m_matColWorld = Socket()->GetFinalMatrix();
	}
}

void CCollider3D::UpdateData()
{
	// 소켓에 해당하는 오브젝트가 부모 오브젝트까지 있다면 해당 Socket의 월드행렬을 Collider 월드행렬로 전달
	if (GetOwner()->Socket() && GetOwner()->GetParent())
	{
		Matrix OwnerWorldMat = Transform()->GetWorldMat();

		CStructuredBuffer* pSocketBuffer = GetOwner()->GetParent()->Animator3D()->GetSocket();
		Matrix BoneMat = {};
		pSocketBuffer->GetData(&BoneMat);
		Matrix SocketMat = Socket()->GetFinalMatrix();

		g_transform.matWorld = SocketMat;
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
		g_transform.matWVP = g_transform.matWV * g_transform.matProj;

		m_matColWorld = SocketMat;
	}
	else	// 없으면 기존 행렬 사용
	{
		g_transform.matWorld = m_matColWorld;
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
		g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	}

	CConstBuffer* pCB = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pCB->SetData(&g_transform, sizeof(tTransform));
	pCB->UpdateData();
}

void CCollider3D::render()
{
	UpdateData();

	m_pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iCollisionCount);
	m_pMtrl->UpdateData();

	m_pMesh->render(0);
}

void CCollider3D::OnCollisionEnter(CCollider3D* _Other)
{
	++m_iCollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionEnter(_Other->GetOwner());
	}
}

void CCollider3D::OnCollision(CCollider3D* _Other)
{
	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollision(_Other->GetOwner());
	}
}

void CCollider3D::OnCollisionExit(CCollider3D* _Other)
{
	--m_iCollisionCount;

	const vector<CScript*>& vecScript = GetOwner()->GetScripts();
	for (size_t i = 0; i < vecScript.size(); ++i)
	{
		vecScript[i]->OnCollisionExit(_Other->GetOwner());
	}
}

void CCollider3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
}

void CCollider3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_eColliderType, sizeof(UINT), 1, _pFile);
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);

	SetCollider3DType(m_eColliderType);
}

void CCollider3D::SaveJsonFile(Json::Value& root)
{
	Json::Value Collider3DInfo;

	Collider3DInfo["ColliderType"] = (UINT)m_eColliderType;

	Collider3DInfo["OffSetPos"].append(m_vOffsetPos.x);
	Collider3DInfo["OffSetPos"].append(m_vOffsetPos.y);
	Collider3DInfo["OffSetPos"].append(m_vOffsetPos.z);

	Collider3DInfo["OffSetScale"].append(m_vOffsetScale.x);
	Collider3DInfo["OffSetScale"].append(m_vOffsetScale.y);
	Collider3DInfo["OffSetScale"].append(m_vOffsetScale.z);


	root["COLLIDER3D"] = Collider3DInfo;
}

void CCollider3D::LoadJsonFromFile(Json::Value& root)
{
	Json::Value Collider3DInfo = root;

	m_eColliderType = (COLLIDER3D_TYPE)Collider3DInfo["ColliderType"].asInt();

	Json::Value tmp = Collider3DInfo["OffSetPos"];
	LoadVecFromJson(tmp, m_vOffsetPos);

	tmp = Collider3DInfo["OffSetScale"];
	LoadVecFromJson(tmp, m_vOffsetScale);
}

