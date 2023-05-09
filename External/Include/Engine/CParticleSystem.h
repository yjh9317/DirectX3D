#pragma once
#include "CRenderComponent.h"

#include "CStructuredBuffer.h"
#include "CParticleUpdateShader.h"

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleUpdateShader>      m_CS;           // ��ƼŬ ������Ʈ ���̴�

    CStructuredBuffer*              m_ParticleBuffer;   // ��ƼŬ ����ȭ ����
    CStructuredBuffer*              m_DataBuffer;       // �߰� ������ ����(��ƼŬ AliveCount)
    UINT                            m_iMaxCount;        // ��ƼŬ �ִ� ����

    int                             m_bPosInherit;      // ������Ʈ ��ǥ�� ����
    int                             m_iAliveCount;      // �̹� �����ӿ� ����(Ȱ��ȭ) �� ��ƼŬ ��
    
    float                           m_fMinLifeTime;     // ��ƼŬ�� �ּ� ���� �ð�
    float                           m_fMaxLifeTime;     // ��ƼŬ�� �ִ� ���� �ð�

    float                           m_fStartSpeed;      // ��ƼŬ�� �ּ� �ӷ�
    float                           m_fEndSpeed;        // ��ƼŬ�� �ִ� �ӷ�

    Vec4                            m_vStartColor;      // ��ƼŬ �ʱ� ����
    Vec4                            m_vEndColor;        // ��ƼŬ ���� ����

    Vec3                            m_vStartScale;      // ��ƼŬ �ʱ� ũ��
    Vec3                            m_vEndScale;        // ��ƼŬ ���� ũ��


    float                           m_fParticleCreateDistance;  // ��ƼŬ ���� ����
    float                           m_fParticleCreateTerm;      // ��ƼŬ ���� ����
    float                           m_fAccTime;
    
    int                             m_iEmissive;        // ��ƼŬ �߱�����
    int                             m_iLighting;        // ���� ���� ����

public:
    void SetEmissive(bool _bEmissive);
    void SetApplyLighting(bool _bLighting);
    void SetMaxParticleCount(UINT _iMax);

    bool GetEmissive() { return m_iEmissive; }
    bool GetApplyLighting() { return m_iLighting; }

    int GetAliveCount() { return m_iAliveCount; }
    void SetAliveCount(int _i) { m_iAliveCount = _i; }

    float GetMaxLifeTime() { return m_fMaxLifeTime; }
    void SetMaxLifeTime(float _f) { m_fMaxLifeTime = _f; }

    float GetMinLifeTime() { return m_fMinLifeTime; }
    void SetMinLifeTime(float _f) { m_fMinLifeTime = _f; }

    float GetStartSpeed() { return m_fStartSpeed; }
    void SetStartSpeed(float _f) { m_fStartSpeed = _f; }

    float GetEndSpeed() { return m_fEndSpeed; }
    void SetEndSpeed(float _f) { m_fEndSpeed = _f; }

    Vec4 GetStartColor() { return m_vStartColor; }
    void SetStartColor(Vec4 _v) { m_vStartColor = _v; }

    Vec4 GetEndColor() { return m_vEndColor; }
    void SetEndColor(Vec4 _v) { m_vEndColor = _v; }

    Vec3 GetStartScale() { return m_vStartScale; }
    void SetStartScale(Vec3 _v) { m_vStartScale = _v; }

    Vec3 GetEndScale() { return m_vEndScale; }
    void SetEndScale(Vec3 _v) { m_vEndScale = _v; }

    float GetCreateDist() { return m_fParticleCreateDistance; }
    void SetCreateDist(float _f) { m_fParticleCreateDistance = _f; }

    float GetCreateTerm() { return m_fParticleCreateTerm; }
    void SetCreateTerm(float _f) { m_fParticleCreateTerm = _f; }
    

public:
    virtual void finalupdate() override;
    virtual void render()   override;


public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;

    virtual void SaveJsonFile(Json::Value& root) override;
    virtual void LoadJsonFromFile(Json::Value& root) override;
    CLONE(CParticleSystem);
public:
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _origin);
    ~CParticleSystem();
};

