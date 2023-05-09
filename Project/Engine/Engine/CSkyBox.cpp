#include "pch.h"
#include "CSkyBox.h"

#include "CTransform.h"


CSkyBox::CSkyBox()
	: CRenderComponent(COMPONENT_TYPE::SKYBOX)
	, m_eType(SKYBOX_TYPE::SPHERE)
{
	SetSkyboxType(m_eType);
}

CSkyBox::~CSkyBox()
{
}

void CSkyBox::finalupdate()
{

}

void CSkyBox::UpdateData()
{
	GetMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, (int*)&m_eType);
}

void CSkyBox::render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial(0))
		return;
	
	UpdateData();


	Transform()->UpdateData();
	GetMaterial(0)->UpdateData();
	GetMesh()->render(0);
}

void CSkyBox::SetSkyboxType(SKYBOX_TYPE _eType)
{
	m_eType = _eType;

	if (SKYBOX_TYPE::SPHERE == m_eType)
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	else
		SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));

	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\SkyBoxMtrl.mtrl"), 0);
}

void CSkyBox::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	// SkyBox Type
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

void CSkyBox::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	// SkyBox Type
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetSkyboxType(m_eType);
}

void CSkyBox::SaveJsonFile(Json::Value& root)
{
	Json::Value SkyBox;

	SkyBox["Type"] = (int)m_eType;

	root["SKYBOX"] = SkyBox;
}

void CSkyBox::LoadJsonFromFile(Json::Value& root)
{ 
	Json::Value SkyBox = root;

	m_eType = (SKYBOX_TYPE)SkyBox["Type"].asInt();
}
