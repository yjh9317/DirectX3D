#include "pch.h"
#include "CScreenUIScript.h"


CScreenUIScript::CScreenUIScript()
	:CScript((int)SCRIPT_TYPE::SCREENUISCRIPT)
	, m_fHPRatio(1.f)
{
}

CScreenUIScript::~CScreenUIScript()
{
}

void CScreenUIScript::update()
{
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fHPRatio);
}
