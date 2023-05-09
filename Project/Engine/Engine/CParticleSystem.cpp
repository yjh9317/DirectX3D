#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CTransform.h"
#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000)
	, m_bPosInherit(0)
	, m_iAliveCount(4)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(2.f)
	, m_fStartSpeed(100.f)
	, m_fEndSpeed(10.f)
	, m_vStartColor(Vec4(1.f, 0.2f, 0.7f, 1.f))
	, m_vEndColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vStartScale(Vec3(10.f, 10.f, 1.f))
	, m_vEndScale(Vec3(1.f, 1.f, 1.f))
	, m_fParticleCreateDistance(50.f)
	, m_fParticleCreateTerm(0.02f)
	, m_iEmissive(0)
	, m_iLighting(0)

{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"), 0);

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();
	
	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);


	GetMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\particle\\AlphaCircle.png"));
}

CParticleSystem::CParticleSystem(const CParticleSystem& _origin)
	: CRenderComponent(_origin)
	, m_iMaxCount(_origin.m_iMaxCount)
	, m_bPosInherit(_origin.m_bPosInherit)
	, m_iAliveCount(_origin.m_iAliveCount)
	, m_fAccTime(_origin.m_fAccTime)
	, m_fMinLifeTime(_origin.m_fMinLifeTime)
	, m_fMaxLifeTime(_origin.m_fMaxLifeTime)
	, m_fStartSpeed(_origin.m_fStartSpeed)
	, m_fEndSpeed(_origin.m_fEndSpeed)
	, m_vStartColor(_origin.m_vStartColor)
	, m_vEndColor(_origin.m_vEndColor)
	, m_vStartScale(_origin.m_vStartScale)
	, m_vEndScale(_origin.m_vEndScale)
	, m_fParticleCreateDistance(_origin.m_fParticleCreateDistance)
	, m_fParticleCreateTerm(_origin.m_fParticleCreateTerm)
	, m_iEmissive(_origin.m_iEmissive)
	, m_iLighting(_origin.m_iLighting)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\ParticleRenderMtrl.mtrl"), 0);

	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, false, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}


void CParticleSystem::SetEmissive(bool _bEmissive)
{
	m_iEmissive = (int)_bEmissive;
}

void CParticleSystem::SetApplyLighting(bool _bLighting)
{
	m_iLighting = (int)_bLighting;
}

void CParticleSystem::SetMaxParticleCount(UINT _iMax)
{
	if (m_iMaxCount < _iMax)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), _iMax, SB_TYPE::READ_WRITE, false, nullptr);
	}
	m_iMaxCount = _iMax;
}

void CParticleSystem::finalupdate()
{
	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;		

		tParticleData data = {};
		data.iAliveCount = m_iAliveCount;
		m_DataBuffer->SetData(&data, 1);
	}
		
	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);

	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());

	m_CS->Excute();	
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);
		
	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_2, &m_iLighting);
	GetMaterial(0)->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);	

	m_ParticleBuffer->Clear();
}

void CParticleSystem::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	wstring strKey;
	if(nullptr != m_CS)
		strKey = m_CS->GetKey();
	SaveWStringToFile(strKey, _pFile);
	
	fwrite(&m_iMaxCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_bPosInherit, sizeof(int), 1, _pFile);
	fwrite(&m_iAliveCount, sizeof(int), 1, _pFile);
	fwrite(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fwrite(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
	fwrite(&m_iEmissive, sizeof(int), 1,_pFile);
	fwrite(&m_iLighting, sizeof(int), 1, _pFile);
}

void CParticleSystem::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	wstring strKey;
	LoadWStringFromFile(strKey, _pFile);
	m_CS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();

	UINT iMaxCount = 0;
	fread(&iMaxCount, sizeof(UINT), 1, _pFile);
	SetMaxParticleCount(iMaxCount);

	fread(&m_bPosInherit, sizeof(int), 1, _pFile);
	fread(&m_iAliveCount, sizeof(int), 1, _pFile);
	fread(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fread(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fread(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fread(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
	fread(&m_iEmissive, sizeof(int), 1, _pFile);
	fread(&m_iLighting, sizeof(int), 1, _pFile);
}

void CParticleSystem::SaveJsonFile(Json::Value& root)
{
	Json::Value ParticleSystem;

	ParticleSystem["PosInherit"] = m_bPosInherit;
	ParticleSystem["AliveCount"] = m_iAliveCount;
	ParticleSystem["MinLifeTime"] = m_fMinLifeTime;
	ParticleSystem["MaxLifeTime"] = m_fMaxLifeTime;
	ParticleSystem["StartSpeed"] = m_fStartSpeed;
	ParticleSystem["EndSpeed"] = m_fEndSpeed;

	VecToJson(&ParticleSystem, "StartColor", m_vStartColor);
	VecToJson(&ParticleSystem, "EndColor", m_vEndColor);
	VecToJson(&ParticleSystem, "StartScale", m_vStartScale);
	VecToJson(&ParticleSystem, "EndScale", m_vEndScale);

	ParticleSystem["ParticleCreateDistance"] = m_fParticleCreateDistance;
	ParticleSystem["ParticleCreateTerm"] = m_fParticleCreateTerm;
	ParticleSystem["Emissive"] = m_iEmissive;
	ParticleSystem["Lighting"] = m_iLighting;

	root["PARTICLESYSTEM"] = ParticleSystem;
}

void CParticleSystem::LoadJsonFromFile(Json::Value& root)
{
	Json::Value ParticleSystem = root;


	m_bPosInherit  = ParticleSystem["PosInherit"].asInt();
	m_iAliveCount  = ParticleSystem["AliveCount"].asInt();
	m_fMinLifeTime = ParticleSystem["MinLifeTime"].asFloat();
	m_fMaxLifeTime = ParticleSystem["MaxLifeTime"].asFloat();
	m_fStartSpeed  = ParticleSystem["StartSpeed"].asFloat();
	m_fEndSpeed    = ParticleSystem["EndSpeed"].asFloat();
	
	LoadVecFromJson(ParticleSystem["StartColor"], m_vStartColor);
	LoadVecFromJson(ParticleSystem["EndColor"], m_vStartColor);
	LoadVecFromJson(ParticleSystem["StartScale"], m_vStartColor);
	LoadVecFromJson(ParticleSystem["EndScale"], m_vStartColor);

	m_fParticleCreateDistance = ParticleSystem["ParticleCreateDistance"].asFloat();
	m_fParticleCreateTerm = ParticleSystem["ParticleCreateTerm"].asFloat();
	m_iEmissive = ParticleSystem["Emissive"].asInt();
	m_iLighting = ParticleSystem["Lighting"].asInt();
}
