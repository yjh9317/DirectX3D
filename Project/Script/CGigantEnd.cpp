#include "pch.h"
#include "CGigantEnd.h"


CGigantEnd::CGigantEnd()
	:CScript((int)SCRIPT_TYPE::GIGANTEND)
{
}

CGigantEnd::~CGigantEnd()
{
}

void CGigantEnd::enter()
{
	Animator3D()->Play(L"LoopEnd", false);
	Animator3D()->GetCurAnim()->SetLerpTime(0.f);
}

void CGigantEnd::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Idle");
	}
}

void CGigantEnd::exit()
{
	Animator3D()->FindAnim(L"LoopEnd")->Reset();
}

void CGigantEnd::start()
{
	CState* pState = FSM()->FindState(L"LoopEnd");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"LoopEnd", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CGigantEnd::update()
{

}
