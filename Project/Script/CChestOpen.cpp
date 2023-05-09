#include "pch.h"
#include "CChestOpen.h"

#include <Engine/CFSM.h>
#include <Engine/CEventMgr.h>

#include "CMedikitScript.h"

CChestOpen::CChestOpen()
	:CScript((int)SCRIPT_TYPE::CHESTOPEN)
{
}

CChestOpen::~CChestOpen()
{
}

void CChestOpen::enter()
{
	Animator3D()->Play(L"Open", false);

	Animator3D()->FindAnim(L"Open")->SetLerpTime(0.f);
}

void CChestOpen::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Opened");
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 38 && !m_bOpen)
	{
		m_bOpen = true;
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\chest_open_01.wav");
		pSound->Play(1, 0.1, false);

		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Medikit.pref")->Instantiate();

		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		pObject->Transform()->SetRelativePos(vPos);
		CMedikitScript* pScript = pObject->GetScript<CMedikitScript>();

		Vec3 vDir = Transform()->GetWorldFrontDir();
		pScript->SetDir(vDir);

		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)6;
		CEventMgr::GetInst()->AddEvent(info);
	}
}

void CChestOpen::exit()
{
}

void CChestOpen::update()
{
	CState* pState = FSM()->FindState(L"Open");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Open", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}
