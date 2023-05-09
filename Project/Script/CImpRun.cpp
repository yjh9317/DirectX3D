#include "pch.h"
#include "CImpRun.h"


CImpRun::CImpRun()
	:CScript((int)SCRIPT_TYPE::IMPRUN)
{
}

CImpRun::~CImpRun()
{
}

void CImpRun::enter()
{
	Animator3D()->Play(L"Run", true);
}

void CImpRun::StateUpdate()
{
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = pScene->GetLayer(2);
	vector<CGameObject*> vObj = pLayer->GetRootObjects();

	Vec2 PlayerXZPos;

	for (int i = 0; i < vObj.size(); ++i)
	{
		CGameObject* pObject = vObj[i];
		if (pObject->GetName() == L"Player")
		{
			PlayerXZPos.x = pObject->Transform()->GetWorldPos().x;
			PlayerXZPos.y = pObject->Transform()->GetWorldPos().z;
		}
	}

	Vec2 MonsterXZPos;
	MonsterXZPos.x = Transform()->GetWorldPos().x;
	MonsterXZPos.y = Transform()->GetWorldPos().z;


	Vec2 vDir = PlayerXZPos - MonsterXZPos;
	vDir.Normalize();


	Vec2 XZ = Vec2(0.f, 1.f);
	XZ.Normalize();
	float angle = XZ.Dot(vDir);

	float acos = acosf(angle);
	float fresult = (acos / XM_PI) * 180;

	if (vDir.x < 0.f)
		acos = -acos;
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.y = acos;
	Transform()->SetRelativeRotation(vRot);



	Vec3 vFrontDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	Vec3 vPos = Transform()->GetWorldPos();
	vPos += vFrontDir * DT * 100;
	Transform()->SetRelativePos(vPos);

	float dist = (PlayerXZPos - MonsterXZPos).Length();
	if (100.f > dist)
	{
		FSM()->ChangeState(L"Attack");
	}
}

void CImpRun::exit()
{
}

void CImpRun::start()
{
	CState* pState = FSM()->FindState(L"Run");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Run", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpRun::update()
{

}

