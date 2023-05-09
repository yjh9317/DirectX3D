#include "pch.h"
#include "CCoffeeScript.h"

#include "CPlayerScript.h"

CCoffeeScript::CCoffeeScript()
	:CScript((int)SCRIPT_TYPE::COFFEESCRIPT)
{
}

CCoffeeScript::~CCoffeeScript()
{
}

void CCoffeeScript::update()
{
}

void CCoffeeScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* pScript =_OtherObject->GetScript<CPlayerScript>();
		float _fSpeed = pScript->GetSpeed();
		pScript->SetSpeed(_fSpeed + 50.f);

		GetOwner()->Destroy();
	}
	
}


