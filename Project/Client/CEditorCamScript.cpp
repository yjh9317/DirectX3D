#include "pch.h"
#include "CEditorCamScript.h"

#include <Engine\CKeyMgr.h>
#include <Engine\CTimeMgr.h>

#include <Engine\CTransform.h>
#include <Engine\CCamera.h>

#include <Engine/CSceneMgr.h>
#include <Engine/CScene.h>
#include <Engine/CLayer.h>

CEditorCamScript::CEditorCamScript()
	: CScript(-1)
	, m_fSpeed(200.f)
	, m_fDist(400.f)
{
}

CEditorCamScript::~CEditorCamScript()
{
}

void CEditorCamScript::update()
{

	Vec3 vPos = Transform()->GetRelativePos();
	float fSpeed = m_fSpeed;

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
