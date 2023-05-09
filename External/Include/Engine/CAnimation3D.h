#pragma once
#include "CEntity.h"

class CAnimator3D;

class CAnimation3D :
    public CEntity
{
private:
    CAnimator3D*                m_pOwner;

    // �ִϸ��̼� ����
    tMTAnimClip                 m_AnimInfo;
    int                         m_iFrameCount;
    int                         m_iClipIdx;
                
    // �ִϸ��̼� ������Ʈ �� ����
    bool		                m_bFinish;
    float                       m_fTime;
    int                         m_iPrevFrameIdx;
    int                         m_iFrameIdx;
    int                         m_iNextFrameIdx;
    float                       m_fRatio;
    float                       m_fSpeed;

    // �ִϸ��̼� ���� �� ����
    float                       m_fLerpTime;
    bool                        m_bIsLerp;

    // ����ȭ ����
    CStructuredBuffer*          m_pBoneFinalMatBuffer;  // Ư�� �������� ���� ���
    bool						m_bFinalMatUpdate; // ������� ���� ���࿩��
    
public:
    void finalupdate();


    CLONE(CAnimation3D)

public:
    CAnimation3D* CreateAnim(const wstring& _strName, int _ClipIdx, float _fStart, float _fEnd, int _AnimFrame);
    void Reset()
    {
        m_bFinish = false;
        m_fTime = 0.f;
        m_iFrameIdx = m_AnimInfo.iStartFrame;
    }

    int GetPrevanimFrame() { return m_iPrevFrameIdx; }
    void SetPrevAnimFrame(int _frame) { m_iPrevFrameIdx = _frame; }
    

    int GetCurFrameIdx() { return m_iFrameIdx; }
    void SetCurFrameIdx(int _frame) { m_iFrameIdx = _frame; }

    int GetNextFrameIdx() { return m_iNextFrameIdx; }
    int GetFrameConunt() {     return m_iFrameCount;    }
    int GetClipIdx() { return m_iClipIdx; }
    float GetRatio() { return m_fRatio; }
    bool  IsFinish() { return m_bFinish; }

    void SetLerp(bool _b) { m_bIsLerp = _b; }
    bool GetLerp() { return m_bIsLerp; }

    float GetLerpTime() { return m_fLerpTime; }
    void SetLerpTime(float _f) { m_fLerpTime = _f; }

    float GetSpeed() { return m_fSpeed; }
    void SetSpeed(float _f) { m_fSpeed = _f; }

    tMTAnimClip& GetAnimClip() { return m_AnimInfo; }

    virtual void SaveToScene(FILE* _pFile);
    virtual void LoadFromScene(FILE* _pFile);

    virtual void SaveJsonFile(Json::Value& root, const string& str) override;
    virtual void LoadJsonFromFile(Json::Value& root, const string& str) override;

public:
    CAnimation3D();
    CAnimation3D(const CAnimation3D& _origin);
    ~CAnimation3D();

    friend class CAnimator3D;
};

