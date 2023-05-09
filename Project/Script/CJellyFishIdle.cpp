#include "pch.h"
#include "CJellyFishIdle.h"

#include <Engine/CCollider3D.h>

CJellyFishIdle::CJellyFishIdle()
	:CScript((int)SCRIPT_TYPE::JELLYFISHIDLE)
{
}

CJellyFishIdle::~CJellyFishIdle()
{
}

void CJellyFishIdle::enter()
{
	Animator3D()->Play(L"Idle", true);
}

void CJellyFishIdle::StateUpdate()
{
	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");

	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vPlayerColPos = pPlayer->Collider3D()->GetOffsetPos();
	vPlayerPos.y += pPlayer->Collider3D()->GetWorldScale().y / 2.f + vPlayerColPos.y;
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec3 vDir = vPlayerPos - vMonsterPos;
	vDir.Normalize();

	Vec3 vUpDir = Transform()->GetWorldUpDir();

	//https://learn.microsoft.com/ko-kr/windows/win32/direct3d9/d3dxmatrixlookatlh

	Vec3 zAxis = vDir;
	Vec3 xAxis = vUpDir.Cross(zAxis).Normalize();
	Vec3 yAxis = zAxis.Cross(xAxis);

	Matrix Mat = {};
	Mat._11 = xAxis.x;		Mat._21 = yAxis.x;	  	Mat._31 = zAxis.x;
	Mat._12 = xAxis.y;		Mat._22 = yAxis.y;	  	Mat._32 = zAxis.y;
	Mat._13 = xAxis.z;		Mat._23 = yAxis.z;	  	Mat._33 = zAxis.z;
	Mat._14 = -xAxis.Dot(vPlayerPos); Mat._24 = -yAxis.Dot(vPlayerPos); Mat._34 = -zAxis.Dot(vPlayerPos); Mat._44 = 1;

	Vec3 Result = DecomposeRotMat(Mat);

	Transform()->SetRelativeRotation(Result);

	Vec3 vPos = Transform()->GetRelativePos();
	vPos += vDir * 100 * DT;
	Transform()->SetRelativePos(vPos);
}

void CJellyFishIdle::exit()
{
}

void CJellyFishIdle::start()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Idle", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CJellyFishIdle::update()
{

}

