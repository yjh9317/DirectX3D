#include "pch.h"
#include "LandScapeUI.h"

#include <Engine/CLandScape.h>
#include <Engine/CPathMgr.h>

#include <Engine/CTexture.h>
#include <Engine/CMaterial.h>
#include <Engine/CMeshRender.h>

LandScapeUI::LandScapeUI()
	:ComponentUI("LandScapeUI", COMPONENT_TYPE::LANDSCAPE)
	, m_bCheck(false)
{
}

LandScapeUI::~LandScapeUI()
{
}


void LandScapeUI::update()
{
	ComponentUI::update();
}

void LandScapeUI::render_update()
{
	ComponentUI::render_update();

	ImGui::PushItemWidth(200);

	CGameObject* pTargetObject = GetTargetObject();

	CLandScape* pLand = pTargetObject->LandScape();

	if (!pLand)
		return;

	bool b = pLand->IsDynamicShadow();
	ImGui::Checkbox("Shadow", &b);
	pLand->SetDynamicShadow(b);

	float vBrushScale[2] = { pLand->GetBrushScale().x,pLand->GetBrushScale().y };

	int iBrushIdx = pLand->GetBrushIdx();

	ImGui::Text("BrushXScale");
	ImGui::SameLine(100);
	ImGui::SliderFloat("##BrushXScale", &vBrushScale[0], 0, 1);


	ImGui::Text("BrushYScale");
	ImGui::SameLine(100);
	ImGui::SliderFloat("##BrushYScale", &vBrushScale[1], 0, 1); 

		pLand->SetBrushScale(Vec2(vBrushScale[0], vBrushScale[1]));

	ImGui::Text("BrushIdxs");
	ImGui::SameLine(100);
	ImGui::SliderInt("##BrushIdx", &iBrushIdx, 0, 1);
	pLand->SetBrushIdx(iBrushIdx);

	int BrushDir = pLand->GetBrushDir();
	ImGui::Text("BrushDir");
	ImGui::SameLine(100);
	ImGui::SliderInt("##BrushDir", &BrushDir, 0, 1);
	pLand->SetBrushDir(BrushDir);

	float BrushPow = pLand->GetBrushPow();
	ImGui::Text("BrushPow");
	ImGui::SameLine(100);
	ImGui::SliderFloat("##BrushPow", &BrushPow, 0, 1);
	pLand->SetBrushPow(BrushPow);

	ImGui::Text("HeightMap");
	ImGui::SameLine(100);
	ImGui::InputText("##HeightMapSave", (char*)m_str.c_str(), 50);

	if (ImGui::Button("HeightSave"))
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath();
		wstr += L"texture\\height\\";
		string tmp = m_str.c_str();
		wstring ss = wstring(tmp.begin(), tmp.end());
		wstr += ss;
		wstr += L".png";
		wstring wstr2 = CPathMgr::GetInst()->GetRelativePath(wstr);


		Ptr<CTexture> CurTex = pLand->GetHeightMap();

		ComPtr<ID3D11Texture2D> temp = CurTex->GetTex2D();

		CResMgr::GetInst()->CreateTexture(wstr2, CurTex->GetTex2D());


		Ptr<CTexture> ppTex = CResMgr::GetInst()->Load<CTexture>(wstr2, wstr2);

		pLand->SetHeightMap(ppTex);

		
	}

	ImGui::SameLine();
	if (ImGui::Button("HeightLoad"))
	{
		m_bCheck = true;
	}

	if (m_bCheck)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath();
		wstr += L"texture\\height\\";
		string tmp = m_str.c_str();
		wstring ss = wstring(tmp.begin(), tmp.end());
		wstr += ss;
		wstr += L".png";
		wstring wstr2 = CPathMgr::GetInst()->GetRelativePath(wstr);


		Ptr<CTexture> pTex =CResMgr::GetInst()->Load<CTexture>(wstr2, wstr2);

		if (nullptr == pTex)
			return;

		pLand->GetSharedMaterial(0)->SetTexParam(TEX_PARAM::TEX_0, pTex);
		pLand->SetHeightMap(pTex);

		m_bCheck = false;

	}


	int _iWeightIdx = pLand->GetWeightIndex();
	ImGui::Text("Weight Index");
	ImGui::SameLine();
	ImGui::SliderInt("##WeiIdx", &_iWeightIdx, 0, 2);
	pLand->SetWeightIndex(_iWeightIdx);

	ImGui::Text("WeightMap Save");
	ImGui::SameLine();
	ImGui::InputText("##WeightMapSave", (char*)m_WeightString.c_str(), 50);


	if (ImGui::Button("WeightSave"))
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath();
		wstr += L"texture\\weight\\";
		string tmp = m_WeightString.c_str();
		wstring ss = wstring(tmp.begin(), tmp.end());
		wstr += ss;
		wstr += L".png";
		wstring wstr2 = CPathMgr::GetInst()->GetRelativePath(wstr);

		pLand->WeightExecute();

		pLand->GetWeightMap()->Save(wstr);

		Ptr<CTexture> pTex = CResMgr::GetInst()->Load<CTexture>(wstr2, wstr2, false);
		pLand->SetWeightMap(pTex);


	}

	ImGui::SameLine();

	if (ImGui::Button("WeightLoad"))
	{
		m_bWeightCheck = true;

	}

	if (m_bWeightCheck)
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath();
		wstr += L"texture\\weight\\";
		string tmp = m_WeightString.c_str();
		wstring ss = wstring(tmp.begin(), tmp.end());
		wstr += ss;
		wstr += L".png";
		wstring wstr2 = CPathMgr::GetInst()->GetRelativePath(wstr);

		Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstr2);

		if (nullptr == pTex)
			return;

		pTex->Load(wstr, 1, true);
		if (nullptr == pTex)
		{
			return;
		}
		pLand->SetWeightMap(pTex);
		pLand->WeightLoad();
		m_bWeightCheck = false;
	}

	if (ImGui::Button("ArrayTex"))
	{
		wstring wstr = CPathMgr::GetInst()->GetContentPath();
		wstr += L"texture\\tile\\";
		string tmp = m_WeightString.c_str();
		wstring ss = wstring(tmp.begin(), tmp.end());
		wstr += ss;
		wstr += L".dds";
		wstring wstr2 = CPathMgr::GetInst()->GetRelativePath(wstr);

		vector<Ptr<CTexture>> _v = {};
		Ptr<CTexture> ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\Tex.tga", L"texture\\tile\\Tex.tga");
		_v.push_back(ppTex);
		ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_02.tga", L"texture\\tile\\TILE_02.tga");
		_v.push_back(ppTex);
		ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_03.tga", L"texture\\tile\\TILE_03.tga");
		_v.push_back(ppTex);
		ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\Tex_N.tga", L"texture\\tile\\Tex_N.tga");
		_v.push_back(ppTex);
		ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_02_N.tga", L"texture\\tile\\TILE_02_N.tga");
		_v.push_back(ppTex);
		ppTex = CResMgr::GetInst()->Load<CTexture>(L"texture\\tile\\TILE_03_N.tga", L"texture\\tile\\TILE_03_N.tga");
		_v.push_back(ppTex);

		Ptr<CTexture> ppp = CResMgr::GetInst()->CreateArrayTexture(wstr2, _v, 1, true);
		ppp->Save(wstr);

	}
}

