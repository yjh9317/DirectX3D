#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CCore.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTimeMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CCollider3D.h>

#include <Engine/func.h>

CCameraMoveScript::CCameraMoveScript()
	: CScript((int)SCRIPT_TYPE::CAMERAMOVESCRIPT)
	, m_Filter(nullptr)
	, m_fCamSpeed(100.f)
	, m_fDist(350.f)
	, m_fCameraYDist(500)
{
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::start()
{
	srand((unsigned)time(NULL));
	unsigned int init[32];
	for (int i = 0; i < 32; i++) {
		init[i] = rand() << 16 | rand();
		// WELL Random 을 초기화 하기 위해, C 표준 rand() 함수를 이용하여 init 값을 생성합니다
	}
	InitWELLRNG1024a(init);

	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pScene->GetLayer(2); // Player Layer
	vector<CGameObject*> vObj = pLayer->GetRootObjects();

	for (int i = 0; i < vObj.size(); ++i)
	{
		if (vObj[i]->GetName() == L"Player")
		{
			m_Player = vObj[i];
		}
	}

}

void CCameraMoveScript::update()
{

	// 1번은 카메라가 자유롭게 이동하고 마우스가 고정 X
	if (KEY_TAP(KEY::_1))
	{
		m_bType = false;
	}
	// 2번은 마우스 고정
	else if (KEY_TAP(KEY::_2))
	{
		m_bType = true;
	}


	if (!m_bType)
	{
		Vec3 vPos = Transform()->GetRelativePos();
		float fSpeed = m_fCamSpeed;

		if (KEY_PRESSED(KEY::LSHFT))
		{
			fSpeed *= 10.f;
		}

		if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
		{
			if (KEY_PRESSED(KEY::W) || KEY_PRESSED(KEY::UP))
			{
				Vec3 vFront = Transform()->GetWorldFrontDir();
				vPos += DT * vFront * fSpeed;
			}

			if (KEY_PRESSED(KEY::S) || KEY_PRESSED(KEY::DOWN))
			{
				Vec3 vFront = Transform()->GetWorldFrontDir();
				vPos -= DT * vFront * fSpeed;
			}

			if (KEY_PRESSED(KEY::A) || KEY_PRESSED(KEY::LEFT))
			{
				Vec3 vRight = Transform()->GetWorldRightDir();
				vPos -= DT * vRight * fSpeed;
			}

			if (KEY_PRESSED(KEY::D) || KEY_PRESSED(KEY::RIGHT))
			{
				Vec3 vRight = Transform()->GetWorldRightDir();
				vPos += DT * vRight * fSpeed;
			}
		}

		else
		{
			if (KEY_PRESSED(KEY::W))
			{
				vPos.y += DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::S))
			{
				vPos.y -= DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::A))
			{
				vPos.x -= DT * fSpeed;
			}

			if (KEY_PRESSED(KEY::D))
			{
				vPos.x += DT * fSpeed;
			}
		}

		if (nullptr != m_Filter && KEY_TAP(KEY::F))
		{
			if (m_Filter->IsActive())
				m_Filter->Deactivate();
			else
				m_Filter->Activate();
		}




		if (KEY_PRESSED(KEY::RBTN))
		{
			Vec3 vRot = Transform()->GetRelativeRotation();

			Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
			vRot.y += DT * vMouseDir.x * XM_PI;
			vRot.x -= DT * vMouseDir.y * XM_PI;

			Transform()->SetRelativeRotation(vRot);
		}

		Transform()->SetRelativePos(vPos);
	}
	// 진동 만들긴햇는데 허접함;;
	else if (m_bType)
	{
		if (KEY_TAP(KEY::_3))
		{
			m_bVib = true;
		}
		if (m_bVib)
		{
			static float fTime;
			fTime += DT;
			static float xx = 0;
			if (fTime > 1.8f)
			{
				m_bVib = false;
				fTime = 0;
				xx = 0;
			}
			Vec3 vPos = Transform()->GetRelativePos();

			xx += DT * 5;

			float f = sin(xx * 20.0f) * powf(0.8f, xx);

			//double a = (WELLRNG1024a() - 0.5)* 2 ;
			//double b = (WELLRNG1024a() - 0.5) * 2;
			//double c = (WELLRNG1024a() - 0.5) * 2;
			//vPos += Vec3(a,b,c);
			vPos += Vec3(f, 0, 0.f);
			Transform()->SetRelativePos(vPos);
			return;
		}


		// 마우스 고정시키기
		HWND hFocusedHwnd = GetFocus();
		if (hFocusedHwnd)
		{
			POINT pt;
			pt.x = CDevice::GetInst()->GetRenderResolution().x;
			pt.y = CDevice::GetInst()->GetRenderResolution().y;
			ScreenToClient(CCore::GetInst()->GetMainHwnd(), &pt);
			SetCursorPos(pt.x, pt.y);
		}
		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(CCore::GetInst()->GetMainHwnd(), &ptMouse);
	
		float x = ptMouse.x;
		float y = ptMouse.y;

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMousePos() - Vec2(x,y);		
		vMouseDir.Normalize();
		vMouseDir.y = -vMouseDir.y;	// Window와 Directx의 Y방향은 반대

		Vec3 CameraRotation = Transform()->GetRelativeRotation();
		CameraRotation.y += vMouseDir.x * DT * 4.;
		CameraRotation.x -= vMouseDir.y * DT * 4.;


		if (CameraRotation.x > 10 * XM_PI / 180.f)
			CameraRotation.x = 10 * XM_PI / 180.f;
		else if (CameraRotation.x < (-10.f * XM_PI /180.f))
			CameraRotation.x = -10.f * XM_PI / 180.f;

		Transform()->SetRelativeRotation(CameraRotation);

		// 플레이어의 회전값세팅
		if(m_Player)
		{
				Vec3 vPlayerPos = m_Player->Transform()->GetRelativePos();
				Vec3 vCamPos = vPlayerPos;

				vCamPos.x = vPlayerPos.x - m_fDist * sinf(CameraRotation.y);
				vCamPos.z = vPlayerPos.z - m_fDist * cosf(CameraRotation.y);
				vCamPos.y = vPlayerPos.y + 500.f * sinf(CameraRotation.x);

				if (vCamPos.y < vPlayerPos.y + 120.f)
					vCamPos.y = vPlayerPos.y + 120.f;
				Transform()->SetRelativePos(vCamPos);
			}
		}
		// 안개
	if (KEY_TAP(KEY::_4))
	{
		m_Filter = CSceneMgr::GetInst()->FindObjectByName(L"DOF");

		if (m_Filter->IsActive())
		{
			m_Filter->Deactivate();
		}
		else
		{
			m_Filter->Activate();
		}
	}
}
