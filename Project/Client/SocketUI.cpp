#include "pch.h"
#include "SocketUI.h"

#include <Engine/CSocket.h>

SocketUI::SocketUI()
	:ComponentUI("Socket",COMPONENT_TYPE::SOCKET)
{
}

SocketUI::~SocketUI()
{
}

void SocketUI::update()
{
	ComponentUI::update();
}

void SocketUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();

	CSocket* pSocket = pTargetObject->Socket();

	Vec3 vPos = pSocket->GetTransform();
	Vec3 vScale = pSocket->GetScale();
	Vec3 vRot = pSocket->GetRotation();
	vRot.ToDegree();

	bool b = pSocket->IsCollision();
	ImGui::Checkbox("Collision", &b);
	pSocket->SetCollision(b);

	ImGui::PushItemWidth(200); // Float3 위젯 간격 설정

	ImGui::Text("_Position");
	ImGui::SameLine();
	ImGui::InputFloat3("##Position", vPos);
	pSocket->SetTransform(vPos);

	ImGui::Text("_Scale   ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Scale", vScale);
	pSocket->SetScale(vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::InputFloat3("##Rotation", vRot);
	vRot.ToRadian();
	pSocket->SetRotation(vRot);
}

