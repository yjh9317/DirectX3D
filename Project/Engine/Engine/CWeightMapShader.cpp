#include "pch.h"
#include "CWeightMapShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

CWeightMapShader::CWeightMapShader()
	: CComputeShader(32, 32, 1)
	, m_pWeightMap(nullptr)
	, m_pWeightTex(nullptr)
	, m_pRaycastInfo(nullptr)
	, m_iBrushIdx(0)
	, m_iWeightIdx(0)
	, m_iWidth(0)
	, m_iHeight(0)
{
	
}

CWeightMapShader::~CWeightMapShader()
{

}

void CWeightMapShader::UpdateData()
{
	m_Param.iArr[0] = (int)m_iWidth;
	m_Param.iArr[1] = (int)m_iHeight;
	m_Param.iArr[2] = m_iBrushIdx;
	m_Param.iArr[3] = m_iWeightIdx;
	m_Param.v2Arr[0] = m_vBrushScale;

	m_Param.fArr[0] = m_bSave;
	m_Param.fArr[1] = m_bLoad;
	
	m_pWeightMap->UpdateData_CS(0, false);

	m_pWeightTex->UpdateData_CS(1, false);

	m_pBrushArrTex->UpdateData_CS(8, true);	

	m_pRaycastInfo->UpdateData_CS(16, true);

	// �׷� �� ���
	m_iGroupX = (m_iWidth / m_iGroupPerThreadCountX) + 1;
	m_iGroupY = (m_iHeight / m_iGroupPerThreadCountY) + 1;
	m_iGroupZ = 1;
}

void CWeightMapShader::Clear()
{
	if (nullptr != m_pWeightMap)
	{
		m_pWeightMap->Clear();
		m_pWeightMap = nullptr;
	}

	if (nullptr != m_pWeightTex)
	{
		m_pWeightTex->ClearCS(1);
		m_pWeightTex = nullptr;
	}

	if (nullptr != m_pBrushArrTex)
	{
		m_pBrushArrTex->Clear(0);
		m_pBrushArrTex = nullptr;
	}

	if (nullptr != m_pRaycastInfo)
	{
		m_pRaycastInfo->Clear();
		m_pRaycastInfo = nullptr;
	}

	if (m_bSave)
	{
		m_bSave = false;
	}
	if(m_bLoad)
	{
		m_bLoad = false;
	}
}

