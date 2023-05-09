#include "pch.h"
#include "CFSM.h"

#include "CEventMgr.h"
#include "CTimeMgr.h"

#include "CState.h"

CFSM::CFSM()
	: CComponent(COMPONENT_TYPE::FSM)
	, m_pCurState(nullptr)
	, m_fCurTime(0.f)
{
}

CFSM::~CFSM()
{
	Safe_Del_Map(m_mapState);
}

void CFSM::finalupdate()
{
	if(nullptr == m_pCurState)
		return;
	

	m_pCurState->m_UpdateFunction();
	
	
}

void CFSM::CreateState(const wstring& _StateName,
						std::function<void()> _updatefunc,
						std::function<void()> _enterfunc,
						std::function<void()> _exitfunc)
{
	if (FindState(_StateName))
		return;

	CState* pState = new CState();
	pState->m_StateName = _StateName;
	pState->m_EnterFunction = _enterfunc;
	pState->m_UpdateFunction = _updatefunc;
	pState->m_ExitFunction = _exitfunc;
	pState->m_pOwnerAI = this;
	m_mapState.insert(make_pair(_StateName, pState));
}

CState* CFSM::FindState(const wstring& _StateName)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_StateName);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void CFSM::ChangeState(const wstring& _StateName)
{
	CState* _NextState = FindState(_StateName);

	if (nullptr == _NextState)
		return;
		
	m_pPrevState = m_pCurState;

	m_ChangeStateName = _StateName;

	tEventInfo evn = {};
	evn.eType = EVENT_TYPE::CHANGE_AI_STATE;
	evn.lParam = (DWORD_PTR)this;
	evn.wParam = (DWORD_PTR)m_ChangeStateName.c_str();

	CEventMgr::GetInst()->AddEvent(evn);
}


void CFSM::SaveToScene(FILE* _pFile)
{
	CComponent::SaveToScene(_pFile);
}

void CFSM::LoadFromScene(FILE* _pFile)
{
	CComponent::LoadFromScene(_pFile);
}

void CFSM::SaveJsonFile(Json::Value& root)
{
	
}

void CFSM::LoadJsonFromFile(Json::Value& root)
{
}


