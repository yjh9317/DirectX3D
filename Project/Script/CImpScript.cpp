#include "pch.h"
#include "CImpScript.h"

#include <Engine/CCollider3D.h>
#include <Engine/CObjectHeight.h>
#include <Engine/CState.h>

#include "CHPUIScript.h"


CImpScript::CImpScript()
	:CScript((int)SCRIPT_TYPE::IMPSCRIPT)
	,m_bIdleCheck(false)
	, m_fSpeed(100.f)
	, m_MaxHP(120.f)
	, m_CurHP(120.f)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CImpScript::CImpScript(const CImpScript& _origin)
	:CScript((int)SCRIPT_TYPE::IMPSCRIPT)
	, m_bIdleCheck(_origin.m_bIdleCheck)
	, m_fSpeed(_origin.m_fSpeed)
	, m_MaxHP(120.f)
	, m_CurHP(120.f)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CImpScript::~CImpScript()
{
	SAFE_DELETE(m_pBuffer);
}


void CImpScript::enter()
{

}

void CImpScript::start()
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

void CImpScript::update()
{
	CState* pState = FSM()->FindState(L"Spawn");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Spawn");
	}

	CGameObject* pGameObject = GetOwner();
	Ptr<CMaterial> pMtrl = MeshRender()->GetDynamicMaterial(0);

	if (m_pHPUI)
	{
		CHPUIScript* pScript = (CHPUIScript*)m_pHPUIScript;
		pScript->SetHPRatio(float(m_CurHP / (float)m_MaxHP));
	}

	
}

void CImpScript::lateupdate()
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

void CImpScript::OnCollisionEnter(CGameObject* _OtherObject)
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

void CImpScript::SaveToScene(FILE* _pFile)
{
}

void CImpScript::LoadFromScene(FILE* _pFile)
{
}

