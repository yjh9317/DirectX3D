#include "pch.h"
#include "CJellyFishDeath.h"

CJellyFishDeath::CJellyFishDeath()
	:CScript((int)SCRIPT_TYPE::JELLYFISHDEATH)
{
}

CJellyFishDeath::~CJellyFishDeath()
{
}

void CJellyFishDeath::enter()
{
	Animator3D()->Play(L"Death", false);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\jellyfish_detonate_01.ogg");
	pSound->Play(1, 0.05, false);

}

void CJellyFishDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish() && !m_bCheck)
	{
		m_bCheck = true;
		GetOwner()->Destroy();
	}
}

void CJellyFishDeath::exit()
{
}

void CJellyFishDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CJellyFishDeath::update()
{
}