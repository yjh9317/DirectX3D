#include "pch.h"
#include "FSMUI.h"

FSMUI::FSMUI()
	:ComponentUI("FSMUI", COMPONENT_TYPE::FSM)
{
	SetSize(Vec2(0.f, 30.f));
}

FSMUI::~FSMUI()
{
}

void FSMUI::update()
{
	ComponentUI::update();
}

void FSMUI::render_update()
{
	ComponentUI::render_update();


}