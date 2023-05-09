#include "pch.h"
#include "CLemurianRun.h"

CLemurianRun::CLemurianRun()
	:CScript((int)SCRIPT_TYPE::LEMURIANRUN)
{
}

CLemurianRun::~CLemurianRun()
{
}


void CLemurianRun::enter()
{
	Animator3D()->Play(L"Run", true);
}

void CLemurianRun::StateUpdate()
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
	m_fAttackTime += DT;

	if (650.f > dist && m_fAttackTime >1.5f)
	{
		m_fAttackTime = 0.f;
		FSM()->ChangeState(L"FireballBefore");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 205)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\step_dirt_01.ogg");
		pSound->Play(1, 0.05, false);
	}
	else if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 219)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\step_dirt_01.ogg");
		pSound->Play(1, 0.05, false);
	}
}

void CLemurianRun::exit()
{
}

void CLemurianRun::start()
{
	CState* pState = FSM()->FindState(L"Run");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Run", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianRun::update()
{

}

