#include "pch.h"
#include "CPlayerIdle.h"

#include <Engine/CState.h>
#include <Engine/CFSM.h>
#include <Engine/CAnimator3D.h>

#include "CPlayerRoll.h"
#include "CPlayerMove.h"


CPlayerIdle::CPlayerIdle()
	:CScript((int)SCRIPT_TYPE::PLAYERIDLE)
{
}

CPlayerIdle::~CPlayerIdle()
{
}


void CPlayerIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
}

void CPlayerIdle::StateUpdate()
{
	if (KEY_TAP(KEY::LSHFT))
	{
		FSM()->ChangeState(L"Roll");
		CPlayerRoll* pScript = GetOwner()->GetScript<CPlayerRoll>();
		pScript->SetDir(0);
	}
	// Walk
	if (KEY_TAP(KEY::A) || KEY_TAP(KEY::D) || KEY_TAP(KEY::S) ||KEY_TAP(KEY::W))
	{
		FSM()->ChangeState(L"Move");
	}
	// Run
	else if (KEY_TAP(KEY::LCTRL))
	{
		FSM()->ChangeState(L"Run");
	}
	else if (KEY_TAP(KEY::LBTN))
	{
		FSM()->ChangeState(L"Fire");
	}
}

void CPlayerIdle::exit()
{
	Animator3D()->FindAnim(L"Idle")->SetLerpTime(0.07f);
}

void CPlayerIdle::start()
{
}

void CPlayerIdle::update()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle",
			std::bind(&CPlayerIdle::StateUpdate, this),
			std::bind(&CPlayerIdle::enter, this),
			std::bind(&CPlayerIdle::exit, this));
	}
}

