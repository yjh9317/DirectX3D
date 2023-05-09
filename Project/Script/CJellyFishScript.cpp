#include "pch.h"
#include "CJellyFishScript.h"

#include "CHPUIScript.h"

CJellyFishScript::CJellyFishScript()
	:CScript((int)SCRIPT_TYPE::JELLYFISHSCRIPT)
	,m_MaxHP(100)
	,m_CurHP(100)
{
}

CJellyFishScript::~CJellyFishScript()
{
}



void CJellyFishScript::start()
{
	vector<CGameObject*> _v = GetOwner()->GetChild();

	for (int i = 0; i < _v.size(); ++i)
	{
		if (_v[i]->GetName() == "HPUI")
		{
			m_pHPUI = _v[i];
			m_pHPUIScript = m_pHPUI->GetScript<CHPUIScript>();
			return;
		}
	}
}

void CJellyFishScript::update()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Spawn");
	}

	if (m_pHPUI)
	{
		CHPUIScript* pScript = (CHPUIScript*)m_pHPUIScript;
		pScript->SetHPRatio(m_CurHP / (float)m_MaxHP);
	}

	if (m_iHit)
	{
		m_ftime += DT;
		if (m_ftime > 0.7f)
		{
			m_ftime = 0.f;
			m_iHit = 0;
			MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_iHit);
		}
		float time = m_ftime / 0.7f;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &time);
	}
}

void CJellyFishScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (L"ApRound" == _OtherObject->GetName() && Animator3D()->GetCurAnim()->GetAnimClip().strAnimName != L"Death")
	{
		m_CurHP -= 20;
		_OtherObject->Destroy();

		if (m_CurHP <= 0)
			FSM()->ChangeState(L"Death");

		m_iHit = 1;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_iHit);
	}
}
