#include "pch.h"
#include "CPlayerMove.h"

#include <Engine/CAnimator3D.h>

#include "CPlayerScript.h"
#include "CPlayerRoll.h"

CPlayerMove::CPlayerMove()
	:CScript((int)SCRIPT_TYPE::PLAYERMOVE)
	, m_fSpeed(10.f)
{
}

CPlayerMove::~CPlayerMove()
{
}


void CPlayerMove::enter()
{
	CPlayerScript* pSCript = GetOwner()->GetScript<CPlayerScript>();
	if (pSCript)
	{
		m_fSpeed = pSCript->GetSpeed() * 1;
	}
}

void CPlayerMove::StateUpdate()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	//vFrontDir.y = 0.f;
	Vec3 vRightDir = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	int iCurIdx = Animator3D()->GetCurAnim()->GetCurFrameIdx();

	if (iCurIdx == 279 || iCurIdx == 310 || iCurIdx == 347 || iCurIdx == 240)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\step_stone_01.ogg");
		pSound->Play(1, 0.1, false);
	}
	else if (iCurIdx == 295 || iCurIdx == 329 || iCurIdx == 360 || iCurIdx == 255)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\step_stone_02.ogg");
		pSound->Play(1, 0.1, false);
	}

	if (KEY_PRESSED(KEY::W))
	{
		vPos += m_fSpeed * vFrontDir * DT;

		if (KEY_PRESSED(KEY::D))
		{
			vPos += m_fSpeed * vRightDir *DT;
		}
		else if (KEY_PRESSED(KEY::A))
		{
			vPos += m_fSpeed * -vRightDir * DT;
		}

		Animator3D()->Play(L"RunFront", true);

		if (KEY_TAP(KEY::LSHFT))
		{
			CPlayerRoll* pScript = GetOwner()->GetScript<CPlayerRoll>();
			pScript->SetDir(0);
			FSM()->ChangeState(L"Roll");
		}

	}
	else if (KEY_PRESSED(KEY::A))
	{
		if (KEY_PRESSED(KEY::W))
		{
			vPos += m_fSpeed * vFrontDir * DT;
		}
		else if (KEY_PRESSED(KEY::S))
		{
			vPos += m_fSpeed * -vFrontDir * DT;
		}
		vPos += m_fSpeed * -vRightDir * DT;
		Animator3D()->Play(L"RunLeft", true);

		if (KEY_TAP(KEY::LSHFT))
		{
			CPlayerRoll* pScript = GetOwner()->GetScript<CPlayerRoll>();
			pScript->SetDir(1);
			FSM()->ChangeState(L"Roll");
		}

	}
	else if (KEY_PRESSED(KEY::D))
	{
		if (KEY_PRESSED(KEY::W))
		{
			vPos += m_fSpeed * vFrontDir * DT;
		}
		else if (KEY_PRESSED(KEY::S))
		{
			vPos += m_fSpeed * -vFrontDir * DT;
		}
		vPos += m_fSpeed * vRightDir * DT;
		Animator3D()->Play(L"RunRight", true);

		if (KEY_TAP(KEY::LSHFT))
		{
			CPlayerRoll* pScript = GetOwner()->GetScript<CPlayerRoll>();
			pScript->SetDir(2);
			FSM()->ChangeState(L"Roll");
		}
	}
	else if (KEY_PRESSED(KEY::S))
	{
		vPos += m_fSpeed * -vFrontDir * DT;
		Animator3D()->Play(L"RunBack", true);

		if (KEY_TAP(KEY::LSHFT))
		{
			CPlayerRoll* pScript = GetOwner()->GetScript<CPlayerRoll>();
			pScript->SetDir(3);
			FSM()->ChangeState(L"Roll");
		}
	}


	else
	{
		FSM()->ChangeState(L"Idle");
	}

	Transform()->SetRelativePos(vPos);

}

void CPlayerMove::exit()
{
}

void CPlayerMove::start()
{
	CState* pState = FSM()->FindState(L"Move");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Move", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerMove::update()
{

}

