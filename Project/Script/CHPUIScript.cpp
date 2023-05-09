#include "pch.h"
#include "CHPUIScript.h"

#include <Engine/CCollider3D.h>

CHPUIScript::CHPUIScript()
	:CScript((int)SCRIPT_TYPE::HPUISCRIPT)
	, m_fHPRatio(1.f)
{
}

CHPUIScript::~CHPUIScript()
{
}

void CHPUIScript::start()
{
	CGameObject* pObj = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");
	if (nullptr != pObj)
	{
		m_pCamera = pObj;
	}
}

void CHPUIScript::update()
{
	//CGameObject* pCamera = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");

	Vec3 vCameraPos = m_pCamera->Transform()->GetWorldPos();

	Vec3 vPos = Transform()->GetWorldPos();

	Vec3 vDir = vCameraPos - vPos;
	vDir.Normalize();

	Vec3 vUpDir = Vec3(0.f,1.f,0.f);

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3d9/d3dxmatrixlookatlh

	Vec3 zAxis = vDir;
	Vec3 xAxis = vUpDir.Cross(zAxis).Normalize();
	Vec3 yAxis = zAxis.Cross(xAxis);
	
	Matrix Mat = {};
	Mat._11 = xAxis.x;		Mat._21 = yAxis.x;	  	Mat._31 = zAxis.x;
	Mat._12 = xAxis.y;		Mat._22 = yAxis.y;	  	Mat._32 = zAxis.y;
	Mat._13 = xAxis.z;		Mat._23 = yAxis.z;	  	Mat._33 = zAxis.z;
	Mat._14 = -xAxis.Dot(vCameraPos); Mat._24 = -yAxis.Dot(vCameraPos); Mat._34 = -zAxis.Dot(vCameraPos); Mat._44 = 1;
	

	Vec3 Result = DecomposeRotMat(Mat);

	Transform()->SetRelativeRotation(Result);

	if (GetOwner()->GetParent())
	{
		CGameObject* pObject = GetOwner()->GetParent();
		Vec3 _vPos = pObject->Transform()->GetRelativePos();
		Vec3 vColPos = pObject->Collider3D()->GetOffsetPos();
		Vec3 vParentScale = pObject->Collider3D()->GetWorldScale();
		
		_vPos.y += vColPos.y/2.f + vParentScale.y;
		Transform()->SetRelativePos(_vPos);
	}
	float f = 1 - m_fHPRatio;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &f);
	//MeshRender()->GetMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &f);

}

