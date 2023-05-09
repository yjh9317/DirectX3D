#include "pch.h"
#include "CLemurianPre.h"

CLemurianPre::CLemurianPre()
	:CScript((int)SCRIPT_TYPE::LEMURIANPRE)
{
}

CLemurianPre::~CLemurianPre()
{
}


void CLemurianPre::enter()
{
	Animator3D()->Play(L"FireballBefore", false);
}

void CLemurianPre::StateUpdate()
{
	if (Animator3D()->GetCurAnim()->IsFinish())
	{
		FSM()->ChangeState(L"Fireball");
	}
}

void CLemurianPre::exit()
{
	Animator3D()->FindAnim(L"FireballBefore")->Reset();
}

void CLemurianPre::start()
{
	CState* pState = FSM()->FindState(L"FireballBefore");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"FireballBefore", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

void CLemurianPre::update()
{

}

