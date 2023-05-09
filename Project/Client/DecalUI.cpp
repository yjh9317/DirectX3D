#include "pch.h"
#include "DecalUI.h"

#include <Engine/CDecal.h>

DecalUI::DecalUI()
	:ComponentUI("DecalUI",COMPONENT_TYPE::DECAL)
{
}

DecalUI::~DecalUI()
{
}


void DecalUI::update()
{
	ComponentUI::update();
}

void DecalUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CDecal* pDecal = pTargetObject->Decal();

	if (!pDecal)
		return;

	bool _b = pDecal->IsUseDeferredLighting();
	ImGui::Checkbox("IsLighting", &_b);
	pDecal->SetDeferredLighting(_b);


	ImGui::Text("DecalType");
	ImGui::SameLine();

	DECAL_TYPE eType = pDecal->GetDecalType();

	const char* szDecalType[2] = { "SPHERE", "CUBE" };

	if (ImGui::BeginCombo("##DecalType", szDecalType[(int)eType]))
	{
		for (int i = 0; i < 2; i++)
		{
			if (ImGui::Selectable(szDecalType[i]))
			{
				pDecal->SetDecalType((DECAL_TYPE)i);
			}
		}

		ImGui::EndCombo();
	}
}

