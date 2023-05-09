#include "pch.h"
#include "CLight3D.h"

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightInfo{}
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = new CGameObject;
	m_pLightCam->AddComponent(new CTransform);
	m_pLightCam->AddComponent(new CCamera);
}

CLight3D::CLight3D(const CLight3D& _origin)
	: CComponent(_origin)
	, m_LightInfo(_origin.m_LightInfo)
	, m_iLightIdx(-1)
	, m_pLightCam(nullptr)
{
	m_pLightCam = _origin.m_pLightCam->Clone();	
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_pLightCam);
}

void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_LightInfo.iLightType = (int)_eType;

	switch (_eType)
	{
	case LIGHT_TYPE::DIRECTIONAL:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DirLightMtrl.mtrl");
		m_pLightCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pLightCam->Camera()->SetWidth(4096.f);
		m_pLightCam->Camera()->SetAspectRatio(1.f);
		m_pLightCam->Camera()->SetFar(100000.f);
		break;
	case LIGHT_TYPE::POINT:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\PointLightMtrl.mtrl");
		break;
	case LIGHT_TYPE::SPOT:
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");

		break;	
	}
}

void CLight3D::SetLightDir(Vec3 _vDir)
{
	m_LightInfo.vLightDir = _vDir;
	m_LightInfo.vLightDir.Normalize();

	Vec3 vFront = m_LightInfo.vLightDir;
	Vec3 vUp = Vec3(0.f, 1.f, 0.f);
	Vec3 vRight = XMVector3Cross(vUp, vFront);
	vRight.Normalize();
	vUp = XMVector3Cross(vFront, vRight);
	vUp.Normalize();

	Matrix matRot = XMMatrixIdentity();
	matRot._11 = vRight.x;	matRot._12 = vRight.y;	matRot._13 = vRight.z;
	matRot._21 = vUp.x;		matRot._22 = vUp.y;		matRot._23 = vUp.z;
	matRot._31 = vFront.x;	matRot._32 = vFront.y;	matRot._33 = vFront.z;
	
	Vec3 vRot = DecomposeRotMat(matRot);

	// 광원이 가리키는 방향이 Transform 의 Front 가 되도록 회전시켜준다.
	Transform()->SetRelativeRotation(vRot);
}

void CLight3D::SetRange(float _fRange)
{
	m_LightInfo.fRange = _fRange;
	Transform()->SetRelativeScale(_fRange * 2.f, _fRange * 2.f, _fRange * 2.f);
}

void CLight3D::update()
{
}

void CLight3D::finalupdate()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();
	m_LightInfo.vLightDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	m_LightInfo.fRange = Transform()->GetWorldScale().x / 2.f;

	m_iLightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);


	// 광원의 카메라도 광원과 동일한 Transform 이 되도록 업데이트 한다.
	m_pLightCam->Transform()->SetRelativePos(Transform()->GetWorldPos());
	m_pLightCam->Transform()->SetRelativeRotation(DecomposeRotMat(Transform()->GetWorldRotation()));
	m_pLightCam->finalupdate_module();
}

void CLight3D::render()
{
	m_pLightMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &m_iLightIdx);	

	// 방향성 광원인 경우 그림자 처리를 위해서 광원카메라로 투영시킬 수 있게 View * Proj 행렬을 전달
	if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_LightInfo.iLightType)
	{
		Matrix matLightVP = m_pLightCam->Camera()->GetViewMat() * m_pLightCam->Camera()->GetProjMat();
		m_pLightMtrl->SetScalarParam(SCALAR_PARAM::MAT_0, &matLightVP);
	}

	Transform()->UpdateData();	
	m_pLightMtrl->UpdateData();

	m_pVolumeMesh->render(0);
}

void CLight3D::render_shadowmap()
{
	m_pLightCam->Camera()->SortShadowObject();
	m_pLightCam->Camera()->render_shadowmap();
}

void CLight3D::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);

	SaveResPtr(m_pVolumeMesh,_pFile);
	SaveResPtr(m_pLightMtrl,_pFile);
}

void CLight3D::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_LightInfo, sizeof(tLightInfo), 1, _pFile);

	LoadResPtr(m_pVolumeMesh, _pFile);
	LoadResPtr(m_pLightMtrl, _pFile);
}

void CLight3D::SaveJsonFile(Json::Value& root)
{
	Json::Value Light3DInfo;

	Vec4 vAmb = m_LightInfo.color.vAmb;
	Light3DInfo["Amb"].append(vAmb.x);
	Light3DInfo["Amb"].append(vAmb.y);
	Light3DInfo["Amb"].append(vAmb.z);
	Light3DInfo["Amb"].append(vAmb.w);

	Vec4 vDif = m_LightInfo.color.vDiff;
	Light3DInfo["Diff"].append(vDif.x);
	Light3DInfo["Diff"].append(vDif.y);
	Light3DInfo["Diff"].append(vDif.z);
	Light3DInfo["Diff"].append(vDif.w);

	Vec4 vSpec = m_LightInfo.color.vSpec;
	Light3DInfo["Spec"].append(vSpec.x);
	Light3DInfo["Spec"].append(vSpec.y);
	Light3DInfo["Spec"].append(vSpec.z);
	Light3DInfo["Spec"].append(vSpec.w);

	Light3DInfo["Angle"] = m_LightInfo.fAngle;
	Light3DInfo["Range"] = m_LightInfo.fRange;

	Light3DInfo["LightType"] = m_LightInfo.iLightType;

	Vec3 vLightDir = m_LightInfo.vLightDir;
	Light3DInfo["LightDir"].append(vLightDir.x);
	Light3DInfo["LightDir"].append(vLightDir.y);
	Light3DInfo["LightDir"].append(vLightDir.z);

	Vec3 vWorldPos = m_LightInfo.vWorldPos;
	Light3DInfo["WorldPos"].append(vWorldPos.x);
	Light3DInfo["WorldPos"].append(vWorldPos.y);
	Light3DInfo["WorldPos"].append(vWorldPos.z);

	SaveJsonRes(m_pVolumeMesh,"VolumeMesh",&Light3DInfo);
	SaveJsonRes(m_pLightMtrl,"LightMtrl", &Light3DInfo);

	root["LIGHT3D"] = Light3DInfo;

}

void CLight3D::LoadJsonFromFile(Json::Value& root)
{
	Json::Value Light3DInfo = root;

	Json::Value tmp = Light3DInfo["Amb"];
	LoadVecFromJson(tmp, m_LightInfo.color.vAmb);

	tmp = Light3DInfo["Diff"];
	LoadVecFromJson(tmp, m_LightInfo.color.vDiff);

	tmp = Light3DInfo["Spec"];
	LoadVecFromJson(tmp, m_LightInfo.color.vSpec);

	m_LightInfo.fAngle = Light3DInfo["Angle"].asFloat();
	m_LightInfo.fRange = Light3DInfo["Range"].asFloat();
	m_LightInfo.iLightType = Light3DInfo["LightType"].asInt();

	tmp = Light3DInfo["LightDir"];
	LoadVecFromJson(tmp, m_LightInfo.vLightDir);

	tmp = Light3DInfo["WorldPos"];
	LoadVecFromJson(tmp, m_LightInfo.vWorldPos);

	LoadJsonRes(m_pVolumeMesh, &(root["VolumeMesh"]));
	LoadJsonRes(m_pLightMtrl, &(root["LightMtrl"]));
}
