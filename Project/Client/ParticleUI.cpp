#include "pch.h"
#include "ParticleUI.h"

#include <Engine/CParticleSystem.h>

ParticleUI::ParticleUI()
	:ComponentUI("ParticleUI",COMPONENT_TYPE::PARTICLESYSTEM)
{
}

ParticleUI::~ParticleUI()
{
}

void ParticleUI::update()
{
	ComponentUI::update();
}

void ParticleUI::render_update()
{
	ComponentUI::render_update();

	CGameObject* pTargetObject = GetTargetObject();
	CParticleSystem* pParticle = pTargetObject->ParticleSystem();

	if (!pParticle)
		return;

	bool IsLighting = pParticle->GetApplyLighting();
	ImGui::Checkbox("Lighting", &IsLighting);
	pParticle->SetApplyLighting(IsLighting);

	ImGui::SameLine();

	bool Emissive = pParticle->GetEmissive();
	ImGui::Checkbox("Emissive", &Emissive);
	pParticle->SetEmissive(Emissive);


	ImGui::SameLine();
	int _Alive = pParticle->GetAliveCount();
	bool _b = false;

	if (_Alive)
		_b = true;
	else
		_b = false;

	ImGui::Checkbox("_Alive", &_b);

	if (_b)
		_Alive = 1;
	else
		_Alive = 0;

	pParticle->SetAliveCount(_Alive);

	float _Dist = pParticle->GetCreateDist();
	ImGui::Text("Distance");
	ImGui::SameLine(120);
	ImGui::InputFloat("##ParticleDist", &_Dist);

	pParticle->SetCreateDist(_Dist);

	float _Term = pParticle->GetCreateTerm();
	ImGui::Text("Term");
	ImGui::SameLine(120);
	ImGui::InputFloat("##ParticleTerm", &_Term);

	pParticle->SetCreateTerm(_Term);


	float _minlife = pParticle->GetMinLifeTime();
	ImGui::Text("MinLife");
	ImGui::SameLine(120);
	ImGui::InputFloat("##ParticleMin", &_minlife);
	pParticle->SetMinLifeTime(_minlife);

	float _maxlife = pParticle->GetMaxLifeTime();
	ImGui::Text("MaxLife");
	ImGui::SameLine(120);
	ImGui::InputFloat("##ParticleMax", &_maxlife);
	pParticle->SetMaxLifeTime(_maxlife);


	float _StartScale[3] = { pParticle->GetStartScale().x,pParticle->GetStartScale().y,pParticle->GetStartScale().z };
	ImGui::Text("StartScale");
	ImGui::SameLine(120);
	ImGui::InputFloat3("##StartScale", _StartScale);
	pParticle->SetStartScale(Vec3(_StartScale[0], _StartScale[1], _StartScale[2]));

	float _EndScale[3] = { pParticle->GetEndScale().x,pParticle->GetEndScale().y,pParticle->GetEndScale().z };
	ImGui::Text("EndScale");
	ImGui::SameLine(120);
	ImGui::InputFloat3("##EndScale", _EndScale);
	pParticle->SetEndScale(Vec3(_EndScale[0], _EndScale[1], _EndScale[2]));

	float _StartSpeed = pParticle->GetStartSpeed();
	ImGui::Text("StartSpeed");
	ImGui::SameLine(120);
	ImGui::InputFloat("##_StartSpeed", &_StartSpeed);
	pParticle->SetStartSpeed(_StartSpeed);

	float _EndSpeed = pParticle->GetEndSpeed();
	ImGui::Text("EndSpeed");
	ImGui::SameLine(120);
	ImGui::InputFloat("##_EndSpeed", &_EndSpeed);
	pParticle->SetEndSpeed(_EndSpeed);

	float _StartColor[4] = { pParticle->GetStartColor().x,pParticle->GetStartColor().y,pParticle->GetStartColor().z,pParticle->GetStartColor().w };
	ImGui::Text("StartColor");
	ImGui::SameLine(120);
	ImGui::InputFloat4("##StartColor", _StartColor);
	pParticle->SetStartColor(Vec4(_StartColor[0],_StartColor[1],_StartColor[2],_StartColor[3]));

	float _EndColor[4] = { pParticle->GetEndColor().x,pParticle->GetEndColor().y,pParticle->GetEndColor().z,pParticle->GetEndColor().w };;
	ImGui::Text("EndColor");
	ImGui::SameLine(120);
	ImGui::InputFloat4("##EndColor", _EndColor);
	pParticle->SetEndColor(Vec4(_EndColor[0], _EndColor[1], _EndColor[2], _EndColor[3]));


}
