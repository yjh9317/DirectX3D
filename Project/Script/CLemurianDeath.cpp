#include "pch.h"
#include "CLemurianDeath.h"

CLemurianDeath::CLemurianDeath()
	:CScript((int)SCRIPT_TYPE::LEMURIANDEATH)
{
}

CLemurianDeath::~CLemurianDeath()
{
}

void CLemurianDeath::enter()
{
	Animator3D()->Play(L"Death",false);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\lemurianBruiser_death.ogg");
	pSound->Play(1, 0.1, false);


}

void CLemurianDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish() && !m_bDead)
	{
		m_bDead = true;
		GetOwner()->Destroy();
	}
}

void CLemurianDeath::exit()
{
}

void CLemurianDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianDeath::update()
{

}

