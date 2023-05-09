#include "pch.h"
#include "CLemurianScript.h"

#include <Engine/CObjectHeight.h>
#include "CHPUIScript.h"

CLemurianScript::CLemurianScript()
	:CScript((int)SCRIPT_TYPE::LEMURIANSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(150)
	, m_CurHP(m_MaxHP)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CLemurianScript::CLemurianScript(const CLemurianScript& _origin)
	:CScript((int)SCRIPT_TYPE::LEMURIANSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(_origin.m_MaxHP)
	, m_CurHP(m_MaxHP)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CLemurianScript::~CLemurianScript()
{
	SAFE_DELETE(m_pBuffer);
}


void CLemurianScript::start()
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

void CLemurianScript::update()
{

	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Spawn");
	}

	CGameObject* pObject = GetOwner();
	if (m_pHPUI)
	{
		CHPUIScript* pScript = (CHPUIScript*)m_pHPUIScript;
		float f = ((float)m_CurHP / (float)m_MaxHP);
		pScript->SetHPRatio(f);
	}

}

void CLemurianScript::lateupdate()
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

void CLemurianScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (L"ApRound" == _OtherObject->GetName() && Animator3D()->GetCurAnim()->GetAnimClip().strAnimName != L"Death")
	{
		if (Animator3D()->GetCurAnim()->GetAnimClip().strAnimName == L"Idle" ||
			Animator3D()->GetCurAnim()->GetAnimClip().strAnimName == L"Run")
		{
			FSM()->ChangeState(L"Hurt");
		}
		else if (Animator3D()->GetCurAnim()->GetAnimClip().strAnimName == L"Hurt")
		{
			Animator3D()->GetCurAnim()->Reset();
		}

		m_CurHP -= 20;
		_OtherObject->Destroy();

		if (m_CurHP <= 0)
			FSM()->ChangeState(L"Death");
	}
}

