#include "pch.h"
#include "CLemurianHurt.h"

#include <Engine/CMeshRender.h>
#include <Engine/CMaterial.h>

CLemurianHurt::CLemurianHurt()
	:CScript((int)SCRIPT_TYPE::LEMURIANHURT)
{
}

CLemurianHurt::~CLemurianHurt()
{
}


void CLemurianHurt::enter()
{
	Animator3D()->Play(L"Hurt", false);

}

void CLemurianHurt::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
	int a = 1;
	Ptr<CMaterial> DynamicMtrl = MeshRender()->GetDynamicMaterial(0);
	//MeshRender()->SetSharedMaterial(DynamicMtrl,0);
	DynamicMtrl->SetScalarParam(SCALAR_PARAM::INT_0, &a);
}

void CLemurianHurt::exit()
{
	int a = 0;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	Animator3D()->FindAnim(L"Hurt")->Reset();
}

void CLemurianHurt::start()
{
	CState* pState = FSM()->FindState(L"Hurt");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Hurt", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianHurt::update()
{

}
