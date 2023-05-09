#include "pch.h"
#include "CBellScript.h"

#include "CPlayerScript.h"

CBellScript::CBellScript()
	:CScript((int)SCRIPT_TYPE::BELLSCRIPT)
{
}

CBellScript::~CBellScript()
{
}

void CBellScript::update()
{

	m_fTime += DT;

	if (m_fTime > 8.f)
	{
		GetOwner()->Destroy();
	}

	Vec3 vPos = Transform()->GetRelativePos();
	vPos += m_fSpeed * DT * m_vFrontDir;
	Transform()->SetRelativePos(vPos);

	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.x += DT * 5;
	vRot.z += DT * 3;
	Transform()->SetRelativeRotation(vRot);

}

void CBellScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* pScript = _OtherObject->GetScript<CPlayerScript>();
		int iCurHP = pScript->GetHP();
		pScript->SetHP(iCurHP-20);
	}
}

