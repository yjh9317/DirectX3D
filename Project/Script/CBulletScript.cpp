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

	// �޽��� 0,0,-1�� �ٶ󺸰��ִµ� 0,0,1�� ��� ������ �� ���� ���� Dir�޾Ƽ� �ݴ�� ������
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vFrontDir = Transform()->GetWorldFrontDir();
	vPos += m_fSpeed * DT * m_vFrontDir;
	Transform()->SetRelativePos(vPos);
}

