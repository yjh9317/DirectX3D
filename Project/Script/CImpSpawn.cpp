#include "pch.h"
#include "CImpSpawn.h"


CImpSpawn::CImpSpawn()
	:CScript((int)SCRIPT_TYPE::IMPSPAWN)
{
}

CImpSpawn::~CImpSpawn()
{
}

void CImpSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\imp_spawn_01.ogg");
	pSound->Play(1, 0.1, false);
}

void CImpSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CImpSpawn::exit()
{
}

void CImpSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpSpawn::update()
{

}

