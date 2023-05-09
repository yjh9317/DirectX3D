class CFSM;


class CState
{

    wstring         m_StateName;
    CFSM*           m_pOwnerAI;         // �����ϰ� �ִ� FSM�ּ�
    float           m_fStateTime;       // State �ִ� �ð�

    std::function<void()> m_EnterFunction;
    std::function<void()> m_UpdateFunction;
    std::function<void()> m_ExitFunction;


public:
    void Update();

public:
    const wstring& GetStateName() { return m_StateName; }
    float GetStateTime() { return m_fStateTime; }
    CFSM* GetFSM() {  return m_pOwnerAI;  }


public:
    CState();
    ~CState();

    friend class CEventMgr;
    friend class CFSM;
};
