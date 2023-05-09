#include "pch.h"
#include "CChestOpened.h"

CChestOpened::CChestOpened()
	:CScript((int)SCRIPT_TYPE::CHESTOPENED)
{
}

CChestOpened::~CChestOpened()
{
}

void CChestOpened::enter()
{
	Animator3D()->Play(L"Opened", false);
}

void CChestOpened::StateUpdate()
{
}

void CChestOpened::exit()
{
}

void CChestOpened::update()
{
	CState* pState = FSM()->FindState(L"Opened");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Opened", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}

