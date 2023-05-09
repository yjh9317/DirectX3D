#include "pch.h"
#include "CBisonRun.h"

CBisonRun::CBisonRun()
	:CScript((int)SCRIPT_TYPE::BISONRUN)
{
}

CBisonRun::~CBisonRun()
{
}

void CBisonRun::enter()
{
	Animator3D()->Play(L"Run", true);
	Animator3D()->FindAnim(L"Run")->SetLerpTime(0.f);
}

void CBisonRun::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	if (!pPlayer)
		return;

	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);


	Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPos = Transform()->GetWorldPos();
	vPos += vFrontDir * DT * 300;
	Transform()->SetRelativePos(vPos);

	float dist = (vPlayerXZ - vMonsterXZ).Length();


	m_fTime += DT;

	if (m_fTime > 1.5f)
	{
		m_fTime = 0.f;
		FSM()->ChangeState(L"Headbut");
	}

	if (150.f > dist)
	{
		m_fTime = 0.f;
		FSM()->ChangeState(L"Headbut");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 258)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_step_charge_01.ogg");
		pSound->Play(1, 0.1, false);
	}
	else if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 275)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_step_charge_02.ogg");
		pSound->Play(1, 0.1, false);
	}
}

void CBisonRun::exit()
{
}

void CBisonRun::start()
{
	CState* pState = FSM()->FindState(L"Run");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Run", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CBisonRun::update()
{
}

