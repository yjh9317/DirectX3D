#include "pch.h"
#include "CChestScript.h"

#include <Engine/CObjectHeight.h>
#include <Engine/CCollider3D.h>

CChestScript::CChestScript()
	:CScript((int)SCRIPT_TYPE::CHESTSCRIPT)
	, m_bIdleCheck(false)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CChestScript::CChestScript(const CChestScript& _origin)
	:CScript((int)SCRIPT_TYPE::CHESTSCRIPT)
	, m_bIdleCheck(false)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CChestScript::~CChestScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CChestScript::update()
{
	Ptr<CObjectHeight> pUpdateShader = (CObjectHeight*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ObjectHeightShader").Get();

	Vec3 vPos = Transform()->GetWorldPos();
	pUpdateShader->SetHeightBuffer(m_pBuffer);
	pUpdateShader->SetPos(vPos);

	pUpdateShader->Excute();

	float f;
	m_pBuffer->GetData(&f);

	Vec3 vRelPos = Transform()->GetRelativePos();
	Vec3 vColliderScale = GetOwner()->Collider3D()->GetWorldScale();
	vRelPos.y = f * pUpdateShader->GetHeightScale() + vColliderScale.y/2.f;
	Transform()->SetRelativePos(vRelPos);

	pUpdateShader->Clear();

	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Idle");
	}
}

