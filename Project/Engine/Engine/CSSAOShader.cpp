#include "pch.h"
#include "CSSAOShader.h"

CSSAOShader::CSSAOShader()
	: m_FirstBuffer(nullptr)
	, m_SecondBuffer(nullptr)
{
	Vec2 _vResolution = CDevice::GetInst()->GetRenderResolution();

	_vResolution /= 2;

	m_iGroupX = (UINT)ceil((float)(_vResolution.x * _vResolution.y) / 1024.0f);
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

CSSAOShader::~CSSAOShader()
{
}

void CSSAOShader::UpdateData()
{
}

void CSSAOShader::Clear()
{
}

