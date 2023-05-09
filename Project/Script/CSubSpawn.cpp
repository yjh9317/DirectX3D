#include "pch.h"
#include "CSubSpawn.h"

CSubSpawn::CSubSpawn()
	:CScript((int)SCRIPT_TYPE::SUBSPAWN)
{
}

CSubSpawn::~CSubSpawn()
{
}

void CSubSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);
}

void CSubSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CSubSpawn::exit()
{
}

void CSubSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CSubSpawn::update()
{

}

