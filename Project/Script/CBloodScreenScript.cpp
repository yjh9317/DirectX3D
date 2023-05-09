#include "pch.h"
#include "CBloodScreenScript.h"

CBloodScreenScript::CBloodScreenScript()
	:CScript((int)SCRIPT_TYPE::BLOODSCREENSCRIPT)
	, m_fTime(0)
{
}

CBloodScreenScript::~CBloodScreenScript()
{
}

void CBloodScreenScript::start()
{
	Ptr<CTexture> pPostTex = CResMgr::GetInst()->FindRes<CTexture>(L"PostProcessTex");
	MeshRender()->GetSharedMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pPostTex);
}

void CBloodScreenScript::update()
{
	if (m_fTime > 0.f)
	{
		MeshRender()->GetSharedMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fTime);
		m_fTime -= DT;
	}
	else if (m_fTime <= 0 && GetOwner()->IsActive())
	{
		GetOwner()->Deactivate();
	}

}

