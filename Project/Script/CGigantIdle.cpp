#include "pch.h"
#include "CGigantIdle.h"

#include "CGigantScript.h"

CGigantIdle::CGigantIdle()
	:CScript((int)SCRIPT_TYPE::GIGANTIDLE)
{
}

CGigantIdle::~CGigantIdle()
{
}

void CGigantIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
	Animator3D()->GetCurAnim()->SetLerpTime(0.f);
	m_HandCollision->Deactivate();
	
}

void CGigantIdle::StateUpdate()
{
	m_fTime +=DT;
	if (m_fTime > 3.f)
	{
		CGigantScript* pScript = GetOwner()->GetScript<CGigantScript>();
		float f = pScript->GetHPRatio();
		if (f < 0.5 && !m_bSpawn)
		{
			m_bSpawn = true;
			FSM()->ChangeState(L"Spawn");
			return;
		}

		if (m_iAtk == 0)
		{
			FSM()->ChangeState(L"Attack");
			++m_iAtk;
		}
		else if (m_iAtk == 1)
		{
			FSM()->ChangeState(L"Loop");
			m_iAtk = 0;
		}

	}
}

void CGigantIdle::exit()
{
	m_fTime = 0.f;
}

void CGigantIdle::start()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}

	vector<CGameObject*> _v = GetOwner()->GetChild();

	for (int i = 0; i < _v.size(); ++i)
	{
		if (_v[i]->GetName() == "Collision")
		{
			m_HandCollision = _v[i];
		}
	}
}

void CGigantIdle::update()
{

}

