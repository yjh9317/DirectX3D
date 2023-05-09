#include "pch.h"
#include "CSubAttack.h"

#include <Engine/CCollider3D.h>
#include <Engine/CEventMgr.h>
#include "CAttackCollisionScript.h"


CSubAttack::CSubAttack()
	:CScript((int)SCRIPT_TYPE::SUBATTACK)
{
}

CSubAttack::~CSubAttack()
{
}

void CSubAttack::enter()
{
	m_bAttack = false;
	Animator3D()->Play(L"Attack", false);

}

void CSubAttack::StateUpdate()
{
	//일정 거리 안에 들어오면 생성
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Walk");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 138 && !m_bAttack)
	{
		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\AttackCollision.pref")->Instantiate();

		Vec3 vPos = Transform()->GetRelativePos();
		Vec3 vFrontDir = Transform()->GetWorldFrontDir();
		vPos += vFrontDir * 150;
		pObject->Transform()->SetRelativePos(vPos);

		pObject->Collider3D()->SetOffsetScale(Vec3(100.f, 40.f, 100.f));

		CAttackCollisionScript* pScript = pObject->GetScript<CAttackCollisionScript>();
		pScript->SetEndTime(0.5);

		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)5;
		CEventMgr::GetInst()->AddEvent(info);

		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\explode.ogg");
		pSound->Play(1, 0.1, true);

		m_bAttack = true;
	}
}

void CSubAttack::exit()
{
	Animator3D()->GetCurAnim()->Reset();
}

void CSubAttack::start()
{
	CState* pState = FSM()->FindState(L"Attack");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Attack", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CSubAttack::update()
{

}

