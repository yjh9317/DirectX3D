#include "pch.h"
#include "CLemurianFireball.h"

#include <Engine/CEventMgr.h>
#include "CBellScript.h"
#include <Engine/CCollider3D.h>

CLemurianFireball::CLemurianFireball()
	:CScript((int)SCRIPT_TYPE::LEMURIANFIREBALL)
{
}

CLemurianFireball::~CLemurianFireball()
{
}

void CLemurianFireball::enter()
{
	Animator3D()->Play(L"Fireball", false);
}

void CLemurianFireball::StateUpdate()
{
	
	if (!IsFire && (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 71))
	{
		IsFire = true;
		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bell.pref")->Instantiate();

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.y += GetOwner()->Collider3D()->GetWorldScale().y * 2.f/ 3.f;
		vPos += Transform()->GetWorldFrontDir() * 50;
		pObject->Transform()->SetRelativePos(vPos);

		CBellScript* pScript = pObject->GetScript<CBellScript>();
		pScript->SetSpeed(500.f);

		CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
		if (!pPlayer)
		{
			FSM()->ChangeState(L"Idle");
		}
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();

		vPlayerPos.y += pPlayer->Collider3D()->GetWorldPos().y/2.f;
		Vec3 vDir = vPlayerPos - pObject->Transform()->GetRelativePos();
		vDir.Normalize();
		pScript->SetFrontDir(vDir);
		
		
		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)5;
		CEventMgr::GetInst()->AddEvent(info);

		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\lemurianBruiser_m1_shoot_01.ogg");
		pSound->Play(1, 0.1, false);
	}

	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CLemurianFireball::exit()
{
	IsFire = false;
	Animator3D()->FindAnim(L"Fireball")->Reset();
}

void CLemurianFireball::start()
{
	CState* pState = FSM()->FindState(L"Fireball");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Fireball", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianFireball::update()
{

}

