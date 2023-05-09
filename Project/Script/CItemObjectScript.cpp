#include "pch.h"
#include "CItemObjectScript.h"

#include <Engine/CObjectHeight.h>

CItemObjectScript::CItemObjectScript()
	:CScript((int)SCRIPT_TYPE::ITEMOBJECTSCRIPT)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CItemObjectScript::CItemObjectScript(const CItemObjectScript& _origin)
	:CScript((int)SCRIPT_TYPE::ITEMOBJECTSCRIPT)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CItemObjectScript::~CItemObjectScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CItemObjectScript::update()
{
	Ptr<CObjectHeight> pUpdateShader = (CObjectHeight*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ObjectHeightShader").Get();

	Vec3 vPos = Transform()->GetWorldPos();
	pUpdateShader->SetHeightBuffer(m_pBuffer);
	pUpdateShader->SetPos(vPos);

	pUpdateShader->Excute();

	float f;
	m_pBuffer->GetData(&f);

	Vec3 vRelPos = Transform()->GetRelativePos();
	vRelPos.y = f * pUpdateShader->GetHeightScale();
	Transform()->SetRelativePos(vRelPos);

	pUpdateShader->Clear();
}