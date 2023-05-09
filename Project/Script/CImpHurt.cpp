#include "pch.h"
#include "CImpHurt.h"


CImpHurt::CImpHurt()
	:CScript((int)SCRIPT_TYPE::IMPHURT)
{
}

CImpHurt::~CImpHurt()
{
}

void CImpHurt::enter()
{
	Animator3D()->Play(L"Hurt", false);

}

void CImpHurt::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
	int a = 1;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	
}

void CImpHurt::exit()
{
	int a = 0;
	MeshRender()->GetDynamicMaterial(0)->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	Animator3D()->FindAnim(L"Hurt")->Reset();
}

void CImpHurt::start()
{
	CState* pState = FSM()->FindState(L"Hurt");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Hurt", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CImpHurt::update()
{

}

