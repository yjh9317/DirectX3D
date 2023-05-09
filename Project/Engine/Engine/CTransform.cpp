#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"



CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_arrRelativeDir{}
	, m_arrWorldDir{}
	, m_bIgnoreParentScale(false)
	, m_bIgnoreTransform(false)
{
}

CTransform::~CTransform()
{
}

void CTransform::finalupdate()
{
	// 크기 x 회전(자전) x 이동
	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);
	Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRotX = XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotY = XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRot.z);
	m_matRelativeRot = matRotX * matRotY * matRotZ;

	// World 행렬 만들기
	m_matWorld = matScale * m_matRelativeRot * matTranslation;

	// RelativeDir 구하기
	static Vec3 vAxis[(UINT)DIR_TYPE::END] = { Vec3::Right, Vec3::Up, Vec3::Front };

	for (int i = 0; i < (int)DIR_TYPE::END; ++i)
	{	
		//vAxis[i](회전하지 않았을 때의 기저축) x 회전행렬
		m_arrWorldDir[i] = m_arrRelativeDir[i] = XMVector3TransformNormal(vAxis[i], m_matRelativeRot);
	}


	if (GetOwner()->GetParent())
	{		
		Matrix matParentWorld = GetOwner()->GetParent()->Transform()->GetWorldMat();

		if (m_bIgnoreParentScale)
		{
			Vec3 vParentWorldScale = GetOwner()->GetParent()->Transform()->GetWorldScale();
			Matrix matParentScale = XMMatrixScaling(vParentWorldScale.x, vParentWorldScale.y, vParentWorldScale.z);
			Matrix matParentScaleInv = XMMatrixInverse(nullptr, matParentScale);

			// 크기 * 회전 * 이동 (크기가 가장 먼저이기때문에 크기 역행렬을 곱해도 상관x)
			m_matWorld = m_matWorld * matParentScaleInv * matParentWorld;
		}
		else if (m_bIgnoreTransform)
		{

		}
		else
		{
			m_matWorld *= matParentWorld;
		}

		// World Dir 구하기
		for (int i = 0; i < (int)DIR_TYPE::END; ++i)
		{
			m_arrWorldDir[i] = XMVector3TransformNormal(m_arrRelativeDir[i], matParentWorld);
			m_arrWorldDir[i].Normalize();
		}
	}

	// 월드 역행렬
	m_matWorldInv = XMMatrixInverse(nullptr, m_matWorld);
}

void CTransform::finalupdate_module()
{
	finalupdate();
}

void CTransform::active()
{
	CComponent::active();
	assert(nullptr);
}

void CTransform::deactive()
{
	CComponent::deactive();
	assert(nullptr);
}


Vec3 CTransform::GetWorldScale()
{
	Vec3 vWorldScale = m_vRelativeScale;

	CGameObject* pParent = GetOwner()->GetParent();
	if (m_bIgnoreParentScale)
		pParent = nullptr;

	while (pParent)
	{
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		bool bIgnoreParentScale = pParent->Transform()->m_bIgnoreParentScale;
		pParent = pParent->GetParent();

		if (bIgnoreParentScale)
			pParent = nullptr;
	}

	return vWorldScale;
}

Matrix CTransform::GetWorldRotation()
{
	Matrix matWorldRot = m_matRelativeRot;

	CGameObject* pParent = GetOwner()->GetParent();

	while (pParent)
	{
		matWorldRot *= pParent->Transform()->m_matRelativeRot;
		pParent = pParent->GetParent();
	}

	return matWorldRot;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;
	g_transform.matWorldInv = m_matWorldInv;

	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	

	CConstBuffer* pBuffer = CDevice::GetInst()->GetCB(CB_TYPE::TRANSFORM);
	pBuffer->SetData(&g_transform, sizeof(tTransform));
	pBuffer->UpdateData();
}

void CTransform::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vRelativeRot, sizeof(Vec3), 1, _pFile);
	fwrite(&m_bIgnoreParentScale, sizeof(bool), 1, _pFile);	
	fwrite(&m_bIgnoreTransform, sizeof(bool), 1, _pFile);
}

void CTransform::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_vRelativePos, sizeof(Vec3), 1, _pFile);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vRelativeRot, sizeof(Vec3), 1, _pFile);
	fread(&m_bIgnoreParentScale, sizeof(bool), 1, _pFile);
	fread(&m_bIgnoreTransform, sizeof(bool), 1, _pFile);
}

void CTransform::SaveJsonFile(Json::Value& root)
{
	Json::Value TransformInfo;

	TransformInfo["RelativePos"].append(m_vRelativePos.x);
	TransformInfo["RelativePos"].append(m_vRelativePos.y);
	TransformInfo["RelativePos"].append(m_vRelativePos.z);

	TransformInfo["RelativeScale"].append(m_vRelativeScale.x);
	TransformInfo["RelativeScale"].append(m_vRelativeScale.y);
	TransformInfo["RelativeScale"].append(m_vRelativeScale.z);

	TransformInfo["RelativeRotation"].append(m_vRelativeRot.x);
	TransformInfo["RelativeRotation"].append(m_vRelativeRot.y);
	TransformInfo["RelativeRotation"].append(m_vRelativeRot.z);

	TransformInfo["IsIgnoreParentScale"] = m_bIgnoreParentScale;
	TransformInfo["IsIgnoreParentTransform"] = m_bIgnoreTransform;

	root["TRANSFORM"] = TransformInfo;
}

void CTransform::LoadJsonFromFile(Json::Value& root)
{
	Json::Value TransformInfo = root;

	int size = TransformInfo.size();

	Json::Value tmp = TransformInfo["RelativePos"];
	LoadVecFromJson(tmp, m_vRelativePos);

	tmp = TransformInfo["RelativeScale"];
	LoadVecFromJson(tmp, m_vRelativeScale);

	tmp = TransformInfo["RelativeRotation"];
	LoadVecFromJson(tmp, m_vRelativeRot);

	m_bIgnoreParentScale = TransformInfo["IsIgnoreParentScale"].asBool();
	m_bIgnoreTransform = TransformInfo["IsIgnoreParentTransform"].asBool();
}
