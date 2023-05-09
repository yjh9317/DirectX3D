#include "pch.h"
#include "CImpDeath.h"


CImpDeath::CImpDeath()
	:CScript((int)SCRIPT_TYPE::IMPDEATH)
{
}

CImpDeath::~CImpDeath()
{
}

void CImpDeath::enter()
{
	Animator3D()->Play(L"Death", false);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\imp_death_01.ogg");
	pSound->Play(1, 0.1, false);
}

void CImpDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		GetOwner()->Destroy();
	}
}

void CImpDeath::exit()
{
}

void CImpDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpDeath::update()
{

}

