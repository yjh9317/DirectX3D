#include "pch.h"
#include "CBisonScript.h"

#include <Engine/CObjectHeight.h>
#include "CHPUIScript.h"

CBisonScript::CBisonScript()
	:CScript((int)SCRIPT_TYPE::BISONSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(400)
	, m_CurHP(400)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CBisonScript::CBisonScript(const CBisonScript& _origin)
	:CScript((int)SCRIPT_TYPE::BISONSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(400)
	, m_CurHP(400)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CBisonScript::~CBisonScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CBisonScript::start()
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

void CBisonScript::update()
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
		pScript->SetHPRatio(m_CurHP / (float)m_MaxHP);
	}

	CGameObject* pObject = GetOwner();

	if (m_iHit)
	{
		m_ftime += DT;
		if (m_ftime > 1.f)
		{
			m_ftime = 0.f;
			m_iHit = 0;
			MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_iHit);
		}
		float time = m_ftime;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::FLOAT_0, &time);
	}
}

void CBisonScript::lateupdate()
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


void CBisonScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (L"ApRound" == _OtherObject->GetName() && Animator3D()->GetCurAnim()->GetAnimClip().strAnimName != L"Death")
	{
		m_CurHP -= 20;
		_OtherObject->Destroy();

		if (m_CurHP <= 0)
			FSM()->ChangeState(L"Death");

		m_iHit = 1;
		m_ftime = 0;
		MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &m_iHit);
	}
}
