#include "pch.h"
#include "CSummon3Script.h"

#include <Engine/CEventMgr.h>

CSummon3Script::CSummon3Script()
	:CScript((int)SCRIPT_TYPE::SUMMON3SCRIPT)
{
}

CSummon3Script::~CSummon3Script()
{
}

void CSummon3Script::update()
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
			CGameObject* pObject = CResMgr::GetInst()->FindRes<CPrefab>(L"prefab\\Bison.pref")->Instantiate();
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

