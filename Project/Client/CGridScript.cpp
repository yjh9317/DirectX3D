#include "pch.h"
#include "CGridScript.h"

CGridScript::CGridScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
	, m_pToolCam(nullptr)
	, m_fThickness(0.1f)
	, m_fDistance(0.f)
	, m_iMaxStep(5)
	, m_fAlpha(0.f)
	, m_vColor(Vec4(1.f, 1.f, 1.f, 1.f))
{
}

CGridScript::~CGridScript()
{
}

void CGridScript::update()
{
	m_vToolCamPos = m_pToolCam->Transform()->GetWorldPos();
	Transform()->SetRelativePos(Vec3(m_vToolCamPos.x, 0.f, m_vToolCamPos.z));

	CalculateGrid();
}

void CGridScript::CalculateGrid()
{
	// ī�޶� ���̿� ���� Grid ������ ���Ѵ�.
	// 1 �ܰ� : ���� 10, ī�޶� ���� 0 ~ 100
	// 2 �ܰ� : ���� 100, ī�޶� ���� 100 ~ 1000
	// 3 �ܰ� : ���� 1000, ī�޶� ���� 1000 ~ 10000
	// 4 �ܰ� : ���� 10000, ī�޶� ���� 10000 ~ 
	Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial(0);

	UINT iHeight = (UINT)abs(m_vToolCamPos.y);
	UINT iStep = 1;
	UINT i = 100;
	for (; iStep <= 4; i *= 10, ++iStep)
	{
		if (iHeight <= i)
			break;
	}

	m_fThickness = ((float)iHeight * 0.01f);
	m_fAlpha = abs(1.f - (float)((double)iHeight / pow(10., (double)iStep + 1.)));

	// Material �� ���� ����	
	pMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &iStep);
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fAlpha);
	pMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_1, &m_fThickness);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_0, &m_vToolCamPos);
	pMtrl->SetScalarParam(SCALAR_PARAM::VEC4_1, &m_vColor);
}
