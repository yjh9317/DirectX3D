#include "pch.h"
#include "CDirectionalLightScript.h"

#include <Engine/CLandScape.h>

CDirectionalLightScript::CDirectionalLightScript()
	:CScript((int)SCRIPT_TYPE::DIRECTIONALLIGHTSCRIPT)
{
}

CDirectionalLightScript::~CDirectionalLightScript()
{
}


void CDirectionalLightScript::start()
{
	CGameObject* pLand = CSceneMgr::GetInst()->FindObjectByName(L"LandScape");

	if(CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
		pLand->LandScape()->SetDynamicShadow(true);

	Transform()->SetRelativeScale(20000.f, 20000.f, 20000.f);
}

void CDirectionalLightScript::update()
{
	CGameObject* pObject = CSceneMgr::GetInst()->FindObjectByName(L"Player");

	if (nullptr == pObject)
		return;

	Vec3 vPlayerPos = pObject->Transform()->GetWorldPos();

	Vec3 vPos = Vec3(vPlayerPos.x, vPlayerPos.y + 2000.f, vPlayerPos.z - 1100.f);

	Vec3 vRot = Vec3(60.f, 0.f, 0.f);
	vRot.ToRadian();

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRotation(vRot);
}

