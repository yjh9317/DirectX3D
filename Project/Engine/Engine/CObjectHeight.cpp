#include "pch.h"
#include "CObjectHeight.h"

#include "CSceneMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CLandScape.h"

CObjectHeight::CObjectHeight()
	: CComputeShader(1, 1, 1)
	, m_pBuffer(nullptr)
{
	m_iGroupX = 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

CObjectHeight::~CObjectHeight()
{
}

void CObjectHeight::UpdateData()
{
	CGameObject* pLandScape = CSceneMgr::GetInst()->FindObjectByName(L"LandScape");
	Vec3 vLandScale = pLandScape->Transform()->GetWorldScale();
	Vec3 vLandPos = pLandScape->Transform()->GetWorldPos();
	Ptr<CTexture> HeightMap = pLandScape->LandScape()->GetHeightMap();

	Vec3 vOnPos = Vec3(-vLandPos.x + m_vPos.x, 0.f, -(-vLandPos.z - (vLandScale.z * 32) + m_vPos.z));
	Vec2 vLandScapeSize = Vec2(vLandScale.x * 32, vLandScale.z * 32);
	Vec2 vUV = Vec2(vOnPos.x / vLandScapeSize.x, vOnPos.z / vLandScapeSize.y);
	
	m_pHeightMap = HeightMap;
	m_pHeightMap->UpdateData_CS(0, true);
	m_pBuffer->UpdateData_CS(0, false);
	m_Param.v2Arr[1] = vUV;

	m_LandScapeYScale = vLandScale.y;
}

void CObjectHeight::Clear()
{

	if (nullptr != m_pHeightMap)
	{
		m_pHeightMap->ClearCS(0);
		m_pHeightMap = nullptr;
	}
	if (nullptr != m_pBuffer)
	{
		m_pBuffer->Clear();
		m_pBuffer = nullptr;
	}
}
