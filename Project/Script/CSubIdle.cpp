#include "pch.h"
#include "CSubIdle.h"

CSubIdle::CSubIdle()
	:CScript((int)SCRIPT_TYPE::SUBIDLE)
{
}

CSubIdle::~CSubIdle()
{
}

void CSubIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
}

void CSubIdle::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	if (!pPlayer)
		return;
	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);

	float dist = (vPlayerXZ - vMonsterXZ).Length();
	if (900.f > dist)
	{
		FSM()->ChangeState(L"Walk");
	}
}

void CSubIdle::exit()
{
}

void CSubIdle::start()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CSubIdle::update()
{

}

