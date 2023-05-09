#include "pch.h"
#include "CLemurianSpawn.h"

#include <Engine/CSound.h>

CLemurianSpawn::CLemurianSpawn()
	:CScript((int)SCRIPT_TYPE::LEMURIANSPAWN)
{
}

CLemurianSpawn::~CLemurianSpawn()
{
}


void CLemurianSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);

	
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\lemurian_spawn_01.ogg");
	pSound->Play(1, 0.5, false);
}

void CLemurianSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CLemurianSpawn::exit()
{
}

void CLemurianSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianSpawn::update()
{

}

