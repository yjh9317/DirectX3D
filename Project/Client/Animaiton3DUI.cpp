#include "pch.h"
#include "Animaiton3DUI.h"

#include "CImGuiMgr.h"

#include <Engine/CAnimation3D.h>
#include <Engine/CAnimator3D.h>
#include "AnimationListUI.h"
#include "ListUI.h"

using std::to_wstring;

Animaiton3DUI::Animaiton3DUI()
	:ComponentUI("Animator3D", COMPONENT_TYPE::ANIMATOR3D)
{
	SetSize(Vec2(0.f, 300.f));
}

Animaiton3DUI::~Animaiton3DUI()
{
}

void Animaiton3DUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;
}

void Animaiton3DUI::render_update()
{
	ComponentUI::render_update();

	ImGui::PushItemWidth(180.f);
	
	

	CGameObject* pTargetObject = GetTargetObject();
	if (nullptr == pTargetObject)
		return;

	CAnimator3D* pAnim3D = pTargetObject->Animator3D();

	ImGui::SameLine();

	if (ImGui::Button("AnimationInfo"))
	{
		AnimationListUI* pUI = (AnimationListUI*)CImGuiMgr::GetInst()->FindUI("##AnimationListUI");
		pUI->SetSize(Vec2(500.f, 500.f));
		pUI->Activate();
	}

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ImGui::SameLine();

	const vector<tMTBone>* BoneInfo = pAnim3D->GetBoneInfo();
	int BoneCount = pAnim3D->GetBoneCount();

	vector<string> BoneNameVec = {};

	for (int i = 0; i < BoneCount; ++i)
	{
		wstring BoneName = to_wstring(i) + L".";
		BoneName += BoneInfo[0][i].strBoneName;

		BoneNameVec.push_back(string(BoneName.begin(), BoneName.end()));

	}
	if (ImGui::Button("BoneList"))
	{
		ListUI* pListUI = (ListUI*)CImGuiMgr::GetInst()->FindUI("##ListUI");
		pListUI->Clear();
		pListUI->SetTitle("Bone List");

		for (int i = 0; i < BoneNameVec.size(); ++i)
		{
			pListUI->AddList(string(BoneNameVec[i].begin(), BoneNameVec[i].end()));
		}

		pListUI->Activate();
	}

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	ImGui::Text("Bone Idx ");
	ImGui::SameLine();

	ImGui::PushItemWidth(90.f);

	int _idx = (int)pAnim3D->GetBoneIdx();
	ImGui::InputInt("##boneidx", &_idx,0);
	pAnim3D->SetBoneIdx(_idx);

	ImGui::SameLine();

	int _idx2 = (int)pAnim3D->GetBoneIdx2();
	ImGui::InputInt("##boneidx2", &_idx2,0);
	pAnim3D->SetBoneIdx2(_idx2);

	ImGui::PushItemWidth(180.f);

	bool _repeat = pAnim3D->IsRepeat();
	bool _play = pAnim3D->IsPlay();

	ImGui::Checkbox("Repeat",&_repeat);
	pAnim3D->SetRepeat(_repeat);

	ImGui::SameLine();

	ImGui::Checkbox("Play", &_play);
	pAnim3D->SetPlay(_play);



	ImGui::Text("Save :");
	ImGui::SameLine();
	ImGui::InputText("##SaveAnimator", (char*)StrSave.c_str(), 50);
	ImGui::SameLine();
	if (ImGui::Button("Save"))
	{
		string str = StrSave.c_str();
		wstring wstr = wstring(str.begin(), str.end());
		pAnim3D->Save(wstr);
	}

	ImGui::Text("Load :");
	ImGui::SameLine();
	ImGui::InputText("##LoadAnimator", (char*)StrLoad.c_str(), 50);
	ImGui::SameLine();
	if (ImGui::Button("Load"))
	{
		string str = StrLoad.c_str();
		wstring wstr = wstring(str.begin(), str.end());
		pAnim3D->Load(wstr);
	}

	CAnimation3D* pAnim = pAnim3D->GetCurAnim();

	if (!pAnim)
		return;

	tMTAnimClip _AnimClip = pAnim->GetAnimClip();

	wstring wstr = _AnimClip.strAnimName;
	string str = string(wstr.begin(), wstr.end());

	ImGui::Text("Anim Name");
	ImGui::SameLine(85);
	ImGui::InputText("##Animname", (char*)str.c_str(), 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	float _fDur = _AnimClip.dTimeLength;
	ImGui::Text("Duration");
	ImGui::SameLine(85);
	ImGui::InputFloat("##Duration",&_fDur,0,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	int _iStartFrame = _AnimClip.iStartFrame;
	ImGui::Text("Start Frame");
	ImGui::SameLine(85);
	ImGui::InputInt("##StartFrame",&_iStartFrame,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	int _iEndFrame = _AnimClip.iEndFrame;
	ImGui::Text("End Frame");
	ImGui::SameLine(85);
	ImGui::InputInt("##EndFrame",&_iEndFrame,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	int _CurFrame = pAnim->GetCurFrameIdx();
	ImGui::Text("Cur Frame");
	ImGui::SameLine(85);
	ImGui::SliderInt("##CurFrame",&_CurFrame,_iStartFrame,_iEndFrame-1,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	float _LerpTime = pAnim->GetLerpTime();
	ImGui::Text("Lerp Time");
	ImGui::SameLine();
	ImGui::SliderFloat("##LerpTime",&_LerpTime,0,1);
	pAnim->SetLerpTime(_LerpTime);

	map<wstring, CAnimation3D*> AnimMap = pAnim3D->GetAnimList();
	map<wstring, CAnimation3D*>::iterator iter = AnimMap.begin();

	vector<string> vec = {};

	if (!AnimMap.size())
		return;

	for (; iter != AnimMap.end(); ++iter)
	{
		wstring wstr = iter->first;
		string str = string(wstr.begin(), wstr.end());
		vec.push_back(str);
	}

	static int idx = 0;

	if (ImGui::BeginCombo("##AnimList", vec[idx].c_str()))
	{
		for (int i = 0; i < vec.size(); ++i)
		{
			if (ImGui::Selectable(vec[i].c_str()))
			{
				idx = i;
			}
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine();

	if (ImGui::Button("Play Button"))
	{
		string str = vec[idx];
		wstring wstr = wstring(str.begin(), str.end());
		pAnim3D->Play(wstr, _repeat);
	}


	//void ResourceUI::FindFileName(const wstring & _strFolderPath)
	//{
	//	wstring strContent = _strFolderPath + L"*.*";

	//	WIN32_FIND_DATA FindFileData = {};

	//	HANDLE hFind = nullptr;

	//	hFind = FindFirstFile(strContent.c_str(), &FindFileData);

	//	if (hFind == INVALID_HANDLE_VALUE)
	//		return;

	//	while (FindNextFile(hFind, &FindFileData))
	//	{
	//		if (FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	//		{
	//			if (0 == wcscmp(FindFileData.cFileName, L".."))
	//				continue;

	//			FindFileName(_strFolderPath + FindFileData.cFileName + L"\\");
	//			continue;
	//		}

	//		wstring strRelativePath = _strFolderPath + FindFileData.cFileName;
	//		strRelativePath = CPathMgr::GetInst()->GetRelativePath(strRelativePath);

	//		m_vecResPath.push_back(strRelativePath);
	//	}

	//	FindClose(hFind);
	//}

	

}
