#include "pch.h"
#include "CLemurianIdle.h"

CLemurianIdle::CLemurianIdle()
	:CScript((int)SCRIPT_TYPE::LEMURIANIDLE)
{
}

CLemurianIdle::~CLemurianIdle()
{
}

void CLemurianIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
	Animator3D()->GetCurAnim()->SetLerpTime(0.f);
}

void CLemurianIdle::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	if (!pPlayer)
		return;

	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);

	//Vec2 vDir = vPlayerXZ - vMonsterXZ;
	//vDir.Normalize();

	//Vec2 vFrontDir = Vec2(0.f, 1.f);
	//vFrontDir.Normalize();
	//float angle = vFrontDir.Dot(vDir);

	//float acos = acosf(angle);
	//float fresult = (acos / XM_PI) * 180;

	//if (vDir.x < 0.f)
	//	acos = -acos;
	//Vec3 vRot = Transform()->GetRelativeRotation();
	//vRot.y = acos;
	//Transform()->SetRelativeRotation(vRot);

	float dist = (vPlayerXZ - vMonsterXZ).Length();
	if (900.f > dist)
	{
		FSM()->ChangeState(L"Run");
	}
}

void CLemurianIdle::exit()
{
}


void CLemurianIdle::start()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianIdle::update()
{

}

