#include "pch.h"
#include "Collider3DUI.h"
#include <Engine/CCollider3D.h>

Collider3DUI::Collider3DUI()
	:ComponentUI("Collider3DUI", COMPONENT_TYPE::COLLIDER3D)
{
	SetSize(Vec2(0.f, 100.f));
}

Collider3DUI::~Collider3DUI()
{
}


void Collider3DUI::update()
{
	ComponentUI::update();
}

void Collider3DUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CCollider3D* pCol = pTargetObject->Collider3D();

	if (nullptr == pCol)
		return;

	ImGui::Text("ColliderType");
	ImGui::SameLine();
	
	COLLIDER3D_TYPE eType = pCol->GetCollider3DType();

	const char* szColType[3] = { "CUBE", "SPHERE","SOCKET"};

	if (ImGui::BeginCombo("##ProjType", szColType[(int)eType]))
	{
		for (int i = 0; i < 3; i++)
		{
			if (ImGui::Selectable(szColType[i]))
			{
				pCol->SetCollider3DType((COLLIDER3D_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}

	float vOffsetPos[3] = { pCol->GetOffsetPos().x,pCol->GetOffsetPos().y,pCol->GetOffsetPos().z };
	ImGui::Text("OffSetPos");
	ImGui::SameLine();
	ImGui::InputFloat3("##OffPos", vOffsetPos);
	pCol->SetOffsetPos(Vec3(vOffsetPos[0], vOffsetPos[1], vOffsetPos[2]));

	float vOffsetScale[3] = { pCol->GetWorldScale().x,pCol->GetWorldScale().y,pCol->GetWorldScale().z };
	ImGui::Text("OffSetScale");
	ImGui::SameLine();
	ImGui::InputFloat3("##OffScale", vOffsetScale);
	pCol->SetOffsetScale(Vec3(vOffsetScale[0], vOffsetScale[1], vOffsetScale[2]));

	//bool _b = pCol->GetDebugObject()->IsActive();
	//ImGui::Checkbox("Active", &_b);
	//if (_b)
	//{
	//	pCol->DebugActive();
	//}
	//else
	//{
	//	pCol->DebugDeActive();
	//}

}
