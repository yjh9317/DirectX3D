#include "pch.h"
#include "CPreivewMapShader.h"

CPreivewMapShader::CPreivewMapShader()
	: CComputeShader(32, 32, 1)
	, m_iBrushIdx(0)
	, m_pInput(nullptr)
{
}

CPreivewMapShader::~CPreivewMapShader()
{
}


void CPreivewMapShader::UpdateData()
{
	m_Param.iArr[0] = (int)m_pPreviewMap->Width();
	m_Param.iArr[1] = (int)m_pPreviewMap->Height();
	m_Param.iArr[2] = m_iBrushIdx;
	m_Param.v2Arr[0] = m_vScale;

	// 높이맵
	m_pPreviewMap->UpdateData_CS(0, false);

	// 브러쉬
	m_pBrushTex->UpdateData_CS(8, true);

	// 피킹정보
	m_pInput->UpdateData_CS(16, true);

	// 실행 시킬 스레드 그룹 수 지정
	m_iGroupX = ((UINT)m_pPreviewMap->Width() / m_iGroupPerThreadCountX) + 1;
	m_iGroupY = ((UINT)m_pPreviewMap->Height() / m_iGroupPerThreadCountY) + 1;
	m_iGroupZ = 1;
}

void CPreivewMapShader::Clear()
{
	if (nullptr != m_pPreviewMap)
	{
		m_pPreviewMap->ClearCS(0);
		m_pPreviewMap = nullptr;
	}

	if (nullptr != m_pBrushTex)
	{
		m_pBrushTex->Clear(8);
		m_pBrushTex = nullptr;
	}

	if (nullptr != m_pInput)
	{
		m_pInput->Clear();
		m_pInput = nullptr;
	}
}
