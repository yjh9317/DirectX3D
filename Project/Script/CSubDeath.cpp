#include "pch.h"
#include "CSubDeath.h"

CSubDeath::CSubDeath()
	:CScript((int)SCRIPT_TYPE::SUBDEATH)
{
}

CSubDeath::~CSubDeath()
{
}

void CSubDeath::enter()
{
	Animator3D()->Play(L"Death", false);
}

void CSubDeath::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		GetOwner()->Destroy();
	}
}

void CSubDeath::exit()
{
}

void CSubDeath::start()
{
	CState* pState = FSM()->FindState(L"Death");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Death", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CSubDeath::update()
{

}

