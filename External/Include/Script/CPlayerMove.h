#pragma once
#include <Engine/CScript.h>

enum class MOVE_TYPE
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};


class CPlayerMove :
    public CScript
{
private:
    float   m_fSpeed;
    MOVE_TYPE m_eDir;

public:
    virtual void enter() override;
    virtual void StateUpdate() override;
    virtual void exit() override;

    virtual void start() override;
    virtual void update();

    void SetDir(MOVE_TYPE _dir) { m_eDir = _dir; }

    CLONE(CPlayerMove)

public:
    CPlayerMove();
    ~CPlayerMove();
};

