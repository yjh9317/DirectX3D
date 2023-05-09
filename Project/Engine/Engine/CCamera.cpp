#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CSceneMgr.h"
#include "CScene.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CTransform.h"
#include "CMeshRender.h"

#include "CMRT.h"



CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_ray{}
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_fWidth(0.f)
	, m_fAspectRatio(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fFar(10000.f)
	, m_iLayerMask(0)
	, m_iCamIdx(-1)
{
	m_fWidth = CDevice::GetInst()->GetRenderResolution().x;
	m_fAspectRatio = CDevice::GetInst()->GetRenderResolution().x / CDevice::GetInst()->GetRenderResolution().y;

	m_Frustum.m_pCam = this;
}

CCamera::CCamera(const CCamera& _origin)
	: CComponent(_origin)
	, m_eProjType(_origin.m_eProjType)
	, m_fWidth(_origin.m_fWidth)
	, m_fAspectRatio(_origin.m_fAspectRatio)
	, m_fFOV(_origin.m_fFOV)
	, m_fFar(_origin.m_fFar)
	, m_iLayerMask(_origin.m_iLayerMask)
	, m_iCamIdx(-1)
	, m_Frustum(_origin.m_Frustum)
{
	m_Frustum.m_pCam = this;
}

CCamera::~CCamera()
{

}

void CCamera::finalupdate()
{	
	finalupdate_module();
	
	m_Frustum.finalupdate();

	CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::finalupdate_module()
{
	// View ��� ���
	Vec3 vCamPos = Transform()->GetRelativePos();

	// View �̵����
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

	// View ȸ�����
	// Right, Up, Front �� �����´�.
	Matrix matViewRot = XMMatrixIdentity();

	Vec3 vRight = Transform()->GetWorldRightDir();
	Vec3 vUp = Transform()->GetWorldUpDir();
	Vec3 vFront = Transform()->GetWorldFrontDir();

	matViewRot._11 = vRight.x;	matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y;	matViewRot._22 = vUp.y;	matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z;	matViewRot._32 = vUp.z;	matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;
	m_matViewInv = XMMatrixInverse(nullptr, m_matView);

	// ������� ���
	if (PROJ_TYPE::ORTHOGRAPHIC == m_eProjType)
	{
		float fHeight = m_fWidth / m_fAspectRatio;
		m_matProj = XMMatrixOrthographicLH(m_fWidth, fHeight, 1.f, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, m_fAspectRatio, 1.f, m_fFar);
	}

	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);



	float fHeight = m_fWidth / m_fAspectRatio;
	m_Orthographic = XMMatrixOrthographicLH(m_fWidth, fHeight, 1.f, m_fFar);

	CalRay();
}


void CCamera::SortGameObject()
{
	m_vecDeferred.clear();
	m_vecDeferredDecal.clear();
	m_vecForward.clear();	
	m_vecMasked.clear();
	m_vecForwardDecal.clear();
	m_vecTranslucent.clear();
	m_vecPostProcess.clear();
	m_vecUIObject.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ī�޶� ���� ��� ���̾ �ƴϸ� continue
		if (!(m_iLayerMask & (1 << i)))
			continue;

		CLayer* pLayer = pCurScene->GetLayer(i);
		vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();
		
			if (nullptr == pRenderCom
				|| nullptr == pRenderCom->GetMesh()
				|| nullptr == pRenderCom->GetMaterial(0)
				|| nullptr == pRenderCom->GetMaterial(0)->GetShader())
			{
				continue;
			}

			// ������Ʈ�� ī�޶� �þ� �ۿ� ������ ����
			if (pRenderCom->IsFrustumCulling() 
				&& !m_Frustum.SphereCheck(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetWorldScale().x / 2.f ))
			{
				continue;
			}

			Ptr<CGraphicsShader> pShader = pRenderCom->GetMaterial(0)->GetShader();

			switch (pShader->GetShaderDomain())
			{
			case SHADER_DOMAIN::DOMAIN_DEFERRED:
				m_vecDeferred.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_DEFERRED_DECAL:
				m_vecDeferredDecal.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_FORWARD:
				m_vecForward.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_MASKED:
				m_vecMasked.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_FORWARD_DECAL:
				m_vecForwardDecal.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_TRANSLUCENT:
				m_vecTranslucent.push_back(vecObj[j]);
				break;			
			case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObj[j]);
				break;
			case SHADER_DOMAIN::DOMAIN_UI:
				m_vecUIObject.push_back(vecObj[j]);
				break;

			}
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecDynamicShadow.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

			if (pRenderCom && pRenderCom->IsDynamicShadow())
			{
				m_vecDynamicShadow.push_back(vecObj[j]);
			}
		}
	}
}

void CCamera::render_deferred()
{
	for (size_t i = 0; i < m_vecDeferred.size(); ++i)
	{
		if (m_vecDeferred[i]->IsActive())
			m_vecDeferred[i]->render();

	}
}

void CCamera::render_deferred_decal()
{
	for (size_t i = 0; i < m_vecDeferredDecal.size(); ++i)
	{
		if (m_vecDeferredDecal[i]->IsActive())
			m_vecDeferredDecal[i]->render();
	}
}

void CCamera::render_forward()
{
	for (size_t i = 0; i < m_vecForward.size(); ++i)
	{
		if(m_vecForward[i]->IsActive())
			m_vecForward[i]->render();
	}
}

void CCamera::render_forward_decal()
{
	for (size_t i = 0; i < m_vecForwardDecal.size(); ++i)
	{
		if (m_vecForwardDecal[i]->IsActive())
			m_vecForwardDecal[i]->render();
	}
}

void CCamera::render_masked()
{
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		if (m_vecMasked[i]->IsActive())
			m_vecMasked[i]->render();
	}
}

void CCamera::render_translucent()
{
	for (size_t i = 0; i < m_vecTranslucent.size(); ++i)
	{
		if (m_vecTranslucent[i]->IsActive())
			m_vecTranslucent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		if (m_vecPostProcess[i]->IsActive())
		{
			CRenderMgr::GetInst()->CopyTargetToPostProcess();
			m_vecPostProcess[i]->render();
		}
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUIObject.size(); ++i)
	{
		if (m_vecUIObject[i]->IsActive())
			m_vecUIObject[i]->render();
	}
}


void CCamera::render_shadowmap()
{
	// ���� ī�޶��� View, Proj ����
	g_transform.matView = m_matView;
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProj = m_matProj;

	for (size_t i = 0; i < m_vecDynamicShadow.size(); ++i)
	{
		if (m_vecDynamicShadow[i]->IsActive())
		{			
			m_vecDynamicShadow[i]->GetRenderComponent()->render_shadowmap();
		}
	}
}

void CCamera::SetCameraAsMain()
{
	tEventInfo tEvent = {};
	tEvent.eType = EVENT_TYPE::SET_CAMEAR_INDEX;
	tEvent.lParam = (DWORD_PTR)this;
	tEvent.wParam = 0;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CCamera::SetCameraIndex(int _iIdx)
{
	if (m_iCamIdx == _iIdx)
		return;

	tEventInfo tEvent = {};

	tEvent.eType = EVENT_TYPE::SET_CAMEAR_INDEX;
	tEvent.lParam = (DWORD_PTR)this;
	tEvent.wParam = _iIdx;

	CEventMgr::GetInst()->AddEvent(tEvent);
}

void CCamera::CheckLayerMask(int _iLayerIdx)
{
	if (m_iLayerMask & 1 << _iLayerIdx)
	{
		m_iLayerMask &= ~(1 << _iLayerIdx);
	}
	else
	{
		m_iLayerMask |= 1 << _iLayerIdx;
	}
}

void CCamera::CheckLayerMask(const wstring& _strLayerName)
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pScene->GetLayer(_strLayerName);

	CheckLayerMask(pLayer->GetLayerIdx());	
}


void CCamera::CalRay()
{
	// ���콺 ������ ���ϴ� Ray ���ϱ�
	// SwapChain Ÿ���� ViewPort ����
	CMRT* pMRT = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN);
	D3D11_VIEWPORT tVP = pMRT->GetViewPort();

	//  ���� ���콺 ��ǥ
	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();

	// ������ ī�޶��� ��ǥ�� �ݵ�� ������.
	m_ray.vStart = Transform()->GetWorldPos();

	// view space ������ ����
	m_ray.vDir.x = ((((vMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
	m_ray.vDir.y = (-(((vMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
	m_ray.vDir.z = 1.f;

	// world space ������ ����
	m_ray.vDir = XMVector3TransformNormal(m_ray.vDir, m_matViewInv);
	m_ray.vDir.Normalize();
}




void CCamera::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);

	fwrite(&m_eProjType, sizeof(UINT), 1, _pFile);

	fwrite(&m_fWidth, sizeof(float), 1, _pFile);
	fwrite(&m_fAspectRatio, sizeof(float), 1, _pFile);
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);

	fwrite(&m_iLayerMask, sizeof(UINT), 1, _pFile);
	fwrite(&m_iCamIdx, sizeof(int), 1, _pFile);
}

void CCamera::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);

	fread(&m_eProjType, sizeof(UINT), 1, _pFile);

	fread(&m_fWidth, sizeof(float), 1, _pFile);
	fread(&m_fAspectRatio, sizeof(float), 1, _pFile);
	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);

	fread(&m_iLayerMask, sizeof(UINT), 1, _pFile);
	fread(&m_iCamIdx, sizeof(int), 1, _pFile);
}

void CCamera::SaveJsonFile(Json::Value& root)
{
	Json::Value CameraInfo;

	CameraInfo["ProjType"] = (UINT)m_eProjType;
	CameraInfo["Width"] = m_fWidth;
	CameraInfo["FOV"] = m_fFOV;
	CameraInfo["Far"] = m_fFar;

	CameraInfo["LayerMask"] = m_iLayerMask;
	CameraInfo["CameraIndex"] = m_iCamIdx;

	root["CAMERA"] = CameraInfo;
}

void CCamera::LoadJsonFromFile(Json::Value& root)
{
	Json::Value CameraInfo = root;

	m_eProjType = (PROJ_TYPE)CameraInfo["ProjType"].asInt();
	m_fWidth = CameraInfo["Width"].asFloat();
	m_fFOV = CameraInfo["FOV"].asFloat();
	m_fFar = CameraInfo["Far"].asFloat();

	m_iLayerMask = CameraInfo["LayerMask"].asUInt();
	m_iCamIdx = CameraInfo["CameraIndex"].asInt();
}
