#include "pch.h"
#include "CBulletScript.h"

CBulletScript::CBulletScript()
	:CScript((int)SCRIPT_TYPE::BULLETSCRIPT)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::update()
{

	m_fTime += DT;

	if (m_fTime > 4.f)
	{
		GetOwner()->Destroy();
	}

	// 메쉬가 0,0,-1를 바라보고있는데 0,0,1로 어떻게 돌리는 지 몰라서 따로 Dir받아서 반대로 세팅함
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFrontDir = Transform()->GetWorldFrontDir();
	vPos += m_fSpeed * DT * m_vFrontDir;
	Transform()->SetRelativePos(vPos);
}

