#include "pch.h"
#include "CBisonHurt.h"

CBisonHurt::CBisonHurt()
	:CScript((int)SCRIPT_TYPE::BISONHURT)
{
}

CBisonHurt::~CBisonHurt()
{
}

void CBisonHurt::enter()
{
}

void CBisonHurt::StateUpdate()
{
}

void CBisonHurt::exit()
{
}

void CBisonHurt::start()
{
	CState* pState = FSM()->FindState(L"Hurt");
	if (nullptr == pState)
	{
		FSM()->CreateState(L"Hurt", [&] {this->StateUpdate(); },
			[&] {this->enter(); },
			[&] {this->exit(); });
	}
}


void CBisonHurt::update()
{
}

