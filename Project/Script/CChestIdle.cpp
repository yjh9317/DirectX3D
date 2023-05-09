#include "pch.h"
#include "CChestIdle.h"


CChestIdle::CChestIdle()
	:CScript((int)SCRIPT_TYPE::CHESTIDLE)
{
}

CChestIdle::~CChestIdle()
{
}


void CChestIdle::enter()
{
	Animator3D()->Play(L"Idle", false);
}

void CChestIdle::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");

	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();

	float Len = (vPos - vPlayerPos).Length();
	
	if (Len < 300.f && KEY_TAP(KEY::E))
	{
		FSM()->ChangeState(L"Open");
	}

}

void CChestIdle::exit()
{
}

void CChestIdle::update()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

