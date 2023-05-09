#include "pch.h"
#include "CMedikitScript.h"

#include "CPlayerScript.h"
#include <Engine/CObjectHeight.h>

CMedikitScript::CMedikitScript()
	:CScript((int)SCRIPT_TYPE::MEDIKITSCRIPT)
	, m_fPower(200)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CMedikitScript::CMedikitScript(const CMedikitScript& _origin)
	:CScript((int)SCRIPT_TYPE::MEDIKITSCRIPT)
	, m_pBuffer(nullptr)
	, m_fPower(200)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CMedikitScript::~CMedikitScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CMedikitScript::update()
{
	float f;

	Ptr<CObjectHeight> pUpdateShader = (CObjectHeight*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ObjectHeightShader").Get();

	Vec3 vPos = Transform()->GetWorldPos();
	pUpdateShader->SetHeightBuffer(m_pBuffer);
	pUpdateShader->SetPos(vPos);

	pUpdateShader->Excute();
	
	m_pBuffer->GetData(&f);
	pUpdateShader->Clear();

	

	if (m_b)
	{
		Vec3 vRelPos = Transform()->GetRelativePos();
		vRelPos.y = f * pUpdateShader->GetHeightScale();
		Transform()->SetRelativePos(vRelPos);
	}
	else
	{
		float height = f * pUpdateShader->GetHeightScale();

		Vec3 vRelPos = Transform()->GetRelativePos();
		vRelPos += DT * -m_vDir * 100;

		vRelPos.y += DT * m_fPower;

		m_fPower -= 2.f;

		if (f > vRelPos.y)
			m_b = true;
		
		Transform()->SetRelativePos(vRelPos);
	}
}

void CMedikitScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetName() == L"Player")
	{
		CPlayerScript* pScript = _OtherObject->GetScript<CPlayerScript>();
		float fHP = pScript->GetHP();
		pScript->SetHP(fHP + 50.f);

		GetOwner()->Destroy();
	}
}

