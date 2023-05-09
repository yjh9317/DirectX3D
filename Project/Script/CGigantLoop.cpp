#include "pch.h"
#include "CGigantLoop.h"

#include "CGCScript.h"
#include <Engine/CEventMgr.h>
#include <Engine/CCollider3D.h>

CGigantLoop::CGigantLoop()
	:CScript((int)SCRIPT_TYPE::GIGANTLOOP)
{
}

CGigantLoop::~CGigantLoop()
{
}

void CGigantLoop::enter()
{
	Animator3D()->Play(L"Loop", true);
}

void CGigantLoop::StateUpdate()
{
	m_fFireTime += DT;
	m_fTime += DT;
	if (m_fFireTime > 1.f)
	{
		m_fFireTime = 0.f;
		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\GolemCube.pref")->Instantiate();

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.y += 1500.f;
		pObject->Transform()->SetRelativePos(vPos);
		pObject->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

		CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
		Vec3 vPlayerPos = pPlayer->Transform()->GetRelativePos();

		CGCScript* pScript = pObject->GetScript<CGCScript>();
		pScript->SetSpeed(800.f);
		vPlayerPos.y += pPlayer->Collider3D()->GetWorldPos().y;
		Vec3 vDir = vPlayerPos - pObject->Transform()->GetRelativePos();

		vDir.Normalize();

		pScript->SetFrontDir(vDir);

		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)5;
		CEventMgr::GetInst()->AddEvent(info);

	}
	

	else if (m_fTime>4.5f)
	{
		FSM()->ChangeState(L"LoopEnd");
	}
}

void CGigantLoop::exit()
{
	m_fTime = 0.f;
	Animator3D()->FindAnim(L"Loop")->Reset();
}

void CGigantLoop::start()
{
	CState* pState = FSM()->FindState(L"Loop");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Loop", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CGigantLoop::update()
{

}

