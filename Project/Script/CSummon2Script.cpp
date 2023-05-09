#include "pch.h"
#include "CSummon2Script.h"

#include <Engine/CEventMgr.h>

CSummon2Script::CSummon2Script()
	:CScript((int)SCRIPT_TYPE::SUMMON2SCRIPT)
{
}

CSummon2Script::~CSummon2Script()
{
}


void CSummon2Script::update()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");

	if (!pPlayer)
		return;

	Vec3 vPos = Transform()->GetWorldPos();
	Vec3 pPlayerPos = pPlayer->Transform()->GetWorldPos();

	float len = (vPos - pPlayerPos).Length();

	if (len < 300.f)
	{
		int i = 1;
		Vec4 vColor = Vec4(0.1f, 0.f, 0.f, 1.f);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &i);
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);

		if (KEY_TAP(KEY::E))
		{
			Vec3 vPos = Transform()->GetRelativePos();
			CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Imp.pref")->Instantiate();
			pObject->Transform()->SetRelativePos(Vec3(vPos.x -200.f, vPos.y, vPos.z));

			tEventInfo info;
			info.eType = EVENT_TYPE::CREATE_OBJ;
			info.lParam = (DWORD_PTR)pObject;
			info.wParam = (DWORD_PTR)4;
			CEventMgr::GetInst()->AddEvent(info);

			pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Imp.pref")->Instantiate();
			pObject->Transform()->SetRelativePos(Vec3(vPos.x + 200.f, vPos.y, vPos.z));

			info.eType = EVENT_TYPE::CREATE_OBJ;
			info.lParam = (DWORD_PTR)pObject;
			info.wParam = (DWORD_PTR)4;
			CEventMgr::GetInst()->AddEvent(info);

			pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Lemurian.pref")->Instantiate();
			pObject->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, vPos.z +200.f));

			info.eType = EVENT_TYPE::CREATE_OBJ;
			info.lParam = (DWORD_PTR)pObject;
			info.wParam = (DWORD_PTR)4;
			CEventMgr::GetInst()->AddEvent(info);

			pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Lemurian.pref")->Instantiate();
			pObject->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, vPos.z - 200.f));

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
