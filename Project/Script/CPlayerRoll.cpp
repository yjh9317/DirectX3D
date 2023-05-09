#include "pch.h"
#include "CPlayerRoll.h"

#include <Engine/CFSM.h>
#include <Engine/CState.h>
#include <Engine/CAnimator3D.h>
#include "CPlayerScript.h"

CPlayerRoll::CPlayerRoll()
	:CScript((int)SCRIPT_TYPE::PLAYERROLL)
	, m_fSpeed(25.f)
	, m_iDir(0)
	, m_fTime(0.f)
{
}

CPlayerRoll::~CPlayerRoll()
{
}

void CPlayerRoll::enter()
{
	CPlayerScript* pScript = GetOwner()->GetScript<CPlayerScript>();
	m_fSpeed = pScript->GetSpeed() * 3;

	Animator3D()->FindAnim(L"RollFront")->SetLerpTime(0.f);
	Animator3D()->FindAnim(L"RollLeft")->SetLerpTime(0.f);
	Animator3D()->FindAnim(L"RollRight")->SetLerpTime(0.f);
	Animator3D()->FindAnim(L"RollBack")->SetLerpTime(0.f);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_shift_v3_01.ogg");
	pSound->Play(1, 0.1, true);


	Animator3D()->FindAnim(L"Idle")->SetLerpTime(0.f);
	
}

void CPlayerRoll::StateUpdate()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFrontDir = Transform()->GetWorldFrontDir();
	Vec3 vRightDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		if (L"Move" == FSM()->GetPrevState()->GetStateName())
		{
			FSM()->ChangeState(L"Move");
		}
		else
			FSM()->ChangeState(L"Idle");
	}

	if (0 == m_iDir)
	{
		vPos += m_fSpeed * vFrontDir * DT;
		Animator3D()->Play(L"RollFront", false);
	}
	else if (1 == m_iDir)
	{
		vPos += m_fSpeed * -vRightDir * DT;
		Animator3D()->Play(L"RollLeft", false);
	}
	else if (2 == m_iDir)
	{
		vPos += m_fSpeed * vRightDir * DT;
		Animator3D()->Play(L"RollRight", false);
	}
	else if (3 == m_iDir)
	{
		vPos += m_fSpeed * -vFrontDir * DT;
		Animator3D()->Play(L"RollBack", false);
	}


	Transform()->SetRelativePos(vPos);
}

void CPlayerRoll::exit()
{
	Animator3D()->GetCurAnim()->Reset();
}

void CPlayerRoll::start()
{
	CState* pState = FSM()->FindState(L"Roll");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Roll", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerRoll::update()
{

}


