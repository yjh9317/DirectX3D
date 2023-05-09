#include "pch.h"
#include "CImpAttack.h"

#include <Engine/CCollider3D.h>
#include <Engine/CEventMgr.h>
#include "CAttackCollisionScript.h"

CImpAttack::CImpAttack()
	:CScript((int)SCRIPT_TYPE::IMPATTACK)
{
}

CImpAttack::~CImpAttack()
{
}

void CImpAttack::enter()
{
	m_bAttack = false;
	Animator3D()->Play(L"Attack",false);
}

void CImpAttack::StateUpdate()
{
	//일정 거리 안에 들어오면 생성
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Run");
	}


	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 59 && !m_bAttack)
	{
		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\AttackCollision.pref")->Instantiate();

		Vec3 vPos = Transform()->GetRelativePos();
		Vec3 vFrontDir = Transform()->GetWorldFrontDir();
		vPos += vFrontDir * 50;
		vPos.y += GetOwner()->Collider3D()->GetWorldScale().y / 2.f;
		pObject->Transform()->SetRelativePos(vPos);

		pObject->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));

		CAttackCollisionScript* pScript = pObject->GetScript<CAttackCollisionScript>();
		pScript->SetEndTime(0.5);

		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)5;
		CEventMgr::GetInst()->AddEvent(info);

		m_bAttack = true;
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 58)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\imp_attack_01.ogg");
		pSound->Play(1, 0.1, false);
	}
	else if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 66)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\imp_attack_02.ogg");
		pSound->Play(1, 0.1, false);
	}

}

void CImpAttack::exit()
{
	Animator3D()->GetCurAnim()->Reset();
}

void CImpAttack::start()
{
	CState* pState = FSM()->FindState(L"Attack");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Attack", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpAttack::update()
{

}
