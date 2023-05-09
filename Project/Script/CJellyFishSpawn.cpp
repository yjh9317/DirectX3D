#include "pch.h"
#include "CJellyFishSpawn.h"

CJellyFishSpawn::CJellyFishSpawn()
	:CScript((int)SCRIPT_TYPE::JELLYFISHSPAWN)
{
}

CJellyFishSpawn::~CJellyFishSpawn()
{
}

void CJellyFishSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);
}

void CJellyFishSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CJellyFishSpawn::exit()
{
}

void CJellyFishSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CJellyFishSpawn::update()
{

}
