#include "pch.h"
#include "CPlayerRun.h"

#include <Engine/CAnimator3D.h>

#include "CPlayerScript.h"

CPlayerRun::CPlayerRun()
	:CScript((int)SCRIPT_TYPE::PLAYERRUN)
	, m_fSpeed(20.f)
{
}

CPlayerRun::~CPlayerRun()
{
}


void CPlayerRun::enter()
{
	CPlayerScript* pSCript = GetOwner()->GetScript<CPlayerScript>();
	if (pSCript)
	{
		m_fSpeed = pSCript->GetSpeed() * 5.f;
	}

	Animator3D()->Play(L"Sprint", true);
}

void CPlayerRun::StateUpdate()
{
	Vec3 vFrontDir = Transform()->GetWorldFrontDir();

	Vec3 vPos = Transform()->GetRelativePos();
	vPos += m_fSpeed * vFrontDir * DT;

	vFrontDir.y = 0.f;

	if (KEY_TAP(KEY::LCTRL))
	{
		FSM()->ChangeState(L"Idle");
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerRun::exit()
{
}

void CPlayerRun::start()
{
	CState* pState = FSM()->FindState(L"Run");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Run", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerRun::update()
{

}

