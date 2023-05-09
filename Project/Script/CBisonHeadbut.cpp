#include "pch.h"
#include "CBisonHeadbut.h"

#include <Engine/CCollider3D.h>
#include <Engine/CEventMgr.h>
#include "CAttackCollisionScript.h"


CBisonHeadbut::CBisonHeadbut()
	:CScript((int)SCRIPT_TYPE::BISONHEADBUT)
{
}

CBisonHeadbut::~CBisonHeadbut()
{
}

void CBisonHeadbut::enter()
{
	Animator3D()->Play(L"Headbut", false);

	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bison_headbutt_attack_swing_v2_01.ogg");
	pSound->Play(1, (float)0.1, false);

	CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\AttackCollision.pref")->Instantiate();

	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFrontDir = Transform()->GetWorldFrontDir();
	vPos += vFrontDir * 120;
	vPos.y += GetOwner()->Collider3D()->GetWorldScale().y / 2.f;
	pObject->Transform()->SetRelativePos(vPos);

	pObject->Collider3D()->SetOffsetScale(Vec3(80.f, 80.f, 80.f));

	CAttackCollisionScript* pScript = pObject->GetScript<CAttackCollisionScript>();
	pScript->SetEndTime(0.5);

	tEventInfo info;
	info.eType = EVENT_TYPE::CREATE_OBJ;
	info.lParam = (DWORD_PTR)pObject;
	info.wParam = (DWORD_PTR)5;
	CEventMgr::GetInst()->AddEvent(info);
}

void CBisonHeadbut::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CBisonHeadbut::exit()
{
}

void CBisonHeadbut::start()
{
	CState* pState = FSM()->FindState(L"Headbut");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Headbut", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}


void CBisonHeadbut::update()
{
}

