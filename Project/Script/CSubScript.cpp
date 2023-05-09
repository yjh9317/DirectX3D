#include "pch.h"
#include "CSubScript.h"

#include <Engine/CObjectHeight.h>

#include "CHPUIScript.h"

CSubScript::CSubScript()
	:CScript((int)SCRIPT_TYPE::SUBSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(120)
	, m_CurHP(m_MaxHP)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CSubScript::CSubScript(const CSubScript& _origin)
	:CScript((int)SCRIPT_TYPE::SUBSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(120)
	, m_CurHP(_origin.m_MaxHP)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CSubScript::~CSubScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CSubScript::start()
{
	vector<CGameObject*> _v = GetOwner()->GetChild();

	for (int i = 0; i < _v.size(); ++i)
	{
		if (_v[i]->GetName() == "HPUI")
		{
			m_pHPUI = _v[i];
			m_pHPUIScript = m_pHPUI->GetScript<CHPUIScript>();
			return;
		}
	}
}

void CSubScript::update()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Spawn");
	}

	if (m_pHPUI)
	{
		CHPUIScript* pScript = (CHPUIScript*)m_pHPUIScript;
		pScript->SetHPRatio(((float)m_CurHP / (float)m_MaxHP));
	}
}

void CSubScript::lateupdate()
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

void CSubScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (L"ApRound" == _OtherObject->GetName() && Animator3D()->GetCurAnim()->GetAnimClip().strAnimName != L"Death")
	{
		m_CurHP -= 20;
		_OtherObject->Destroy();

		if (m_CurHP <= 0)
			FSM()->ChangeState(L"Death");
	}
}

