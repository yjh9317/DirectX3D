#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"
#include <Engine/CFSM.h>

#include "CPlayerIdle.h"
#include <Engine/CState.h>

#include <Engine/CLandScape.h>
#include <Engine/CStructuredBuffer.h>
#include <Engine/CObjectHeight.h>

#include "CScreenUIScript.h"
#include "CBloodScreenScript.h"
#include "CCameraMoveScript.h"

CPlayerScript::CPlayerScript()
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pBloodScreen(nullptr)
	, m_pHPUI(nullptr)
	, m_pHPUIScript(nullptr)
	, m_fSpeed(150.f)
	, m_iMaxHP(200)
	, m_iCurHP(200)
	, m_pBuffer(nullptr)
	, m_fheight(0)
	, m_bIdleCheck(false)
{
	AddScriptParam("PlayerSpeed", SCRIPTPARAM_TYPE::FLOAT, &m_fSpeed);
	

	m_pBuffer = new CStructuredBuffer;
	m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CPlayerScript::CPlayerScript(const CPlayerScript& _origin)
	: CScript((int)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pBloodScreen(nullptr)
	, m_pHPUI(nullptr)
	, m_pHPUIScript(nullptr)
	, m_fSpeed(_origin.m_fSpeed)
	, m_iMaxHP(_origin.m_iMaxHP)
	, m_iCurHP(_origin.m_iMaxHP)
	, m_pBuffer(nullptr)
	, m_fheight(0)
	, m_bIdleCheck(false)
{
	if (!m_pBuffer)
	{
		m_pBuffer = new CStructuredBuffer;
		m_pBuffer->Create(sizeof(float), 1, SB_TYPE::READ_WRITE, true, nullptr);
	}
}

CPlayerScript::~CPlayerScript()
{
	if (m_pBuffer)
	{
		SAFE_DELETE(m_pBuffer);
	}
}

void CPlayerScript::start()
{	
	CGameObject* pObj = CSceneMgr::GetInst()->FindObjectByName(L"BloodScreen");
	if (pObj)
	{
		m_pBloodScreen = pObj;
	}
	pObj = CSceneMgr::GetInst()->FindObjectByName(L"DOF");
	if(pObj)
	{
		pObj->Activate();
	}
	pObj = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");
	CCameraMoveScript* pScript = pObj->GetScript<CCameraMoveScript>();
	pScript->SetCam(true);

	vector<CGameObject*> _v = GetOwner()->GetChild();

	for (int i = 0; i < _v.size(); ++i)
	{
		if (_v[i]->GetName() == L"PlayerHPUI")
		{
			m_pHPUI = _v[i];
			m_pHPUIScript = _v[i]->GetScript<CScreenUIScript>();
		}
	}

	if (CSceneMgr::GetInst()->GetCurScene()->GetSceneState() == SCENE_STATE::PLAY)
	{
		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bgm.mp3");
		pSound->Play(0, 0.1, true);
	}
}

void CPlayerScript::update()
{
	CGameObject* pMainCam = CSceneMgr::GetInst()->FindObjectByName(L"MainCamera");

	Vec3 vCameraPos = pMainCam->Transform()->GetWorldPos();
	Vec3 vCameraDir = pMainCam->Transform()->GetWorldFrontDir();

	Vec3 vFinalPos = (vCameraPos + vCameraDir * 10000);

	Vec3 Result = LookAtVector(Transform()->GetWorldPos(), vFinalPos, Transform()->GetWorldUpDir());

	Result.z = 0;
	RotationValueLimit(Result);
	Transform()->SetRelativeRotation(Result);

	CGameObject* pLandScape = CSceneMgr::GetInst()->FindObjectByName(L"LandScape");
	Vec3 vLandScale = pLandScape->Transform()->GetWorldScale();
	Vec3 vPos = Transform()->GetWorldPos();
	Ptr<CObjectHeight> pUpdateShader = (CObjectHeight*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ObjectHeightShader").Get();

	pUpdateShader->SetHeightBuffer(m_pBuffer);
	pUpdateShader->SetPos(vPos);
	pUpdateShader->Excute();

	float Height;
	m_pBuffer->GetData(&Height);
	
	Vec3 vRelPos = Transform()->GetRelativePos();
	vRelPos.y = Height * vLandScale.y;
	Transform()->SetRelativePos(vRelPos);
	
	pUpdateShader->Clear();

	// @@@@@@@@@@@

	CScreenUIScript* pScript = (CScreenUIScript*)m_pHPUIScript;
	float fHP = ((float)m_iCurHP / (float)m_iMaxHP);
	pScript->SetHPRatio(fHP);

	if (fHP < 0)
		m_iCurHP = m_iMaxHP/2.;
}	


void CPlayerScript::lateupdate()
{
	CState* pState = FSM()->FindState(L"Idle");
	if (nullptr == pState)
		return;
	else if(pState && !m_bIdleCheck)
	{
		m_bIdleCheck = true;
		FSM()->ChangeState(L"Idle");
	}
}



void CPlayerScript::OnCollisionEnter(CGameObject* _OtherObject)
{
	if (_OtherObject->GetLayerIndex() == 5 || _OtherObject->GetLayerIndex() == 4)
	{
		if (m_pBloodScreen)
		{
			m_pBloodScreen->Activate();
			CBloodScreenScript* pScript = m_pBloodScreen->GetScript<CBloodScreenScript>();
			pScript->SetTime(1);
		}
		m_iCurHP -= 20;
	}
}

void CPlayerScript::RotationValueLimit(Vec3& RadianValue)
{
	Vec3 Tmp = RadianValue;

	Tmp.ToDegree();

	if (!(Tmp.x > -10.f && Tmp.x < 10.f))
	{
		Tmp.x = std::clamp(Tmp.x, -10.f, 10.f);
	}
	Tmp.ToRadian();
	RadianValue = Tmp;
}

void CPlayerScript::SaveToScene(FILE* _pFile)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _pFile);
}

void CPlayerScript::LoadFromScene(FILE* _pFile)
{
	fread(&m_fSpeed, sizeof(float), 1, _pFile);
}