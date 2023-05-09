#include "pch.h"
#include "CBisonSpawn.h"

CBisonSpawn::CBisonSpawn()
	:CScript((int)SCRIPT_TYPE::BISONSPAWN)
{
}

CBisonSpawn::~CBisonSpawn()
{
}

void CBisonSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_idle.ogg");
	pSound->Play(1, 0.1, false);
}

void CBisonSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CBisonSpawn::exit()
{
}

void CBisonSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CBisonSpawn::update()
{
}

