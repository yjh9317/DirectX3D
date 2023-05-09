#include "pch.h"
#include "CBisonWalk.h"

#include "CBisonRun.h"

CBisonWalk::CBisonWalk()
	:CScript((int)SCRIPT_TYPE::BISONWALK)
{
}

CBisonWalk::~CBisonWalk()
{
}

void CBisonWalk::enter()
{
	Animator3D()->Play(L"Walk", true);
}

void CBisonWalk::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
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

	if (650.f > dist)
	{
		CBisonRun* pScript = GetOwner()->GetScript<CBisonRun>();
		FSM()->ChangeState(L"Run");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 288)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_step_01.ogg");
		pSound->Play(1, 0.05, false);
	}
	else if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 304)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_step_02.ogg");
		pSound->Play(1, 0.05, false);
	}
}

void CBisonWalk::exit()
{
}

void CBisonWalk::start()
{
	CState* pState = FSM()->FindState(L"Walk");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Walk", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}


void CBisonWalk::update()
{
}
