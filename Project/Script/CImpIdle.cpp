#include "pch.h"
#include "CImpIdle.h"

#include <Engine/CBounding.h>

CImpIdle::CImpIdle()
	:CScript((int)SCRIPT_TYPE::IMPIDLE)
{
}

CImpIdle::~CImpIdle()
{
}

void CImpIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
}

void CImpIdle::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);

	float dist = (vPlayerXZ - vMonsterXZ).Length();

	if (800.f > dist)
	{
		FSM()->ChangeState(L"Run");
	}
}

void CImpIdle::exit()
{
}

void CImpIdle::start()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpIdle::update()
{

}


