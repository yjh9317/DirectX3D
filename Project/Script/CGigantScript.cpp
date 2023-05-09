#include "pch.h"
#include "CGigantScript.h"

#include <Engine/CObjectHeight.h>
#include "CScreenUIScript.h"

CGigantScript::CGigantScript()
	:CScript((int)SCRIPT_TYPE::GIGANTSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(800)
	, m_CurHP(800)
{
	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CGigantScript::CGigantScript(const CGigantScript& _origin)
	:CScript((int)SCRIPT_TYPE::GIGANTSCRIPT)
	, m_pHPUI(nullptr)
	, m_pBuffer(nullptr)
	, m_bIdleCheck(false)
	, m_MaxHP(800)
	, m_CurHP(800)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CGigantScript::~CGigantScript()
{
	SAFE_DELETE(m_pBuffer);
}

void CGigantScript::start()
{
	vector<CGameObject*> _v = GetOwner()->GetChild();

	for (int i = 0; i < _v.size(); ++i)
	{
		if (_v[i]->GetName() == L"HPUI")
		{
			m_pHPUI = _v[i];
			m_pHPUIScript = _v[i]->GetScript<CScreenUIScript>();
		}
	}
}

void CGigantScript::update()
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

	if (m_pHPUI)
	{
		CScreenUIScript* pScript = (CScreenUIScript*)m_pHPUIScript;
		pScript->SetHPRatio((float)m_CurHP / (float)m_MaxHP);
	}

	CGameObject* pPlayer = CSceneMgr::GetInst()->FindObjectByName(L"Player");
	if (nullptr == pPlayer)
		return;

	Vec3 vPlayerPos = pPlayer->Transform()->GetWorldPos();
	Vec3 vMonsterPos = Transform()->GetWorldPos();

	Vec2 vPlayerXZ = Vec2(vPlayerPos.x, vPlayerPos.z);
	Vec2 vMonsterXZ = Vec2(vMonsterPos.x, vMonsterPos.z);

	Vec2 vDir = vPlayerXZ - vMonsterXZ;
	vDir.Normalize();

	Vec2 XZ = Vec2(0.f, 1.f);
	XZ.Normalize();
	float angle = XZ.Dot(vDir);

	float acos = acosf(angle);

	if (vDir.x < 0.f)
		acos = -acos;
	Vec3 vRot = Transform()->GetRelativeRotation();
	vRot.y = acos;
	Transform()->SetRelativeRotation(vRot);
}

void CGigantScript::lateupdate()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
		return;
	else if (pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Idle");
	}
}

void CGigantScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (L"ApRound" == _OtherObject->GetName() && Animator3D()->GetCurAnim()->GetAnimClip().strAnimName != L"Death")
	{
		m_CurHP -= 20;
		_OtherObject->Destroy();

		if (m_CurHP <= 0)
			FSM()->ChangeState(L"Death");
	}
}

