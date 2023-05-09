#include "pch.h"
#include "CShrineScript.h"

#include "CCameraMoveScript.h"
#include <Engine/CEventMgr.h>

CShrineScript::CShrineScript()
	:CScript((int)SCRIPT_TYPE::SHRINESCRIPT)
{
}

CShrineScript::~CShrineScript()
{
}

void CShrineScript::update()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");

	if (!pPlayer)
		return;

	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 pPlayerPos = pPlayer->Transform()->GetWorldPos();

	float len = (vPos - pPlayerPos).Length();

	if (len < 1000.f)
	{
		int i = 1;
		Vec4 vColor = Vec4(0.1f,0.f,0.f,1.f);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &i);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);

		if (KEY_TAP(KEY::E))
		{
			//CGameObject* pCam = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");
			//CCameraMoveScript* pScript = pCam->GetScript<CCameraMoveScript>();
			//pScript->SetVibration(true);

			CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Gigant.pref")->Instantiate();
			pObject->Transform()->SetRelativePos(Transform()->GetRelativePos());
			
			tEventInfo info;
			info.eType = EVENT_TYPE::CREATE_OBJ;
			info.lParam = (DWORD_PTR)pObject;
			info.wParam = (DWORD_PTR)4;
			CEventMgr::GetInst()->AddEvent(info);

			GetOwner()->Destroy();
		}
	}
	else
	{
		int i = 0;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	}
}
