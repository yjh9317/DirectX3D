#include "pch.h"
#include "AnimationListUI.h"

#include "CImGuiMgr.h"
#include "SceneOutliner.h"
#include "InspectorUI.h"
#include <Engine/CEventMgr.h>
#include <Engine/CAnimator3D.h>
#include <Engine/CAnimation3D.h>

AnimationListUI::AnimationListUI()
	: UI("##AnimationListUI")
	, pObj(nullptr)
	, m_str{}
	, m_istratframe(0)
	, m_iendframe(0)
	, m_ifixstartframe(0)
	, m_ifixendframe(0)
	, m_ifixAnimSpeed(0.f)
{
}

AnimationListUI::~AnimationListUI()
{
}



void AnimationListUI::render_update()
{
	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pObj = pInspectorUI->GetTargetObject();
	
	ImGui::PushItemWidth(180);
	ImGui::SetWindowFontScale(1.5);

	if (ImGui::BeginTabBar("##AnimList"))
	{
		if (ImGui::BeginTabItem("AnimClip"))
		{
			AnimClip();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Create Anim"))
		{
			CreateAnim();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Anim Info"))
		{
			AnimInfo();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}

void AnimationListUI::AnimClip()
{
	CAnimator3D* pAnim3D = pObj->Animator3D();
	
	if (!pAnim3D)
		return;
	
	
	const vector<tMTAnimClip>* _VecClip = pAnim3D->GetAnimClip();

	static int Clipidx = 0;

	
	vector<string> _vec = {};

	for (int i = 0; i < _VecClip[0].size(); ++i)
	{
		wstring wstr = _VecClip[Clipidx][i].strAnimName;
		string str = string(wstr.begin(), wstr.end());
		_vec.push_back(str);
	}

	static int idx = 0;

	if (ImGui::BeginCombo("##ClipData", _vec[idx].c_str()))
	{
		for (int i = 0; i < _vec.size(); ++i)
		{
			if (ImGui::Selectable(_vec[i].c_str()))
			{
				idx = i;
			}
		}

		ImGui::EndCombo();
	}
	tMTAnimClip _AnimClip = _VecClip[0][idx];
	wstring wstr = _AnimClip.strAnimName;
	string str = string(wstr.begin(), wstr.end());

	ImGui::Text("Clip Idx");
	ImGui::SameLine(140);
	ImGui::InputInt("##clipidx", &Clipidx,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Start Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##startframe", &_AnimClip.iStartFrame,0,0,ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("End Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##endframe", &_AnimClip.iEndFrame,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Frame Length");
	ImGui::SameLine(140);
	ImGui::InputInt("##framelength", &_AnimClip.iFrameLength,0,0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	double starttime = _AnimClip.dStartTime;
	ImGui::Text("Start Time");
	ImGui::SameLine(140);
	ImGui::InputDouble("##starttime",&starttime,0,0,"%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	double endtime = _AnimClip.dEndTime;
	ImGui::Text("End Time");
	ImGui::SameLine(140);
	ImGui::InputDouble("##endtime",&endtime,0,0,"%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	double timelen = _AnimClip.dTimeLength;
	ImGui::Text("Time Length");
	ImGui::SameLine(140);
	ImGui::InputDouble("##timelength", &timelen, 0, 0, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	
}

void AnimationListUI::CreateAnim()
{
	ImGui::Text("CreateAnim");


	ImGui::Text("Anim Name");
	ImGui::SameLine(140);
	ImGui::InputText("##anim",(char*)m_str.c_str(),50);

	ImGui::Text("Start Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##startframe", &m_istratframe);

	ImGui::Text("End Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##endframe", &m_iendframe);

	if (ImGui::Button("Create"))
	{
		char* ch = (char*)m_str.c_str();
		string str = ch;
		wstring wstr = wstring(str.begin(), str.end());
		pObj->Animator3D()->CreateAnim(wstr, 0, m_istratframe, m_iendframe, 30);
	}
}

void AnimationListUI::AnimInfo()
{
	CAnimator3D* pAnim3D = pObj->Animator3D();

	if (!pAnim3D)
		return;
	

	map<wstring, CAnimation3D*>& AnimMap = pAnim3D->GetAnimList();

	if (!AnimMap.size())
		return;

	map<wstring, CAnimation3D*>::iterator iter = AnimMap.begin();

	vector<string> vec = {};

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
		
	if (ImGui::Button("Delete"))
	{
		string str = vec[idx];
		wstring wstr = wstring(str.begin(), str.end());

		AnimMap.erase(wstr);
		idx = 0;

	}

	if (!AnimMap.size())
		return;

	string str = vec[idx];
	wstring wstr = wstring(str.begin(), str.end());
	CAnimation3D* pAnim = AnimMap.find(wstr)->second;

	tMTAnimClip& AnimClip = pAnim->GetAnimClip();
	
	

	ImGui::Text("Anim Name");
	ImGui::SameLine(140);
	ImGui::InputText("##AnimName1", (char*)vec[idx].c_str(), 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Start Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##startframe1", &AnimClip.iStartFrame, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	ImGui::InputInt("##fixstartframe", &m_ifixstartframe);

	ImGui::Text("End Frame");
	ImGui::SameLine(140);
	ImGui::InputInt("##endframe1", &AnimClip.iEndFrame, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	ImGui::InputInt("##fixendframe", &m_ifixendframe);

	float _f = pAnim->GetSpeed();

	ImGui::Text("Speed");
	ImGui::SameLine(140);
	ImGui::InputFloat("##AnimSpeed", &_f, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine();
	ImGui::InputFloat("##fixAnimframe", &m_ifixAnimSpeed);


	ImGui::Text("Frame Length");
	ImGui::SameLine(140);
	ImGui::InputInt("##framelength1", &AnimClip.iFrameLength, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Start Time");
	ImGui::SameLine(140);
	ImGui::InputDouble("##starttime1", &AnimClip.dStartTime, 0, 0, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	ImGui::Text("End Time");
	ImGui::SameLine(140);
	ImGui::InputDouble("##endtime1", &AnimClip.dEndTime, 0, 0, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Time Length");
	ImGui::SameLine(140);
	ImGui::InputDouble("##timelength1", &AnimClip.dTimeLength, 0, 0, "%.3f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


	ImVec2 CurPos = ImGui::GetCursorPos();

	ImGui::SetCursorPos(ImVec2(335.f, 235.f));

	if (ImGui::Button("Fix",ImVec2(120.f,70.f)))
	{
		AnimClip.iStartFrame = m_ifixstartframe;
		AnimClip.iEndFrame = m_ifixendframe;
		AnimClip.iFrameLength = AnimClip.iEndFrame - AnimClip.iStartFrame;

		
		int FrameCount = pAnim->GetFrameConunt();
		AnimClip.dStartTime = AnimClip.iStartFrame / (double)FrameCount;
		AnimClip.dEndTime = AnimClip.iEndFrame / (double)FrameCount;
		AnimClip.dTimeLength = (AnimClip.iEndFrame - AnimClip.iStartFrame) / (double)FrameCount;

		if(m_ifixAnimSpeed > 0.f)
		pAnim->SetSpeed(m_ifixAnimSpeed);
	
		m_ifixstartframe = 0;
		m_ifixendframe = 0;
	}

	
}
