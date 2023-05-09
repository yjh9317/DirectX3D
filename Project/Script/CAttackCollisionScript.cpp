#include "pch.h"
#include "CAttackCollisionScript.h"

CAttackCollisionScript::CAttackCollisionScript()
	:CScript((int)SCRIPT_TYPE::ATTACKCOLLISIONSCRIPT)
	, m_fCurTime(0.f)
	, m_fEndTime(0.5f)
{
}

CAttackCollisionScript::~CAttackCollisionScript()
{
}

void CAttackCollisionScript::update()
{
	m_fCurTime += DT;

	if (m_fEndTime < m_fCurTime)
		GetOwner()->Destroy();
}
