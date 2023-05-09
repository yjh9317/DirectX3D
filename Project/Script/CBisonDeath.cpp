#include "pch.h"
#include "CBisonDeath.h"

CBisonDeath::CBisonDeath()
	:CScript((int)SCRIPT_TYPE::BISONDEATH)
{
}

CBisonDeath::~CBisonDeath()
{
}

void CBisonDeath::enter()
{
	Animator3D()->Play(L"Death", false);
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_death.ogg");
	pSound->Play(1, (float)0.1, false);
}

void CBisonDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish() && !m_bDead)
	{
		m_bDead = true;
		GetOwner()->Destroy();
	}
}

void CBisonDeath::exit()
{
}

void CBisonDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CBisonDeath::update()
{
}

