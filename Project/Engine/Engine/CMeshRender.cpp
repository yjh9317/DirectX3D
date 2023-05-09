#include "pch.h"
#include "CMeshRender.h"

#include "CMesh.h"
#include "CMaterial.h"

#include "CTransform.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CSocket.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
	
{
}

CMeshRender::~CMeshRender()
{

}


void CMeshRender::finalupdate()
{
}

void CMeshRender::render()
{
	if (nullptr == GetMesh())
		return;

	// ==========
	// UpdateData
	// ==========

	// Transform ������Ʈ
	Transform()->UpdateData();

	// Animation2D ������Ʈ
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}


	// Animator3D ������Ʈ
	if (Animator3D())
	{
		Animator3D()->UpdateData();

		for (size_t i = 0; i < (size_t)GetMtrlCount(); ++i)
		{
			if (nullptr == GetMaterial(i))
				continue;
			GetMaterial(i)->SetAnim3D(true); // Animation Mesh �˸���
			GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
		}
	}
	if (Socket())
	{
		Socket()->Regist();
	}

	// ======
	// Render
	// ======
	UINT iSubsetCount = GetMesh()->GetSubsetCount();
	for (size_t i = 0; i < iSubsetCount; ++i)
	{
		if (nullptr == GetMaterial(i))
			continue;

		GetMaterial(i)->UpdateData();
		GetMesh()->render(i);
	}	



	// =====
	// Clear
	// =====
	if (Animator2D())
		CAnimator2D::Clear();
	
	if (Animator3D())	
		Animator3D()->ClearData();	

	if (Socket())
		Socket()->Clear();
}


void CMeshRender::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

}

void CMeshRender::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);
}

void CMeshRender::SaveJsonFile(Json::Value& root)
{
	CRenderComponent::SaveJsonFile(root);
}

void CMeshRender::LoadJsonFromFile(Json::Value& root)
{
	CRenderComponent::LoadJsonFromFile(root);
}


