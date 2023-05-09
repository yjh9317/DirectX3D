#include "pch.h"
#include "TransformUI.h"

#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("Transform", COMPONENT_TYPE::TRANSFORM)	
{
	SetSize(Vec2(0.f, 120.f));
}

TransformUI::~TransformUI()
{
}

void TransformUI::update()
{
	ComponentUI::update();

	CGameObject* pTargetObj = GetTargetObject();
	if (nullptr == pTargetObj)
		return;
		
	CTransform* pTransform = pTargetObj->Transform();

	m_vRelativePos = pTransform->GetRelativePos();
	m_vRelativeScale = pTransform->GetRelativeScale();
	m_vRelativeRot = pTransform->GetRelativeRotation();
}

void TransformUI::render_update()
{
	ComponentUI::render_update();


	CGameObject* pTargetObject = GetTargetObject();

	ImGui::SameLine();
	bool _b = pTargetObject->Transform()->GetIgnoreParentScale();
	ImGui::Checkbox("IgnoreParentScale", &_b);
	pTargetObject->Transform()->SetIgnoreParentScale(_b);

	ImGui::SameLine();
	bool _t = pTargetObject->Transform()->GetIgnoreParentTransform();
	ImGui::Checkbox("IgnoreParentTransform", &_t);
	pTargetObject->Transform()->SetIgnoreParentTransform(_t);

	CTransform* pTrans = pTargetObject->Transform();
	Vec3 vPos = pTrans->GetRelativePos();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRotation();
	vRot.ToDegree();


	ImGui::PushItemWidth(200); // Float3 ���� ���� ����
	
	ImGui::Text("Relative Position");	
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", vPos);
	pTrans->SetRelativePos(vPos);

	ImGui::Text("Relative Scale   ");
	ImGui::SameLine();	
	ImGui::InputFloat3("##Scale", vScale);
	pTrans->SetRelativeScale(vScale);

	ImGui::Text("Relative Rotation");
	ImGui::SameLine();	
	ImGui::InputFloat3("##Rotation", vRot);
	vRot.ToRadian();
	pTrans->SetRelativeRotation(vRot);

	int _ilayeridx = pTargetObject->GetLayerIndex();
	ImGui::InputInt("LayerIdx :", &_ilayeridx, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
}