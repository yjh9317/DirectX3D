#include "pch.h"
#include "MeshRenderUI.h"

#include "CImGuiMgr.h"
#include "ListUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>

MeshRenderUI::MeshRenderUI()
	: ComponentUI("MeshRender", COMPONENT_TYPE::MESHRENDER)
{
	SetSize(Vec2(0.f, 100.f));
}

MeshRenderUI::~MeshRenderUI()
{

}

void MeshRenderUI::update()
{
	ComponentUI::update();

}

void MeshRenderUI::render_update()
{
	ComponentUI::render_update();

	ImGui::PushItemWidth(200);


	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	Ptr<CMesh> pMesh = pMeshRender->GetMesh();
	Ptr<CMaterial> pMtrl = pMeshRender->GetMaterial(0);


	bool b = pMeshRender->IsDynamicShadow();
	ImGui::Checkbox("Shadow", &b);
	pMeshRender->SetDynamicShadow(b);

	UINT MtrlCnt = pMeshRender->GetMtrlCount();

	vector<string> vMtrlName = {};

	for (int i = 0; i < MtrlCnt; ++i)
	{
		if (nullptr == pMeshRender->GetMaterial(i))
			continue;

		wstring wstr = pMeshRender->GetMaterial(i).Get()->GetKey();
		string str = string(wstr.begin(), wstr.end());
		vMtrlName.push_back(str);
	}

	static int ICurMtrl = 0;

	string strMeshName, strMtrlName;
	if(nullptr != pMesh)
		strMeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());
	if(nullptr != pMtrl)
		strMtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());
	
	ImGui::Text("Mesh");	
	ImGui::SameLine(86.f); 

	ImGui::InputText("##MeshName", (char*)strMeshName.c_str(), strMeshName.capacity(), ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		DWORD_PTR dwData = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Resource"))
		{
			memcpy(&dwData, payload->Data, sizeof(DWORD_PTR));
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MeshListBtn", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::MESH);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Mesh List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&MeshRenderUI::MeshSelect);
	}

	if (0 != vMtrlName.size())
	{

		ImGui::Text("Material");

		ImGui::SameLine(86.f);

		if (ImGui::BeginCombo("##Mtrl", vMtrlName[0].c_str()))
		{
			for (int i = 0; i < MtrlCnt; i++)
			{
				if (ImGui::Selectable(vMtrlName[i].c_str()))
				{
					ICurMtrl = i;
					Ptr<CMaterial> tmp = pMeshRender->GetMaterial(ICurMtrl);
					pMeshRender->SetSharedMaterial(pMtrl, ICurMtrl);
					pMeshRender->SetSharedMaterial(tmp, 0);
				}
			}

			ImGui::EndCombo();
		}
	}
	else
	{
		ImGui::Text("Material");
		ImGui::SameLine(86.f);
		ImGui::InputText("##MaterialName", (char*)strMtrlName.c_str(), strMtrlName.capacity(), ImGuiInputTextFlags_ReadOnly);
	}

	ImGui::SameLine(); 
	if (ImGui::Button("##MaterialListBtn", Vec2(15, 15)))
	{
		// ListUI 활성화한다.
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResList(RES_TYPE::MATERIAL);
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Material List");

		for (const auto& pair : mapRes)
		{
			pListUI->AddList(string(pair.first.begin(), pair.first.end()));
		}

		pListUI->Activate();
		pListUI->SetDBCEvent(this, (DBCLKED)&MeshRenderUI::MtrlSelect);
	}
}

void MeshRenderUI::MeshSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strMeshKey = wstring(strSelectedName.begin(), strSelectedName.end());

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMeshKey);
	assert(pMesh.Get());

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	pMeshRender->SetMesh(pMesh);
}

void MeshRenderUI::MtrlSelect(DWORD_PTR _param)
{
	string strSelectedName = (char*)_param;
	wstring strMtrlKey = wstring(strSelectedName.begin(), strSelectedName.end());

	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strMtrlKey);
	assert(pMtrl.Get());

	CMeshRender* pMeshRender = GetTargetObject()->MeshRender();
	pMeshRender->SetSharedMaterial(pMtrl, 0);
}


