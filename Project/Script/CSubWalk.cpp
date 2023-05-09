#include "pch.h"
#include "CSubWalk.h"

CSubWalk::CSubWalk()
	:CScript((int)SCRIPT_TYPE::SUBWALK)
{
}

CSubWalk::~CSubWalk()
{
}


void CSubWalk::enter()
{
	Animator3D()->Play(L"Walk", true);
}

void CSubWalk::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	if (!pPlayer)
	{
		FSM()->ChangeState(L"Idle");
		return;
	}
	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);

	Vec2 vDir = vPlayerXZ - vMonsterXZ;
	vDir.Normalize();

	Vec2 XZ = Vec2(0.f, 1.f);
	XZ.Normalize();
	float angle = XZ.Dot(vDir);

	float acos = acosf(angle);
	float fresult = (acos / XM_PI) * 180;

	if (vDir.x < 0.f)
		acos = -acos;

	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.y = acos;
	Transform()->SetRelativeRotation(vRot);

	Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPos = Transform()->GetWorldPos();
	vPos += vFrontDir * DT * 100;
	Transform()->SetRelativePos(vPos);

	float dist = (vPlayerXZ - vMonsterXZ).Length();
	if (150.f > dist)
	{
		FSM()->ChangeState(L"Attack");
	}
}

void CSubWalk::exit()
{
}

void CSubWalk::start()
{
	CState* pState = FSM()->FindState(L"Walk");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Walk", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CSubWalk::update()
{

}

 