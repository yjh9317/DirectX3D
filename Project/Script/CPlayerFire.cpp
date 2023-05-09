#include "pch.h"
#include "CPlayerFire.h"

#include <Engine/CCollider3D.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CEventMgr.h>
#include "CBulletScript.h"

CPlayerFire::CPlayerFire()
	:CScript((int)SCRIPT_TYPE::PLAYERFIRE)
{
}

CPlayerFire::~CPlayerFire()
{
}


void CPlayerFire::enter()
{
	Animator3D()->FindAnim(L"Fire")->SetSpeed(2.f);
	m_LeftShotNum = 0;
	m_RightShotNum = 0;
}

void CPlayerFire::StateUpdate()
{
	if (KEY_TAP(KEY::A) || KEY_TAP(KEY::D) || KEY_TAP(KEY::S) || KEY_TAP(KEY::W))
	{
		FSM()->ChangeState(L"Move");
	}

	if (KEY_PRESSED(KEY::LBTN))
	{
		Animator3D()->Play(L"Fire", true);
	}


	int check = Animator3D()->GetCurAnim()->GetCurFrameIdx();
	if ((Animator3D()->GetCurAnim()->GetCurFrameIdx() == 425 || Animator3D()->GetCurAnim()->GetCurFrameIdx() == 450)&& !m_bFire)
	{
		m_bFire = true;
		CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\ApRound.pref")->Instantiate();


		// 날라갈 방향

		Vec3 vDir = Transform()->GetWorldFrontDir();

		CGameObject* pCam = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");
		Vec3 vCamDir = pCam->Transform()->GetWorldFrontDir();
		// 총알의 회전값
		Vec3 vPos = Transform()->GetRelativePos();
		
		Vec3 vBulletPos = vPos;
		vBulletPos += vDir * 20.f;

		Vec3 vUpDir = Transform()->GetWorldUpDir();
		Vec3 zAxis = -vDir;
		Vec3 xAxis = vUpDir.Cross(zAxis).Normalize();
		Vec3 yAxis = zAxis.Cross(xAxis);

		Matrix Mat = {};
		Mat._11 = xAxis.x;		Mat._21 = yAxis.x;	  	Mat._31 = zAxis.x;
		Mat._12 = xAxis.y;		Mat._22 = yAxis.y;	  	Mat._32 = zAxis.y;
		Mat._13 = xAxis.z;		Mat._23 = yAxis.z;	  	Mat._33 = zAxis.z;
		Mat._14 = -xAxis.Dot(-vDir); Mat._24 = -yAxis.Dot(-vDir); Mat._34 = -zAxis.Dot(-vDir); Mat._44 = 1;

		Vec3 Result = DecomposeRotMat(Mat);
		CBulletScript* pScript = pObject->GetScript<CBulletScript>();
		pScript->SetSpeed(1000.f);
		pScript->SetFrontDir(vDir);

		vBulletPos.y += GetOwner()->Collider3D()->GetWorldScale().y * 2/ 3.f;
		pObject->Transform()->SetRelativePos(vBulletPos);
		pObject->Transform()->SetRelativeScale(Vec3(10.f, 10.f, 10.f));
		pObject->Transform()->SetRelativeRotation(Result);
		
		tEventInfo info;
		info.eType = EVENT_TYPE::CREATE_OBJ;
		info.lParam = (DWORD_PTR)pObject;
		info.wParam = (DWORD_PTR)3;
		CEventMgr::GetInst()->AddEvent(info);

		m_bSound = true;
	}
	else if ((Animator3D()->GetCurAnim()->GetCurFrameIdx() != 425 && Animator3D()->GetCurAnim()->GetCurFrameIdx() != 450) && m_bFire)
	{
		m_bFire = false;
	}

	if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 425 && m_bSound)
	{
		if (m_LeftShotNum == 0)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_01.ogg");
			pSound->Play(1, 0.1, true);
		}
		else if (m_LeftShotNum == 1)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_03.ogg");
			pSound->Play(1, 0.1, true);
		}
		else if (m_LeftShotNum == 2)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_05.ogg");
			pSound->Play(1, 0.1, true);
		}
		++m_LeftShotNum;
		if (m_LeftShotNum > 2)
		{
			m_LeftShotNum = 0;
		}
		m_bSound = false;
	}
	else if (Animator3D()->GetCurAnim()->GetCurFrameIdx() == 450 && m_bSound)
	{
		if (m_RightShotNum == 0)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_02.ogg");
			pSound->Play(1, 0.1, true);
		}
		else if (m_RightShotNum == 1)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_04.ogg");
			pSound->Play(1, 0.1, true);
		}
		else if (m_RightShotNum == 2)
		{
			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\char_commando_M1_v4_06.ogg");
			pSound->Play(1, 0.1, true);
		}
		++m_RightShotNum;
		if (m_RightShotNum > 2)
		{
			m_RightShotNum = 0;
		}
		m_bSound = false;
	}

}

void CPlayerFire::exit()
{
	m_bFire = false;
}

void CPlayerFire::start()
{
	CState* pState = FSM()->FindState(L"Fire");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Fire", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CPlayerFire::update()
{
}
