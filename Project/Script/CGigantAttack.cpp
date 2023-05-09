#include "pch.h"
#include "CGigantAttack.h"

CGigantAttack::CGigantAttack()
	:CScript((int)SCRIPT_TYPE::GIGANTATTACK)
{
}

CGigantAttack::~CGigantAttack()
{
}


void CGigantAttack::enter()
{

	Animator3D()->Play(L"Attack", false);
	Animator3D()->FindAnim(L"Attack")->SetSpeed(0.8);
	m_HandCollision->Activate();

}

void CGigantAttack::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}

}

void CGigantAttack::exit()
{
	Animator3D()->FindAnim(L"Attack")->Reset();
	m_HandCollision->Deactivate();
}

void CGigantAttack::start()
{
	CState* pState = FSM()->FindState(L"Attack");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Attack", [&] {this->StateUpdate(); },
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

void CGigantAttack::update()
{

}

