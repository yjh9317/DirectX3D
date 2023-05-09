#include "pch.h"
#include "CGCScript.h"

#include <Engine/CCollider3D.h>

CGCScript::CGCScript()
	:CScript((int)SCRIPT_TYPE::GCSCRIPT)
{
}

CGCScript::~CGCScript()
{
}


void CGCScript::update()
{
	m_fTime += DT;

	if (m_fTime > 8.f)
	{
		GetOwner()->Destroy();
	}

	CCollider3D* pCol = GetOwner()->Collider3D();

	pCol->SetOffsetScale(Vec3(100.f, 100.f, 100.f));

	Vec3 vPos = Transform()->GetRelativePos();
	vPos += m_fSpeed * DT * m_vFrontDir;
	Transform()->SetRelativePos(vPos);

	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.x += DT * 2;
	vRot.z += DT * 2;
	Transform()->SetRelativeRotation(vRot);
}

