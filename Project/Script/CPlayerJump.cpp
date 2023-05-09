#include "pch.h"
#include "CPlayerJump.h"

#include <Engine/CAnimator3D.h>

CPlayerJump::CPlayerJump()
	:CScript((int)SCRIPT_TYPE::PLAYERJUMP)
	,m_fSpeed(15.f)
	,m_fTime(0.5f)
{
}

CPlayerJump::~CPlayerJump()
{
}


void CPlayerJump::enter()
{
	Animator3D()->Play(L"Jump", false);
}

void CPlayerJump::StateUpdate()
{

	m_fTime += DT;

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += m_fSpeed * DT;

	// Animator 점프 애니메이션 생성
	
	//if (m_fTime > 0.5f)
	//{
	//	vPos.y -= m_fSpeed * DT;
	//	//떨어지는 애니메이션 작동
	//}
	
	//if() 만약 땅을 밟았을시 Idle로 변경
	
	
}

void CPlayerJump::exit()
{
	Animator3D()->GetCurAnim()->Reset();
}

void CPlayerJump::start()
{
	CState* pState = FSM()->FindState(L"Jump");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Jump", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerJump::update()
{

}
