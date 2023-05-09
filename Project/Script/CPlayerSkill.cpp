#include "pch.h"
#include "CPlayerSkill.h"

#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>

CPlayerSkill::CPlayerSkill()
	:CScript((int)SCRIPT_TYPE::PLAYERSKILL)
{
}

CPlayerSkill::~CPlayerSkill()
{
}


void CPlayerSkill::enter()
{
	Animator3D()->Play(L"Skill",false);
}

void CPlayerSkill::StateUpdate()
{
	CAnimation3D* pAnim = Animator3D()->GetCurAnim();
	if (pAnim->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CPlayerSkill::exit()
{
	Animator3D()->GetCurAnim()->Reset();
}

void CPlayerSkill::start()
{
	CState* pState = FSM()->FindState(L"Skill");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Skill", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerSkill::update()
{
}

