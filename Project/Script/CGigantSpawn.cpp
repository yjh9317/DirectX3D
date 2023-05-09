#include "pch.h"
#include "CGigantSpawn.h"

#include <Engine/CEventMgr.h>

CGigantSpawn::CGigantSpawn()
	:CScript((int)SCRIPT_TYPE::GIGANTSPAWN)
{
}

CGigantSpawn::~CGigantSpawn()
{
}

void CGigantSpawn::enter()
{
	Animator3D()->Play(L"Spawn", false);
}

void CGigantSpawn::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 315 && !m_bSpawn)
	{
		m_bSpawn = true;
		Vec3 vPos = Transform()->GetRelativePos();
		vPos += Transform()->GetWorldFrontDir() * 700;
		vPos += Transform()->GetWorldRightDir() * 300.f;

		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SubGigant.pref")->Instantiate();
		pObject->Transform()->SetRelativePos(vPos);
		pObject->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));
		pObject->Transform()->SetRelativeRotation(Transform()->GetRelativeRotation());

		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;

		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)4;
		CEventMgr::GetInst()->AddEvent(info);

		pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\SubGigant.pref")->Instantiate();
		vPos -= Transform()->GetWorldRightDir() * 600.f;
		pObject->Transform()->SetRelativePos(vPos);
		pObject->Transform()->SetRelativeScale(Vec3(0.5f, 0.5f, 0.5f));
		pObject->Transform()->SetRelativeRotation(Transform()->GetRelativeRotation());

		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)4;
		CEventMgr::GetInst()->AddEvent(info);
	}
}

void CGigantSpawn::exit()
{
	m_bSpawn = false;
	Animator3D()->FindAnim(L"Spawn")->Reset();
}

void CGigantSpawn::start()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Spawn", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CGigantSpawn::update()
{

}

