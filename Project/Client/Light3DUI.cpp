#include "pch.h"
#include "Light3DUI.h"
#include <Engine/CLight3D.h>

Light3DUI::Light3DUI()
	:ComponentUI("Light3DUI",COMPONENT_TYPE::LIGHT3D)
{
}

Light3DUI::~Light3DUI()
{
}


void Light3DUI::update()
{
	ComponentUI::update();
}

void Light3DUI::render_update()
{
	ComponentUI::render_update();


	CGameObject* pTargetObject = GetTargetObject();
	CLight3D* pLight = pTargetObject->Light3D();

}
