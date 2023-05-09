#pragma once
#include "CComponent.h"

class CState;

class CFSM :
    public CComponent
{
private:
    map<wstring, CState*>    m_mapState;
    CState*                  m_pCurState;
    CState*                  m_pPrevState;
    float                    m_fCurTime;
    

    wstring                  m_ChangeStateName;

public:
    virtual void finalupdate() override;
    void ChangeState(const wstring& _StateName);
    void CreateState(const wstring& _StateName, std::function<void()> _updatefunc, std::function<void()> _enterfunc, std::function<void()> _exitfunc);
    CState* FindState(const wstring& _StateName);

    CState* GetCurState() { return m_pCurState; }
    CState* GetPrevState() { return m_pPrevState; }
    float GetStateTime() { return m_fCurTime; }

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
    CLONE(CFSM)

public:
    CFSM();
    ~CFSM();

    friend class CEventMgr;
};

